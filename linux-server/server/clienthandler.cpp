#include "clienthandler.h"
#include "../common/protocol.h"
#include <QtEndian>
#include <QDebug>

#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

ClientHandler::ClientHandler(QTcpSocket* socket, QObject* parent)
    : QObject(parent), m_socket(socket) {

    // Linux performance tuning
    int one = 1;
    setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    int bufSize = 1 << 20; // 1 MB
    setsockopt(m_socket->socketDescriptor(), SOL_SOCKET, SO_RCVBUF, &bufSize, sizeof(bufSize));
    setsockopt(m_socket->socketDescriptor(), SOL_SOCKET, SO_SNDBUF, &bufSize, sizeof(bufSize));

    connect(m_socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}

void ClientHandler::onReadyRead() {
    m_recvBuffer += m_socket->readAll();

    while (m_recvBuffer.size() >= 4) {
        quint32 msgSize = qFromBigEndian<quint32>(
            reinterpret_cast<const uchar*>(m_recvBuffer.constData())
        );
        quint32 totalPacketSize = 4 + msgSize;

        if (static_cast<quint32>(m_recvBuffer.size()) < totalPacketSize) {
            break;
        }

        QByteArray payload = m_recvBuffer.mid(4, msgSize);
        auto userData = Protocol::parse(payload);

        if (userData) {
            qDebug() << "[SERVER] OK:" << userData->userId
                     << QString::fromStdString(userData->name)
                     << QString::fromStdString(userData->email);
            m_socket->write("OK", 2);
        } else {
            qWarning() << "[SERVER] Malformed content";
            m_socket->disconnectFromHost();
            return;
        }

        if (totalPacketSize == static_cast<quint32>(m_recvBuffer.size())) {
            m_recvBuffer.clear();
        } else {
            m_recvBuffer = m_recvBuffer.mid(totalPacketSize);
        }
    }
}

void ClientHandler::onDisconnected() {
    m_socket->deleteLater();
    deleteLater();
}
