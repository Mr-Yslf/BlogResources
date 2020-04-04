#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fps_timer.setInterval(40);
    connect(&fps_timer,SIGNAL(timeout()),this,SLOT(display_frame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_opencamera_clicked()
{
    myCapture.open(0);
    if(!myCapture.open(0))
    {
        qDebug()<<"Camera Open Failed.";
        return;
    }
    fps_timer.start();
}

void MainWindow::on_pushButton_closecamera_clicked()
{
    fps_timer.stop();
    myCapture.release();
    ui->label_videoViewer->clear();
}

void MainWindow::on_pushButton_savevideo_clicked()
{
    if(!myCapture.isOpened())
    {
        qDebug()<<"Camera Is Not Open.";
        return;
    }
    writer.open("D:\\test.avi",VideoWriter::fourcc('M', 'J', 'P', 'G'),25, Size(640, 480), true);
    while (!complete_flag)
    {
        myCapture >> picture;
        writer.write(picture);
        namedWindow("VideoPlay", WINDOW_NORMAL);
        imshow("VideoPlay", picture);
        waitKey(40);
    }
}

void MainWindow::on_pushButton_savecomplete_clicked()
{
    complete_flag = true;
}

void MainWindow::on_pushButton_videoreview_clicked()
{
    myCapture.open("D:\\test.avi");
    while (myCapture.isOpened())
    {
        myCapture >> picture;
        if(picture.empty())
            break;
        imshow("VideoPlay", picture);
        if (waitKey(40) == 27)  // ESC键的ASCII码为27，如果按下ESC键就推出
            break;
    }
    destroyWindow("VideoPlay");
}

void MainWindow::display_frame()
{
    myCapture >> picture;
    QImage img1 = QImage((const unsigned char*)picture.data, picture.cols, picture.rows, QImage::Format_RGB888).rgbSwapped();
    ui->label_videoViewer->setPixmap(QPixmap::fromImage(img1));
    namedWindow("VideoPlay", WINDOW_NORMAL);
    imshow("VideoPlay", picture);
    waitKey(40);
}
