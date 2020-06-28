#include "client.h"
#include "ui_client.h"
#include <QMessageBox>
#include <QFileDialog>


#include <QDebug>
Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(RcvFile()));
}

Client::~Client()
{
    delete ui;
}
void Client::RcvFile()
{
    QByteArray buf = tcpSocket->readAll();
    if(isStart)
    {
        isStart = false;
        QString dir = QFileDialog::getExistingDirectory(this,"选择文件夹 ","./"); 
        fileName = dir.append("/").append(QString(buf).section("##",0,0));
        fileSize = QString(buf).section("##",1,1).toInt();
        QString info = tr("文件名称：").append(QString(buf).section("##",0,0)).append("\n")
                  .append("保存路径：").append(dir).append("   \n")
                  .append("文件大小：").append(QString::number(fileSize/1024.0/1024.0,'f',2)).append("Mb");
        rcvSize = 0;
        qDebug()<<fileName<<fileSize;
        ui->textEdit_filemsg->setText(info);
        tcpSocket->write(QString(buf).section("##",0,0).toUtf8().data());
        file.setFileName(fileName);
        if(file.open(QIODevice::WriteOnly))
        {

        }
        else
        {
            qDebug()<<"write only error";
        }

        mTime.start();
    }
    else
    {
        qint64 tmp = 0;
        static qint64 cnt = 0;
        cnt++;
        qint64 len = file.write(buf);
        rcvSize += len;
        tmp += len;
        int progress = (int)((rcvSize*1.0)/(fileSize*1.0) * 100);
        if(cnt%10 == 0)
        {
            float speed;
            QTime current_time = QTime::currentTime();
            int msec = current_time.msec();
            if(last_time != 0)
            {
                if(msec<last_time)
                    speed = tmp * 1000.0/((msec + 1000-last_time)*1024.0*1024.0);
                else if(msec == last_time)
                    speed = 100.0;
                else
                    speed = tmp * 1000.0/((msec-last_time)*1024.0*1024.0);
            }
            last_time = msec;
            tmp = 0;
            ui->progressBar->setValue(progress);
            if(cnt%100 == 0)
                ui->label_speed->setText(QString::number(speed,'f',2).append("Mb/s"));

        }
        if(rcvSize == fileSize)
        {
            QString str = ("下载时间：");
            ui->textEdit_filemsg->append(str.append(QString::number(mTime.elapsed()/1000.0,'f',1)).append("s"));
            ui->progressBar->setValue(100);
            ui->label_speed->setText("0.00");
            file.close();
            QMessageBox::information(this,"完成","文件接收完成");
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
            isStart = true;
        }
    }
}

void Client::on_pushButton_connect_clicked()
{
    QString ip = ui->lineEdit_ip->text();
    quint16 port = ui->lineEdit_port->text().toInt();
    tcpSocket->connectToHost(ip,port);
}
