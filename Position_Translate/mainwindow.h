#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <math.h>
typedef struct _POSITION
{
    double longitude;
    double latitude;
}POSITION;
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
    double translate_lon(double lon,double lat);
    double translate_lat(double lon,double lat);
    bool outof_China(double lon,double lat);
    POSITION bd09togcj02(double bd_lon, double bd_lat);
    POSITION gcj02tobd09(double gcj_lon,double gcj_lat);
    POSITION gcj02towgs84(double gcj_lon,double gcj_lat);
    POSITION wgs84togcj02(double wgs_lon,double wgs_lat);
    void pushbutton1();
    void pushbutton2();
    void pushbutton3();
private:
    Ui::MainWindow *ui;
    POSITION bd_pos;
    POSITION gcj_pos;
    POSITION wgs_pos;
    double x_PI = 3.14159265358979323846 * 3000.0 / 180.0;
    double PI = 3.1415926535897932384626;
    double a = 6378245.0;
    double ee = 0.00669342162296594323;
};

#endif // MAINWINDOW_H
