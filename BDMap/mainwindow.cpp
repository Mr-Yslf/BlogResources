#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    JSBridge = new bridge(this);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("window",(QObject*)JSBridge);
    ui->MapWidget->page()->setWebChannel(channel);
    ui->MapWidget->page()->load(QUrl("qrc:/Baidu_JS/BDMap.html"));
    connect(JSBridge,SIGNAL(DisplayPoint(QString,QString)),this,SLOT(DisplaySlot(QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::DisplaySlot(QString lng, QString lat)
{
    ui->lineEdit_RcvMsg->setText(lng+","+lat);
}

void MainWindow::on_pushButton_clicked()
{
    QString context = ui->lineEdit_SendMsg->text();
    if(!context.contains(','))
    {
        qDebug()<<"输入格式错误";        //输入格式 经度+纬度，中间以英文逗号‘,’隔开
        return;
    }
    QString lng = context.split(',').at(0);
    QString lat = context.split(',').at(1);

    ui->MapWidget->page()->runJavaScript(QString("SetPoint(%1,%2)").arg(lng).arg(lat));
}
