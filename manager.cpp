#include "manager.h"



manager::manager(QObject *parent) : QObject(parent)
{

    my_protocol = new Protocol();
    laser_timer = new QTimer();
    my_connector = new ConnectWithPC;
    my_holder = new Holder;
    my_laser = new Laser;
    my_gyroscope = new Gyroscope;
    //gpio_open();


}

manager::~manager()
{
    emit SignalHolderStop();
    delete my_protocol;
    delete laser_timer;
    delete my_connector;
    delete my_holder;
    delete my_laser;
    delete my_gyroscope;
    //gpio_close();
}

void manager::start()
{
    connect(my_laser,SIGNAL(datapass_Signal(QByteArray)),this,SLOT(SlotHandleLaserResults(QByteArray)));
    connect(my_gyroscope,SIGNAL(datapass_Signal(QByteArray)),this,SLOT(SlotHandleGyroscopeResults(QByteArray)));
    connect(my_connector, SIGNAL(SignalTcpReadDataReady(QByteArray)), this, SLOT(SlotHandleTcpResults(QByteArray)));
    connect(laser_timer, SIGNAL(timeout()), this, SLOT(SlotHandleTimeout()));

    connect(this, SIGNAL(SignalTcpStartListen()), my_connector, SLOT(SlotTcpStartListen()));
    connect(this, SIGNAL(SignalTcpTransmit(QByteArray)), my_connector, SLOT(SlotTcpSendData(QByteArray)));

    connect(this, SIGNAL(SignalOpenLaserSerialport(QString,int)), my_laser, SLOT(SlotOpenSerialport(QString,int)));
    connect(this, SIGNAL(SignalCloseLaserSerialport()), my_laser, SLOT(SlotCloseSerialport()));
    connect(this, SIGNAL(SignalLaserMeasure()), my_laser, SLOT(SlotLaserMeasure()));

    connect(this, SIGNAL(SignalOpenHolderSerialport(QString,int)), my_holder, SLOT(SlotOpenSerialport(QString,int)));
    connect(this, SIGNAL(SignalCloseHolderSerialport()), my_holder, SLOT(SlotCloseSerialport()));
    connect(this, SIGNAL(SignalMoveHolder(int,int,int)), my_holder, SLOT(SlotMoveHolder(int,int,int)));
    connect(this, SIGNAL(SignalHolderStop()), my_holder, SLOT(SlotStopHolder()));

    connect(this, SIGNAL(SignalOpenGyroscopeSerialport(QString,int)), my_gyroscope, SLOT(SlotOpenSerialport(QString,int)));
    connect(this, SIGNAL(SignalCloseGyroscopeSerialport()), my_gyroscope, SLOT(SlotCloseSerialport()));
    connect(this, SIGNAL(SignalGyroscopeAngleInit()), my_gyroscope, SLOT(SlotGyroscopeAngleInit()));



    emit SignalTcpStartListen();



}

