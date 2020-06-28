#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QTime>
namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
private slots:

    void RcvFile();

    void on_pushButton_connect_clicked();

private:
    Ui::Client *ui;
    QFile file;
    QTcpSocket *tcpSocket;
    QString fileName;
    qint64 fileSize;
    qint64 rcvSize;
    QTime mTime;
    int last_time = 0;
    bool isStart = true;
};

#endif // CLIENT_H
