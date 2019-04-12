//头文件说明：下位机与上位机MFC程序进行无线网络通信的管理类
//下位机端为服务端，上位机MFC程序端为客户端，用上位机程序向下位机请示建立连接

#ifndef CONNECTWITHPC_H
#define CONNECTWITHPC_H

#pragma once

#include <QHostAddress>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include "usart_ctrl_api.h"


class QTcpServer;  //将所用的C/S；类包含进来
class QTcpSocket;


class ConnectWithPC:public QObject
{
    Q_OBJECT
public:
    ConnectWithPC(QObject *parent = 0);
    ~ConnectWithPC();
    void InitServer(); //初始化
    bool StartListen(QHostAddress qhostaddress,int port); //开始监听
    void SendData(const QByteArray bmsg); //给客户端发送数据的函数
    bool StopListen();  //停止监听的函数


signals:
    void SignalTcpReadDataReady(QByteArray tcp_data);


private slots:
    void SlotHandleNewConnect();  //处理新连接请求槽函数
    void SlotHandleDisconnect();  //处理连接断开消息槽函数
    void SlotHandleReceiveData(); //处理接收数据的槽函数
    void SlotTcpStartListen();
    void SlotTcpSendData(QByteArray);


private:
    QThread *my_thread;
    QTcpServer *server;
    QTcpSocket *socket; //用于指向请求连接的客户端
    bool        listenflags;


};

#endif // CONNECTWITHPC_H
