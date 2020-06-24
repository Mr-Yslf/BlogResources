#include "client.h"
#include "ui_client.h"
#include <QHostAddress>
#include <QDebug>
Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(recivedata()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
        this,SLOT(displayError(QAbstractSocket::SocketError)));
}

Client::~Client()
{
    delete ui;
}

void Client::on_pushButton_connect_clicked()
{
    socket->abort();
    QString ip = ui->lineEdit_ip->text();
    quint16 port = ui->lineEdit_port->text().toInt();
    qDebug()<<ip<<port;
    socket->connectToHost(QHostAddress(ip),port);
}
void Client::displayError(QAbstractSocket::SocketError)
{
   qDebug()<<socket->errorString();
   socket->close();
}
void Client::recivedata()
{
    QString datas=socket->readAll();
    ui->textEdit_read->setText(datas);
}

void Client::on_pushButton_send_clicked()
{
    QString txt = ui->textEdit_write->toPlainText();
    socket->write(txt.toUtf8().data());
}
