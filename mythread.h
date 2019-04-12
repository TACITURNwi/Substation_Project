#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QTimer>
#include "laser.h"
#include "holder.h"
#include "connectwithpc.h"
#include <usart_ctrl_api.h>


class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    ~MyThread();
    void InitLaser();
    //QThread *laser_thread;

private:
    QTimer *my_timer;
    ConnectWithPC *my_connector;
    Holder *my_holder;
    Laser *my_laser;

signals:
    void SignalTcpStartListen();










private slots:
    void Trigger();


};

#endif // MYTHREAD_H
