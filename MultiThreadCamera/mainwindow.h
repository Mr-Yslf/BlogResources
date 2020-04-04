#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <QTimer>
#include <QDebug>
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_opencamera_clicked();

    void on_pushButton_closecamera_clicked();

    void on_pushButton_savevideo_clicked();

    void on_pushButton_savecomplete_clicked();

    void on_pushButton_videoreview_clicked();

    void display_frame();

private:
    Ui::MainWindow *ui;
    bool complete_flag = false;
    VideoCapture myCapture;
    VideoWriter writer;
    Mat picture;
    QTimer fps_timer;
};

#endif // MAINWINDOW_H
