#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QCameraInfo>
#include <QList>
#include "camthread.h"
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

    void on_pushButton_searchcamera_clicked();

    void recivePicture(QImage img);

    void reviewVideo_complete();
private:
    Ui::MainWindow *ui;
    QThread *firstThread;
    CamThread *MyCamThread;
    QList<QCameraInfo> camera_list;
    QTimer fps_timer;
};

#endif // MAINWINDOW_H
