#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebChannel>
#include "bridge.h"
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
    void DisplaySlot(QString lng,QString lat);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bridge *JSBridge;
};

#endif // MAINWINDOW_H
