#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <Qdir>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    firstThread = new QThread;
    MyCamThread = new CamThread;
    MyCamThread->moveToThread(firstThread);
    connect(&fps_timer,    SIGNAL(timeout()), MyCamThread, SLOT(mainwindowDisplay()));
    connect(MyCamThread,SIGNAL(sendPicture(QImage)),this,SLOT(recivePicture(QImage)));
    fps_timer.setInterval(50);
    connect(MyCamThread,SIGNAL(reviewComplete()),this,SLOT(reviewVideo_complete()));

    QString save_picture = QCoreApplication::applicationDirPath();
    QDir dir;
    dir.cd(save_picture);
    if(!dir.exists("video"))
    {
        dir.mkdir("video");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete MyCamThread;
}

void MainWindow::on_pushButton_opencamera_clicked()
{
    if(ui->camera_name->currentIndex() >= 0)
    {
        firstThread->start();
        MyCamThread->camNumber(ui->camera_name->currentIndex());
        fps_timer.start();
        MyCamThread->openCamera();
    }
    else
        QMessageBox::information(this,tr("Error"),tr("Have No Camera Device!"),QMessageBox::Ok);
}

void MainWindow::on_pushButton_closecamera_clicked()
{
    fps_timer.stop();
    ui->label_videoViewer->clear();
    MyCamThread->closeCamera();
    firstThread->quit();
    firstThread->wait();
}

void MainWindow::on_pushButton_savevideo_clicked()
{
    MyCamThread->setFlag(false);
    MyCamThread->startsave();
}

void MainWindow::on_pushButton_savecomplete_clicked()
{
    MyCamThread->setFlag(true);
    MyCamThread->closeImshow();
}

void MainWindow::on_pushButton_videoreview_clicked()
{
    firstThread->start();
    MyCamThread->reviewVideo();
}

void MainWindow::display_frame()
{

}

void MainWindow::on_pushButton_searchcamera_clicked()
{
    ui->camera_name->clear();
    camera_list = QCameraInfo::availableCameras();
    for(auto i =0;i<camera_list.size();i++)
    {
        ui->camera_name->addItem(camera_list.at(i).description());
    }
}
void MainWindow::recivePicture(QImage img)
{
    ui->label_videoViewer->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::reviewVideo_complete()
{
    firstThread->quit();
    firstThread->wait();
}
