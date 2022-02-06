#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "udp_server.h"

using namespace udpServer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UDPserver server;

    QMainWindow win;
    win.setWindowTitle("Server UPD");
    win.resize(400, 200);

    QLayout *layout = new QVBoxLayout;
    QWidget* centraWidget = new QWidget();
    layout->addWidget(centraWidget);
    centraWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    win.setCentralWidget(centraWidget);
    auto selectFileButton = new QPushButton("choose *.BMP picture file...", &win);
    layout->addWidget(selectFileButton);
    selectFileButton->setGeometry(40, 20, 180, 50);
    selectFileButton->connect(selectFileButton, &QPushButton::clicked, &server, &UDPserver::change_picture_path );
    QLabel* status_label = new QLabel("Choose bitmap picture and launch START on client", centraWidget);
    layout->addWidget(status_label);
    status_label->setGeometry(40, 340, 200, 20);
    status_label->show();
    server.set_statusLabel(status_label);

     win.show();


     return a.exec();
}