void manager::SlotHandleTcpResults(QByteArray data)
{

    quint32 cmd_length;
    QByteArray ack_data;
    qDebug()<< "tcp_data:" << data<< endl;

    cmd_length = my_protocol->CmdLengthComparison(data);
    if(cmd_length == LONG_ROTATE_CMD)
    {
        qint32 rotate_direction, rotate_speed, rotate_angle;
        rotate_direction = my_protocol->GetRotateDirection(data);
        rotate_speed = my_protocol->GetRotateSpeed(data);
        rotate_angle = my_protocol->GetRotateAngle(data);
        //判断方向
        switch(rotate_direction)
        {
        case 0:
            emit SignalMoveHolder(1, rotate_speed, rotate_angle);
            qDebug() << "holder rotate upward" << endl << "rotate_speed：" << rotate_speed << endl << "rotate_angle" << rotate_angle << endl;
            break;
        case 1:
            emit SignalMoveHolder(2, rotate_speed, rotate_angle);
            qDebug() << "holder rotate downward" << endl << "rotate_speed：" << rotate_speed << endl << "rotate_angle" << rotate_angle << endl;
            break;
        case 2:
            emit SignalMoveHolder(3, rotate_speed, rotate_angle);
            qDebug() << "holder rotate leftward" << endl << "rotate_speed：" << rotate_speed << endl << "rotate_angle" << rotate_angle << endl;
            break;
        case 3:
            emit SignalMoveHolder(4, rotate_speed, rotate_angle);
            qDebug() << "holder rotate rightward" << endl << "rotate_speed：" << rotate_speed << endl << "rotate_angle" << rotate_angle << endl;
            break;
        }
        ack_data = my_protocol->HolderResponse(3, 1);
        emit SignalTcpTransmit(ack_data);
    }
    else if(cmd_length == SHORT_COMMON_CMD)
    {
        quint32 cmd_type = my_protocol->Convert2Cmd(data);

        switch(cmd_type)
        {
        case HOLDER_START_RECEIVE_CMD:
            qDebug() << "cmd:holder start receive cmd" ;
            emit SignalOpenHolderSerialport(HOLDER_SERIALPORT_NUMBER,2400);
            emit SignalOpenGyroscopeSerialport(GYROSCOPE_SERIALPORT_NUMBER, 115200);
            emit SignalGyroscopeAngleInit();

            //加判断：成功与失败!
            ack_data = my_protocol->HolderResponse(0, 1);
            qDebug() << "ack_data:" << ack_data << endl;
            emit SignalTcpTransmit(ack_data);
            break;
        case HOLDER_STOP_RECEIVE_CMD:
            qDebug() << "cmd:holder stop receive cmd" ;
            emit SignalHolderStop();
            emit SignalCloseHolderSerialport();
            emit SignalCloseGyroscopeSerialport();
            ack_data = my_protocol->HolderResponse(1, 1);
            qDebug() << "ack_data:" << ack_data << endl;
            emit SignalTcpTransmit(ack_data);
            break;
        case LASER_START_SEND_DATA:
            qDebug() << "cmd:laser start send data" ;
            emit SignalOpenLaserSerialport(LASER_SERIALPORT_NUMBER, 115200);
            laser_timer->start(500);
            ack_data = my_protocol->LaserResponse(0, 1);
            qDebug() << "ack_data:" << ack_data << endl;
            emit SignalTcpTransmit(ack_data);
            break;
        case LASER_STOP_SEND_DATA:
            qDebug() << "cmd:laser stop send data" ;
            //my_laser->StopMeasure();
            laser_timer->stop();
            //my_laser->CloseSerialPort();
            emit SignalCloseLaserSerialport();
            ack_data = my_protocol->LaserResponse(1, 1);
            qDebug() << "ack_data:" << ack_data << endl;
            emit SignalTcpTransmit(ack_data);
            break;
        case HOLDER_SELF_CHECK:
            ack_data = my_protocol->HolderResponse(0xee, 1);
            qDebug() << "cmd: holder self test" ;
            qDebug() << "ack_data:" << ack_data << endl;
            emit SignalTcpTransmit(ack_data);
            break;
        case LASER_SELF_CHECK:
            ack_data = my_protocol->LaserResponse(0xee, 1);
            qDebug() << "cmd: laser self test" ;
            qDebug() << "ack_data:" << ack_data << endl;
            emit SignalTcpTransmit(ack_data);
            break;
        case DATA_ERROR:
            qDebug() << "data error" ;
            //发送错误！
            break;

        }

    }
    else
    {

    }




}

void manager::SlotHandleTimeout()
{

    emit SignalLaserMeasure();

}



void manager::SlotHandleLaserResults(QByteArray data)
{
    QByteArray data_to_pc;



    data = data.mid(5,2);
    //qDebug() << data <<endl;
    //qDebug() << buf.toHex().mid(10,4) ;
    //qDebug() << buf.toHex().mid(10,4).toInt(0,16)*0.001 <<endl;

    data_to_pc = my_protocol->ConvertLaserData(data);

    //qDebug() << data_to_pc ;

    my_connector->SendData(data_to_pc);
}

void manager::SlotHandleGyroscopeResults(QByteArray data)
{


    if(gyroscope_times>10)
    {
        my_protocol->ConvertGyroscopeData(my_gyroscope->AngleDataParsing(data));
        //my_connector->SendData(my_protocol->ConvertGyroscopeData(my_gyroscope->AngleDataParsing(buf)));
        gyroscope_times = 0;
    }
    gyroscope_times++;
}

