#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../common/protocol.h"
#include "../common/validator.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("LINX Client");

    connect(&m_socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(&m_socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(&m_socket, &QAbstractSocket::errorOccurred, this, &MainWindow::onError);
    connect(&m_socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);

    m_socket.connectToHost("127.0.0.1", 9876);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_btnSend_clicked() {
    bool ok;
    uint32_t userId = ui->leUserId->text().toUInt(&ok);
    if (!ok || userId == 0) {
        QMessageBox::warning(this, "Input Error", "User ID must be a positive integer.");
        return;
    }

    std::string name = ui->leName->text().toStdString();
    std::string email = ui->leEmail->text().toStdString();

    if (name.empty() || email.empty()) {
        QMessageBox::warning(this, "Input Error", "Name and email cannot be empty.");
        return;
    }

    if (!Validator::isValidEmail(email)) {
        QMessageBox::warning(this, "Input Error", "Invalid email format.");
        return;
    }

    UserData data{userId, name, email};
    QByteArray packet = Protocol::serialize(data);
    if (packet.isEmpty()) {
        QMessageBox::critical(this, "Error", "Name or email too long (max 255 chars).");
        return;
    }

    qint64 sent = m_socket.write(packet);
    if (sent != packet.size()) {
        QMessageBox::critical(this, "Network Error", "Failed to send full packet.");
    }
}

void MainWindow::onReadyRead() {
    if (m_socket.readAll() == "OK") {
        ui->statusBar->showMessage("✓ Delivered!", 2000);
    }
}

void MainWindow::onConnected() {
    ui->statusBar->showMessage("Connected to server");
}

void MainWindow::onDisconnected() {
    ui->statusBar->showMessage("Disconnected from server");
}

void MainWindow::onError(QAbstractSocket::SocketError) {
    ui->statusBar->showMessage("Error: " + m_socket.errorString());
}