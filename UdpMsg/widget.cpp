#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any,8888);   //这个绑定其实是设置自己的端口号
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(RcvData()));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::RcvData()
{
    char buf[1024] = {0};
    QHostAddress address ;
    quint16 port;
    quint64 len = udpSocket->readDatagram(buf,sizeof(buf),&address,&port);
    if(len>0)
    {
        QString str = QString("[%1:%2]:%3").arg(address.toString()).arg(port).arg(buf);
        ui->textEdit_rcv->append(str);
    }
}
void Widget::on_pushButton_send_clicked()
{
    QHostAddress ip = (QHostAddress)ui->lineEdit_ip->text();
    quint16 port = ui->lineEdit_port->text().toInt();
    QString str =  ui->textEdit_send->toPlainText();
    udpSocket->writeDatagram(str.toUtf8().data(),ip,port);
    ui->textEdit_rcv->append(str);
}

void Widget::on_pushButton_close_clicked()
{
    this->close();
}
