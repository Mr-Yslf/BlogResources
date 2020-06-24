#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QList>
#include <QNetworkInterface>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();

}
Widget::~Widget()
{
    delete ui;
}
void Widget::init()
{
    TcpServer = new QTcpServer(this);
    TcpSocket = new QTcpSocket(this);
    newListen();
    connect(TcpServer,SIGNAL(newConnection()),this,SLOT(mconnect()));
    connect(TcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
                this,SLOT(displayError(QAbstractSocket::SocketError)));

}
void Widget::newListen()
{
    if(!TcpServer->listen(QHostAddress::Any,8000))
    {
        qDebug()<<TcpServer->errorString();
        close();
        return;
    }
}
void Widget::on_pushButton_send_clicked()
{
    QString str = ui->textEdit_write->toPlainText();
    TcpSocket->write(str.toUtf8().data());
}

void Widget::on_pushButton_close_clicked()
{
    TcpSocket->disconnectFromHost();
    TcpSocket->close();
}
void Widget::mconnect()
{
    qDebug()<<"connect";
    TcpSocket = TcpServer->nextPendingConnection();
    connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(RcvData()));
    QString ip = TcpSocket->peerAddress().toString();
    quint16 port = TcpSocket->peerPort();
    ui->textEdit_read->setText(QString::fromLocal8Bit("[%1:%2]:成功连接").arg(ip).arg(port));
}
void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<TcpSocket->errorString();
        TcpSocket->close();
}
void Widget::RcvData()
{
    QString msg = TcpSocket->readAll();
    ui->textEdit_read->setText(msg);
}
