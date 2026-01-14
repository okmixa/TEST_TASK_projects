#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class ClientHandler : public QObject {
    Q_OBJECT
public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* m_socket;
    QByteArray m_recvBuffer;
};

