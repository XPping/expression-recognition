#include "myclient.h"

MyClient::MyClient(QWidget *parent) : QWidget(parent),
    m_tcpSocket(0)
{
    m_tcpSocket = new QTcpSocket(this);
}

void MyClient::connectServer(QString host, int port)
{
    m_tcpSocket->abort();
    m_tcpSocket->connectToHost(host, port);
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData));
}
