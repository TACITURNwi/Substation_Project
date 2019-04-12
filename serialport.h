#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QtSerialPort/QtSerialPort>
#include <QTimer>
#include <QThread>



class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort();
    ~SerialPort();
    void OpenSerialPort(QString portname, int baudrate);
    void CloseSerialPort();
    int respond;

private:
    QThread my_thread;
    QSerialPort my_serialPort;//(实例化一个指向串口的指针，可以用于访问串口)

public slots:
    QByteArray Hexarray(QString array);
    void send(QByteArray data);

private slots:
    void readdata_slot();
    void SlotOpenSerialport(QString port_number, int baudrate);
    void SlotCloseSerialport();

signals:
    QByteArray datapass_Signal(QByteArray data);

};

#endif // SERIALPORT_H
