#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QTcpSocket>
#include <QWidget>

class MyClient : public QWidget
{
    Q_OBJECT
public:
    explicit MyClient(QWidget *parent = nullptr);
    void connectServer(QString host, int port);
signals:

public slots:

private:
    QTcpSocket *m_tcpSocket;
};

#endif // MYCLIENT_H
