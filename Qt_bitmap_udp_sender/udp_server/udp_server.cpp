#include "udp_server.h"
#include "config.h"

#include <QCoreApplication>
#include <QBuffer>
#include <QDir>
#include <QtGui/QImage>
#include <QList>
#include <QByteArray>
#include <QTime>
#include <QFileDialog>


namespace udpServer {


UDPserver::UDPserver(QObject *parent):QObject(parent),
     status(nullptr)
{
    receiver_ = new QUdpSocket(this);
    receiver_->bind(QHostAddress::LocalHost,RECIVE_PORT);
    picture_fileName = DEFAULT_PICTURE;

    connect(receiver_,SIGNAL(readyRead()),this,SLOT(serverReceive()));
}

void UDPserver::serverReceive()
{

    qDebug()<<"Data receiving from client ...";
    if(status != nullptr)
       status->setText("Data receiving from client ..." );

    QByteArray buffer;

    buffer.resize(receiver_->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    receiver_->readDatagram(buffer.data(), buffer.size(), &sender , &senderPort);

    QString QbuffStr(buffer);
    QJsonDocument buffResponse = QJsonDocument::fromJson(QbuffStr.toUtf8());

    QJsonObject jsonObj = buffResponse.object();

    if(jsonObj.find("start") != jsonObj.end())
        serverFeedBack();
}


void UDPserver::set_statusLabel(QLabel* status_label)
{
    status = status_label;
}

void UDPserver::serverFeedBack()
{

    qDebug() << "START accepted. loading picture...";
    if(status != nullptr)
       status->setText("accepted Start Json from client" );

    loadPicture( );
}

void UDPserver::loadPicture( )
{

   QImage *img = new QImage(picture_fileName, "BMP");

   QByteArray base64Image;

   if(!img->isNull())
   {
       QBuffer buffer;
       buffer.open(QIODevice::ReadWrite);
       img->save(&buffer, "bmp");
       //serialization
       base64Image = buffer.buffer().toBase64();
   }
   else
   {
       qDebug() << "ERROR: cannot read Image " << picture_fileName;
       if(status != nullptr)
          status->setText("ERROR: cannot open this file, try from current DIR :) ");
   }


     //split serialized image, because datagram size must be less than 1500 bytes
     quint32 pos = 0;
     quint32 arrsize = base64Image.size();
     QList<QByteArray> arrays;
     quint32 id = 0;

     if(status != nullptr)
        status->setText("proccess image... ");
     while(pos<arrsize){

         //read next memory block for to pack in datagram
         QByteArray arr = base64Image.mid(pos, DATAGRAM_SIZE);
         quint32 datagram_size = arr.size();

         //write datagram info to the tail of bytearray
         arr.append(reinterpret_cast<const char *>(&pos), sizeof(quint32));
         arr.append(reinterpret_cast<const char *>(&datagram_size), sizeof(quint32));
         arr.append(reinterpret_cast<const char *>(&arrsize), sizeof(quint32));

         arrays << arr;
         pos+=DATAGRAM_SIZE;

         // just for test
         //quint32 pos, picture_size;
         //QDataStream stream_read(&arr, QIODevice::ReadOnly);
         //stream_read >> pos
         //       >> datagram_size
         //       >> picture_size;
         //qDebug() << "pos="        << *reinterpret_cast<quint32*>(&arr[arr.size() - 3*sizeof(quint32)])
         //        << " datagram_size="  << *reinterpret_cast<quint32*>(&arr[arr.size() - 2*sizeof(quint32)])
         //        << " picture_size=" << *reinterpret_cast<quint32*>(&arr[arr.size() -   sizeof(quint32)]) ;
         // !just for test
     }


     if(status != nullptr)
        status->setText("sending UDP to client... pos = " + QString("%1").arg(pos));
     //send each part in UDP
     for (auto& arr: arrays)
     {
         qDebug()  << "id "<< id << " arr.size() = " << arr.size();
         //qDebug() << arr;

         receiver_->writeDatagram(arr, QHostAddress::LocalHost, SEND_PORT);

         id++;

         delay(5); //if send immediate - possible problem of quality on client side
     }

        //test deserialization on server side
        //     QByteArray raw_image;
        //     for( auto& i : arrays)
        //     {
        //          raw_image.append(i);
        //     }
        //     qDebug() << "raw image size = " << raw_image.size();
        //     //qDebug() << "raw image: " << raw_image;
        //     img->loadFromData(QByteArray::fromBase64(raw_image), "BMP");
        //     img->save("SERVER_side_serialized_picture.bmp", "BMP");


     delete img;
}

void UDPserver::change_picture_path()
{
    picture_fileName = QFileDialog::getOpenFileName(nullptr, "select *.bmp picture file",
                                             QDir::currentPath(), "*.bmp");
    if(status != nullptr)
       status->setText(picture_fileName);
}



void UDPserver::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, PAUSE_UDP_SEND*PAUSE_UDP_SEND );
    }
}

} // namespace udpServer
