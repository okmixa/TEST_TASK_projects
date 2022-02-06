
#include <QApplication>
#include <QMainWindow>
#include <QDir>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "udp_client.h"


using namespace udpClient;

int main(int argc, char *argv[])
{
    QApplication clientApp(argc, argv);

    //  test how QOpenGLWidget works
    //QImage *img = new QImage(QDir::currentPath() + "/" + "picture5.bmp");
    //PictureRender w(*img);
    //w.show();

    QMainWindow* window = new QMainWindow();
    window->show();
    window->resize(320, 200);
    window->setWindowTitle("Client UDP");

    QLayout *layout = new QVBoxLayout;
    QWidget* centraWidget = new QWidget();
    centraWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    centraWidget->setLayout(layout);
    window->setCentralWidget(centraWidget);

    QPushButton* startButton = new QPushButton("send START to server",  window);
    layout->addWidget(startButton);
    startButton->setGeometry(40, 40, 180, 60);

    QLabel* status_label = new QLabel("......", window);
    layout->addWidget(status_label);
    status_label->setGeometry(40, 100, 1200, 40);
    status_label->show();

    UDPclient client;
    client.set_startButton( startButton );
    client.set_statusLabel( status_label );
   // clientApp.connect( window, &QMainWindow::destroyed, &clientApp, &QApplication::quit );
    startButton->connect(startButton, &QPushButton::clicked, &client, &UDPclient::start );


    while( !client.alreadyRecivedImage()  )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }


    PictureRender w( client.get_image() );
    w.show();


    return clientApp.exec();
}
