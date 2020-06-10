#include "bridge.h"

bridge::bridge(QObject *parent) : QObject(parent)
{

}
void bridge::RcvPoint(const QString &lng, const QString &lat)
{
    qDebug()<<lng<<","<<lat;
    emit DisplayPoint(lng,lat);
}
