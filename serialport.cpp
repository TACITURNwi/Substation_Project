#include "serialport.h"
//#include "readportthread.h"

SerialPort::SerialPort()
{
//    my_serialPort = *(QSerialPort);
//    my_serialPort.moveToThread(&my_thread);
    connect(&my_serialPort,SIGNAL(readyRead()),this,SLOT(readdata_slot()));



}

SerialPort::~SerialPort()
{   

    my_serialPort.close();
    //delete  my_serialPort;
}

void SerialPort::OpenSerialPort(QString portname, int baudrate)
{

    my_serialPort.setPortName(portname);
    my_serialPort.close();
    my_serialPort.open( QIODevice::ReadWrite );
    if(baudrate == 2400)                                         //波特率
    {
        my_serialPort.setBaudRate( QSerialPort::Baud2400);
    }
    else if(baudrate == 115200)
    {
        my_serialPort.setBaudRate( QSerialPort::Baud115200);
    }
    my_serialPort.setDataBits( QSerialPort::Data8 );           //数据字节，8字节
    my_serialPort.setParity( QSerialPort::NoParity );          //校验，无
    my_serialPort.setFlowControl( QSerialPort::NoFlowControl );//数据流控制,无
    my_serialPort.setStopBits( QSerialPort::OneStop );         //一位停止位
    qDebug() << "串口打开";
    this->moveToThread(&my_thread);
    my_serialPort.moveToThread(&my_thread);
    my_thread.start();
    qDebug() << "port " << QThread::currentThread() ;

}

void SerialPort::CloseSerialPort()
{   
        if(my_serialPort.isOpen())
        my_serialPort.close();
        qDebug() << "串口已关闭";
        my_thread.exit();
}

void SerialPort::send(QByteArray data)
{

  my_serialPort.write(data);

}


QByteArray SerialPort::Hexarray(QString array)
{
    QByteArray  senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = array.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=array[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = array[i].toLatin1();

        if((hstr >= '0') && (hstr <= '9'))
                hstr=hstr-0x30;
            else if((hstr >= 'A') && (hstr <= 'F'))
                hstr=hstr-'A'+10;
            else if((hstr >= 'a') && (hstr <= 'f'))
                hstr=hstr-'a'+10;
    //        else return (-1);
            else hstr=hstr-hstr;//不在0-f范围内的会发送成0
        hexdata = hstr;

        if((lstr >= '0') && (lstr <= '9'))
                lstr=(lstr-0x30);
            else if((lstr >= 'A') && (lstr <= 'F'))
                lstr=lstr-'A'+10;
            else if((lstr >= 'a') && (lstr <= 'f'))
                lstr=lstr-'a'+10;
    //        else return (-1);
            else lstr=lstr-lstr;//不在0-f范围内的会发送成0
        lowhexdata =lstr;
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

void SerialPort::readdata_slot()
{
    QByteArray passer=my_serialPort.readAll();
    //qDebug() << "readall" << QThread::currentThread();
    emit datapass_Signal(passer);
}

void SerialPort::SlotOpenSerialport(QString port_number, int baudrate)
{

    this->OpenSerialPort(port_number, baudrate);

}

void SerialPort::SlotCloseSerialport()
{
    this->CloseSerialPort();
}

