#ifndef GYROSCOPE_H
#define GYROSCOPE_H
#include "serialport.h"
#include <qdebug.h>


class Gyroscope : public SerialPort
{
    Q_OBJECT
public:
    Gyroscope();
    QByteArray AngleDataParsing(QByteArray data);
    void Angle_init();

private:

    bool CheckBcc(QByteArray);


private slots:
    void SlotGyroscopeAngleInit();


};

#endif // GYROSCOPE_H
