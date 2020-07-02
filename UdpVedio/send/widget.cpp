#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any,8888);
    connect(&fps_timer,SIGNAL(timeout()),this,SLOT(VideoSend()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_open_clicked()
{
    camera.open(0);
    fps_timer.start(33);
}

void Widget::on_pushButton_close_clicked()
{
    fps_timer.stop();
    camera.release();
    this->close();
}
void Widget::VideoSend()
{
    QHostAddress dstip = (QHostAddress)(ui->lineEdit_ip->text());
    quint16 dstport = ui->lineEdit_port->text().toInt();
    Mat frame;
    camera.read(frame);
    cvtColor(frame,frame,CV_BGR2RGB);
    QImage image((unsigned char *)(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
    ui->label_video->setPixmap(QPixmap::fromImage(image));
    ui->label_video->resize(image.width(),image.height());
    QByteArray byte;
    QBuffer buff(&byte);
    buff.open(QIODevice::WriteOnly);
    image.save(&buff,"JPEG");
    QByteArray ss = qCompress(byte,5);
    udpSocket->writeDatagram(ss,dstip,dstport);
}
