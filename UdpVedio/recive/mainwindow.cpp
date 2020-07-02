#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QHostAddress>
#include<QPixmap>
#include<QImageReader>
#include<QBuffer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    receiver.bind(QHostAddress::Any,6666);
    connect(&receiver,SIGNAL(readyRead()),this,SLOT(video_receive_show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::video_receive_show()
{
    quint64 size = receiver.pendingDatagramSize();    
    QByteArray buff;    
    buff.resize(size);
    QHostAddress adrr ;
    quint16 port;
    receiver.readDatagram(buff.data(),buff.size(),&adrr,&port);    
    buff = qUncompress(buff);
    QBuffer buffer(&buff);
    QImageReader reader(&buffer,"JPEG");//可读入磁盘文件、设备文件中的图像、以及其他图像数据如pixmap和image，相比较更加专业。
    //buffer属于设备文件一类，
    QImage image = reader.read();//read()方法用来读取设备图像，也可读取视频，读取成功返回QImage*，否则返回NULL    
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->resize(image.width(),image.height());
}
