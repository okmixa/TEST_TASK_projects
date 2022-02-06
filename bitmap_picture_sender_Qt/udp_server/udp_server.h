#pragma once

#include <iostream>

#include <QMainWindow>
#include <QWidget>
#include <QtNetwork/QUdpSocket>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLabel>

namespace udpServer {


class UDPserver : public QObject
{
    Q_OBJECT

private:
    void loadPicture( );

    QUdpSocket *receiver_;
    QLabel* status;

public:

    UDPserver(QObject *parent = 0);

    void set_statusLabel(QLabel* status_label);

private:
    void delay( int millisecondsToWait );

    QString picture_fileName;


signals:
public slots:
    void change_picture_path();
    void serverReceive();
    void serverFeedBack();
};

} // namespace udpServer
