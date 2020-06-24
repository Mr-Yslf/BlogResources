#include "widget.h"
#include <QApplication>
#include "client.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Client wr;
    wr.show();
    w.show();

    return a.exec();
}
