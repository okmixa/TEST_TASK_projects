#include "udp_client.h"
#include "../udp_server/config.h"

#include <QImage>
#include <QBuffer>
#include <QDir>
#include <QApplication>

namespace udpClient {

UDPclient::UDPclient(QObject *parent)
    : QObject(parent),
      count(0),
      status(nullptr),
      image_recived(false)
{
    sender_ = new QUdpSocket(this);
    sender_->bind(QHostAddress::LocalHost, SEND_PORT);

    connect(sender_,SIGNAL(readyRead()),this,SLOT(clientReceive()));
}

void UDPclient::set_startButton(QPushButton* btn)
{
    start_button = btn;
}

void UDPclient::set_statusLabel(QLabel* status_label)
{
    status = status_label;
}

void  UDPclient::start()
{
    if(start_button != nullptr)
        start_button->setDisabled(true);

    clientSend();
}

bool UDPclient::alreadyRecivedImage()
{
    return image_recived;
}

void UDPclient::clientSend()
{
    QByteArray Data;
    QJsonObject localRecord;

    localRecord.insert("start", QJsonValue{});
    QJsonDocument doc(localRecord);


    qDebug() << "Client is going to send this JSON to server ..." << doc.toJson();
    if(status != nullptr)
        status->setText( doc.toJson().toStdString().c_str() );

    Data.append(doc.toJson());
    sender_->writeDatagram(Data, QHostAddress::LocalHost, RECIVE_PORT);
}

QImage& UDPclient::get_image()
{
    return completed_image;
}

void UDPclient::clientReceive()
{
    //qDebug() << "recive feedback from server";

    QByteArray buffer;

    buffer.resize(sender_->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    sender_->readDatagram(buffer.data(), buffer.size(), &sender , &senderPort);

    //read datagram info from last 12 bytes from the tail
    quint32 pos = *reinterpret_cast<quint32*>(buffer.end() - 3*sizeof(quint32));
    quint32 datagram_size = *reinterpret_cast<quint32*>(buffer.end() - 2*sizeof(quint32));
    quint32 picture_size = *reinterpret_cast<quint32*>(buffer.end() - sizeof(quint32));

    //raw_image.reserve(picture_size);
    raw_image.replace(pos, datagram_size, buffer.data());

    //qDebug() << "pos=" << pos << " datagrams_count=" << datagram_size << " picture_size="  << picture_size << "count:  = " << count;
    //qDebug() << "raw size=" << raw_image.size() << " buffer size=" << buffer.size();

    if(status != nullptr)
        status->setText( "recive datagram from server"  + QString("%1").arg(count) + "  size = " + QString("%1").arg(buffer.size()) );

    count++;

    if( raw_image.size() == picture_size )
    {
       qDebug() << "complete total datagrams accepted = " << count << " raw image size = " << raw_image.size();
       if(status != nullptr)
           status->setText( "complete ! total datagrams accepted " + QString("%1").arg(count) +  " raw image size = "  + QString("%1").arg(raw_image.size()) );
       //qDebug() << "raw image: " << raw_image;


       // decode from base64 format
       completed_image.loadFromData(QByteArray::fromBase64(raw_image), "BMP");

       if(!completed_image.isNull())
        {
           completed_image.save("CHECK_ME_client_recived.bmp", "BMP");

           //bool will checked in while looop in main, true = send image to render
           image_recived = true;
        }
        else
        {
            start_button->setDisabled(false);
            count = 0;
            raw_image.clear();

            qDebug() << "Error: picture transfer failed";
            if(status != nullptr)
                status->setText( "ERROR: picture transfer failed"  );

        }

       //TODO: chech images Q_ASSERT(img1 == img2);

    }

}

} // namespace udpClient

