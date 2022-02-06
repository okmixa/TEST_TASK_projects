#pragma once

#include "opengl_widget.h"

#include <QtNetwork/QUdpSocket>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>
#include <QPushButton>
#include <QLabel>


namespace udpClient {

class UDPclient : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *sender_;
    QByteArray raw_image;
    quint32 count;
    bool image_recived;
    QImage completed_image;
    QPushButton* start_button;
    QLabel* status;

    void clientSend();

public:
    UDPclient(QObject *parent = 0);
    QImage& get_image();
    bool alreadyRecivedImage();
    void set_startButton(QPushButton* btn_start);
    void set_statusLabel(QLabel* status_label);

signals:

public slots:
    void clientReceive();
    void start();
};

} // namespace udpClient
