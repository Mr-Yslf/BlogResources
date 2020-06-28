#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFileInfo>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    tcpSocket = NULL;
    setWindowTitle(QString::fromLocal8Bit("服务器端口为: 6666"));
    tcpServer->listen(QHostAddress::Any,6666);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(HaveNewConnection()));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::HaveNewConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    QString ip = tcpSocket->peerAddress().toString();
    quint16 port = tcpSocket->peerPort();
    QString str = QString::fromLocal8Bit("[%1:%2] 成功连接").arg(ip).arg(port);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(RcvData()));
    QMessageBox::information(this,"Info",str,QMessageBox::Ok);
}
void Widget::on_pushButton_choose_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"open","../");
    if(!filePath.isEmpty())
    {
        fileName.clear();
        fileSize = 0;
        sendSize = 0;
        QFileInfo info(filePath);
        fileName = info.fileName();
        fileSize = info.size();
        file.setFileName(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->textEdit->setText(QString::fromLocal8Bit("打开文件 ").append(filePath).append("\n"));
        }
        else
        {
            qDebug()<<"文件打开失败";
        }
    }
    else
    {
        qDebug()<<"文件路径出错";
    }
}

void Widget::on_pushButton_send_clicked()
{
    QString head = tr("%1##%2").arg(fileName).arg(fileSize);
    quint64 len = tcpSocket->write(head.toUtf8().data());
    if(len<=0)
    {
        qDebug()<<"头部信息发送失败 ";
        file.close();
    }
}
void Widget::SendFile()
{
    qint64 len = 0;
    do{
        static quint32 cnt = 0;
        char buf[64*1024] = {0};
        len = 0;
        len = file.read(buf,sizeof(buf));
        len = tcpSocket->write(buf,len);
        sendSize += len;
        qDebug()<<cnt++;
    }while(len > 0);
    if(sendSize == fileSize)
    {
        ui->textEdit->setText(QString::fromLocal8Bit("文件发送完毕 \n"));
        file.close();
        tcpSocket->disconnect();
        tcpSocket->close();
    }
}
void Widget::RcvData()
{
    QByteArray buf = tcpSocket->readAll();
    if(QString(buf) == fileName)
    {
        SendFile();
    }
}
