#ifndef CAMTHREAD_H
#define CAMTHREAD_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>


class CamThread :public QObject
{
    Q_OBJECT
public:
    explicit CamThread(QObject *parent = 0);
signals:
    void reviewComplete();
    void sendPicture(const QImage &img);
public slots:
    void setFlag(bool flag = false);
    void openCamera();
    void closeCamera();
    void startsave();
    void camNumber(const int &n);
    void reviewVideo();
    void closeImshow();
    void mainwindowDisplay();
private slots:

private:
    cv::VideoCapture capture;
    cv::VideoWriter writer;
    cv::Mat src_image;
    bool stopFlag=false;
    int camera_num = 0;
};

#endif // CAMTHREAD_H
