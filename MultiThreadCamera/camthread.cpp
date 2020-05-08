#include "camthread.h"
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QCoreApplication>
CamThread::CamThread(QObject *parent) : QObject(parent)
{
    stopFlag = false;
}
void CamThread::startsave()
{
    QString path = QCoreApplication::applicationDirPath().append("/Video/")
            .append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")).append(".avi");
    cv::String file_path = path.toStdString() ;
    writer.open(file_path,cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), 20.0, cv::Size(640, 480));
    while(!stopFlag)
    {
        capture >> src_image;
        writer.write(src_image);
        cv::namedWindow("video", cv::WINDOW_NORMAL);
        cv::imshow("video", src_image);
        cv::waitKey(50);
    }
}
void CamThread::mainwindowDisplay()
{
    capture >> src_image;
    QImage img1 = QImage((const unsigned char*)src_image.data,
                         src_image.cols, src_image.rows, QImage::Format_RGB888).rgbSwapped();
    emit sendPicture(img1);
}
void CamThread::camNumber(const int &n)
{
    camera_num = n;
}
void CamThread::openCamera()
{
    capture.open(camera_num);
    if(!capture.isOpened())
    {
        return;
    }
}
void CamThread::closeCamera()
{
    if(!stopFlag)   // 如果还在保存视频 则关闭cv窗口
    {
        cv::destroyWindow("video");
    }
    capture.release();
    writer.release();
}
void CamThread::setFlag(bool flag)
{
    stopFlag = flag;
}

void CamThread::closeImshow()
{
    cv::destroyWindow("video");
}
void CamThread::reviewVideo()
{
    cv::VideoCapture video;
    cv::Mat video_src;
    QString path = QFileDialog::getOpenFileName(0,"打开","../","");
    cv::String openpath = path.toStdString();
    video.open(openpath);
    while(video.isOpened())
    {
        video>>video_src;
        if(video_src.empty())
            break;
        cv::imshow("video_review",video_src);
        if(cv::waitKey(50)==27)
        {
            cv::destroyWindow("video_review");
            break;
        }
    }
    emit reviewComplete();
}
