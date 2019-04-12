#include "mythread.h"
#include <QDebug>
#include <QThread>

MyThread::MyThread(QObject *parent) : QObject(parent)
{

}

MyThread::~MyThread()
{
    my_laser->CloseSerialPort();

}



void MyThread::Trigger()
{

}


