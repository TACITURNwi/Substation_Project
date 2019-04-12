#ifndef MANAGER_H
#define MANAGER_H

#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QObject>
#include <QThread>
#include <QTimer>
#include "serialport.h"
#include "connectwithpc.h"
#include "protocol.h"
#include "holder.h"
#include "laser.h"
#include "manager.h"
#include "gyroscope.h"
#include "usart_ctrl_api.h"



class manager : public QObject
{
    Q_OBJECT
public:
    explicit manager(QObject *parent = 0);
    ~manager();
    void start();
    void start2Listen();
private:

    Protocol *my_protocol;
    QTimer *laser_timer;
    ConnectWithPC *my_connector;
    Holder *my_holder;
    Laser *my_laser;
    Gyroscope *my_gyroscope;
    int gyroscope_times;




signals:
    //打开串口信号
    void SignalOpenLaserSerialport(QString serial_port_number, int baudrate);
    void SignalOpenHolderSerialport(QString serial_port_number, int baudrate);
    void SignalOpenGyroscopeSerialport(QString serial_port_number, int baudrate);
    //关闭串口信号
    void SignalCloseLaserSerialport();
    void SignalCloseHolderSerialport();
    void SignalCloseGyroscopeSerialport();
    //开启激光测量信号
    void SignalLaserMeasure();

    void SignalHolderStop();
    void SignalMoveHolder(int rotate_direction, int rotate_speeed, int rotate_angle);
    void SignalTcpStartListen();
    void SignalTcpTransmit(QByteArray);
    void SignalGyroscopeAngleInit();



private slots:
    void SlotHandleLaserResults(QByteArray);
    void SlotHandleGyroscopeResults(QByteArray);
    void SlotHandleTcpResults(QByteArray);
    void SlotHandleTimeout();





};

#endif // MANAGER_H
