#include <QCoreApplication>
#include <QTcpServer>
#include "clienthandler.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QTcpServer server;
    QObject::connect(&server, &QTcpServer::newConnection, [&server]() {
        QTcpSocket* socket = server.nextPendingConnection();
        new ClientHandler(socket);
    });

    if (!server.listen(QHostAddress::Any, 9876)) {
        qCritical() << "Cannot start server on port 9876";
        return -1;
    }

    qDebug() << "Server running on port 9876";
    return app.exec();
}
