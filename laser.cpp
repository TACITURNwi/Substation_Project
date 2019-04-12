#include<qdebug.h>
#include "laser.h"


Laser::Laser()
{

}

Laser::~Laser()
{

}

void Laser::SingleMeasure()
{
    //gpio_ctrl(unsigned int cmd);
    QString cmd;
    cmd = "0103000F0002F408";
    send(Hexarray(cmd));
    //gpio_ctrl(unsigned int cmd);
}

void Laser::AutomaticContinuousMeasure()
{
    QString cmd;
    cmd = "0110002200010200016112";
    send(Hexarray(cmd));

}

void Laser::StopMeasure()
{
    QString cmd;
    cmd = "011000220001020000A0D2";
    send(Hexarray(cmd));

}

/*
bool Laser::JudgeDataValid(QByteArray data)
{

}
*/

void Laser::SlotLaserMeasure()
{
    SingleMeasure();
}


