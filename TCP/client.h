#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

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

    void displayError(QAbstractSocket::SocketError);

    void on_pushButton_connect_clicked();

    void recivedata();

    void on_pushButton_send_clicked();

private:
    Ui::Client *ui;
    QTcpSocket *socket;
};

#endif // CLIENT_H
