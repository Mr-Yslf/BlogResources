#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void init();

    void newListen();

    void mconnect();

    void RcvData();

    void on_pushButton_send_clicked();

    void on_pushButton_close_clicked();

    void displayError(QAbstractSocket::SocketError);

private:
    Ui::Widget *ui;
    QTcpServer *TcpServer;
    QTcpSocket *TcpSocket;
};

#endif // WIDGET_H
