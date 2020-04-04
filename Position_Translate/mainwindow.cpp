#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Position Translate");
    connect(ui->pushButton  ,SIGNAL(clicked()),this,SLOT(pushbutton1()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushbutton2()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(pushbutton3()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::outof_China(double lon, double lat)
{
    return(lon<72.004 || lon>137.8374 || lat<0.8293 || lat >55.8271 || false);
}
POSITION MainWindow::bd09togcj02(double bd_lon, double bd_lat)
{
    double x = bd_lon - 0.0065;
    double y = bd_lat - 0.006;
    double z = sqrt(x*x + y*y) - 0.00002*sin(y*x_PI);
    double theta = atan2(y,x) - 0.000003*cos(x*x_PI);
    gcj_pos.longitude = z*cos(theta);
    gcj_pos.latitude = z*sin(theta);
    return gcj_pos;
}
POSITION MainWindow::gcj02tobd09(double gcj_lon, double gcj_lat)
{
     double z = sqrt(gcj_lon*gcj_lon + gcj_lat*gcj_lat) + 0.00002*sin(gcj_lat * x_PI);
     double theta = atan2(gcj_lat,gcj_lon) + 0.000003 * cos(gcj_lon * x_PI);
     bd_pos.longitude = z*cos(theta) + 0.0065;
     bd_pos.latitude = z*sin(theta) + 0.006;
     return bd_pos;
}
double MainWindow::translate_lon(double lon, double lat)
{
    double ret = 300.0 + lon +2.0*lat + 0.1*lon*lon +0.1*lon*lat + 0.1*sqrt(abs(lon));
    ret += (20.0 * sin(6.0*lon*PI) + 20.0*sin(2.0*lon*PI)) *2.0 / 3.0;
    ret += (20.0 * sin(lon*PI) + 40.0*sin(lon/3.0 *PI)) *2.0 /3.0;
    ret += (150 * sin(lon/12.0 *PI) + 300.0*sin(lon/30.0 * PI)) *2.0 /3.0;
    return ret;
}
double MainWindow::translate_lat(double lon, double lat)
{
    double ret = -100 + 2.0*lon + 3.0*lat + 0.2*lat*lat + 0.1*lon*lat + 0.2*sqrt((abs(lon)));
    ret += (20.0 *sin(6.0*lon*PI) + 20*sin(2.0*lon*PI)) *2.0 /3.0;
    ret += (20.0 *sin(lat*PI) + 40.0*sin(lat/3.0*PI)) *2.0 /3.0;
    ret += (160.0*sin(lat/12.0*PI) + 320.0*sin(lat/30.0 *PI)) *2.0 /3.0;
    return ret;
}
POSITION MainWindow::gcj02towgs84(double gcj_lon, double gcj_lat)
{
    if(outof_China(gcj_lon,gcj_lat))
    {
        wgs_pos.longitude = gcj_lon;
        wgs_pos.latitude = gcj_lat;
        return wgs_pos;
    }
    else
    {
        double dlat = translate_lat(gcj_lon - 105.0,gcj_lat -35.0);
        double dlon = translate_lon(gcj_lon - 105.0,gcj_lat -35.0);
        double radlat = gcj_lat/180.0 *PI;
        double magic = sin(radlat);
        magic = 1 - ee*magic*magic;
        double squrtmagic = sqrt(magic);
        dlon = (dlon *180.0)/(a/squrtmagic*cos(radlat)*PI);
        dlat = (dlat *180.0)/((a*(1-ee))/(magic * squrtmagic)*PI);
        wgs_pos.longitude = gcj_lon - dlon;
        wgs_pos.latitude = gcj_lat - dlat;
        return wgs_pos;
    }
}
POSITION MainWindow::wgs84togcj02(double wgs_lon, double wgs_lat)
{
    if(outof_China(wgs_lon,wgs_lat))
    {
        gcj_pos.longitude = wgs_lon;
        gcj_pos.latitude = wgs_lat;
        return gcj_pos;
    }
    else
    {
        double dlat = translate_lat(wgs_lon - 105.0,wgs_lat - 35.0);
        double dlon = translate_lon(wgs_lon - 105.0,wgs_lat - 35.0);
        double radlat = wgs_lat/180.0 * PI;
        double magic = sin(radlat);
        magic = 1 - ee*magic*magic;
        double squrtmagic = sqrt(magic);
        dlon = (dlon *180.0)/(a/squrtmagic*cos(radlat)*PI);
        dlat = (dlat *180.0)/((a*(1-ee))/(magic * squrtmagic)*PI);
        gcj_pos.longitude = wgs_lon + dlon;
        gcj_pos.latitude = wgs_lat +dlat;
        return gcj_pos;
    }
}
/*************************************************************************
 *
 * 读取 BD09 坐标，转换为 WGS84 和 GCJ02 坐标
 *
 ***********************************************************************/
void MainWindow::pushbutton1()
{
    double bd09_lon = ui->lineEdit_bd09lon->text().toDouble();
    double bd09_lat = ui->lineEdit_bd09lat->text().toDouble();
    double gcj02_lon = bd09togcj02(bd09_lon,bd09_lat).longitude;
    double gcj02_lat = bd09togcj02(bd09_lon,bd09_lat).latitude;
    double wgs84_lon = gcj02towgs84(gcj02_lon,gcj02_lat).longitude;
    double wgs84_lat =gcj02towgs84(gcj02_lon,gcj02_lat).latitude;
    ui->lineEdit_gcj02lon->setText(QString::number(gcj02_lon,'d',9));
    ui->lineEdit_gcj02lat->setText(QString::number(gcj02_lat,'d',9));
    ui->lineEdit_wgs84lon->setText(QString::number(wgs84_lon,'d',9));
    ui->lineEdit_wgs84lat->setText(QString::number(wgs84_lat,'d',9));
}
/*************************************************************************
 *
 * 读取 GCJ02 坐标，转换为 WGS84 和 BD09 坐标
 *
 ***********************************************************************/
void MainWindow::pushbutton2()
{
    double gcj02_lon = ui->lineEdit_gcj02lon->text().toDouble();
    double gcj02_lat = ui->lineEdit_gcj02lat->text().toDouble();
    double bd09_lon = gcj02tobd09(gcj02_lon,gcj02_lat).longitude;
    double bd09_lat = gcj02tobd09(gcj02_lon,gcj02_lat).latitude;
    double wgs84_lon = gcj02towgs84(gcj02_lon,gcj02_lat).longitude;
    double wgs84_lat = gcj02towgs84(gcj02_lon,gcj02_lat).latitude;
    ui->lineEdit_bd09lon->setText(QString::number(bd09_lon,'d',9));
    ui->lineEdit_bd09lat->setText(QString::number(bd09_lat,'d',9));
    ui->lineEdit_wgs84lon->setText(QString::number(wgs84_lon,'d',9));
    ui->lineEdit_wgs84lat->setText(QString::number(wgs84_lat,'d',9));
}
/*************************************************************************
 *
 * 读取 WGS84 坐标，转换为 GCJ02 和 BD09 坐标
 *
 ***********************************************************************/
void MainWindow::pushbutton3()
{
    double wgs84_lon = ui->lineEdit_wgs84lon->text().toDouble();
    double wgs84_lat = ui->lineEdit_wgs84lat->text().toDouble();
    double gcj02_lon = wgs84togcj02(wgs84_lon,wgs84_lat).longitude;
    double gcj02_lat = wgs84togcj02(wgs84_lon,wgs84_lat).latitude;
    double bd09_lon = gcj02tobd09(gcj02_lon,gcj02_lat).longitude;
    double bd09_lat = gcj02tobd09(gcj02_lon,gcj02_lat).latitude;
    ui->lineEdit_bd09lon->setText(QString::number(bd09_lon,'d',9));
    ui->lineEdit_bd09lat->setText(QString::number(bd09_lat,'d',9));
    ui->lineEdit_gcj02lon->setText(QString::number(gcj02_lon,'d',9));
    ui->lineEdit_gcj02lat->setText(QString::number(gcj02_lat,'d',9));
}
