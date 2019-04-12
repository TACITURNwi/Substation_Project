
#include "holder.h"

Holder::Holder()
{
    Lefttimer=new QTimer(this);
    Righttimer=new QTimer(this);
    Uptimer=new QTimer(this);
    Downtimer=new QTimer(this);
    connect(Lefttimer,SIGNAL(timeout()),this,SLOT(Leftslot()));
    connect(Righttimer,SIGNAL(timeout()),this,SLOT(Rightslot()));
    connect(Uptimer,SIGNAL(timeout()),this,SLOT(Upslot()));
    connect(Downtimer,SIGNAL(timeout()),this,SLOT(Downslot()));
}

Holder::~Holder()
{
    StopMove();
    delete Lefttimer;
    delete Righttimer;
    delete Uptimer;
    delete Downtimer;
}

void Holder::StopMove()
{
    qDebug() << "stop";
    QString cmd;
    cmd = "ff010000000001";

    send(Hexarray(cmd));
}

void Holder::LeftMoveAngle(double myLeftspeed,double Leftangle)
{
    if(myLeftspeed<=9){
    int speedTowrite=myLeftspeed*53/9;
    LeftSpinTime=Leftangle*1000/myLeftspeed;
    qDebug()<<LeftSpinTime;
    int sum=5+speedTowrite;

    QString cmd;
    cmd=QString("ff010004%1")
            .arg(speedTowrite,0,16);
    cmd.append(QString("00"));
    QString mysum=QString("%1")
            .arg(sum,0,16);
    cmd.append(mysum);
    qDebug()<<cmd;
    send(Hexarray(cmd));
    Lefttimer->start(LeftSpinTime);
    }
    else
    {
        qDebug()<<"速度设定不在给定范围内,水平转速在0-9度每秒内";
    }
}

void Holder::RightMoveAngle(double myRightspeed, double Rightangle)
{
    if(myRightspeed<=9){
    int speedTowrite=myRightspeed*53/9;
    RightSpinTime=Rightangle*1000/myRightspeed;
    qDebug()<<RightSpinTime;
    int sum=3+speedTowrite;

    QString cmd;
    cmd=QString("ff010002%1")
            .arg(speedTowrite,0,16);
    cmd.append(QString("00"));
    QString mysum=QString("%1")
            .arg(sum,0,16);
    cmd.append(mysum);
    send(Hexarray(cmd));
    Righttimer->start(RightSpinTime);
    }
    else
    {
       qDebug()<<"速度设定不在给定范围内,水平转速在0-9度每秒内";
    }
}

void Holder::DownMoveAngle(double myUpspeed,double Upangle)
{
    if(myUpspeed<=5){
    int speedTowrite=myUpspeed*53/5;
    UpSpinTime=Upangle*1000/myUpspeed;

    int sum=9+speedTowrite;
    QString cmd;
    cmd=QString("ff01000800");
    if(speedTowrite<16)
    {
    QString myspeed=QString("0%1")
            .arg(speedTowrite,0,16);
    cmd.append(myspeed);
    }
    else
    {
        QString myspeed=QString("%1")
                .arg(speedTowrite,0,16);
    cmd.append(myspeed);
    }

    if(sum<16){
    QString mysum=QString("0%1")
            .arg(sum,0,16);
    cmd.append(mysum);
    }
    else
    {
    QString mysum=QString("%1")
                .arg(sum,0,16);
    cmd.append(mysum);
    }

    qDebug()<<cmd;
    send(Hexarray(cmd));
    Uptimer->start(UpSpinTime);
    }
    else
     qDebug()<<"速度设定不在给定范围内,垂直转速在0-5度每秒内";
}

void Holder::UpMoveAngle(double myDownspeed,double Downangle)
{
    if(myDownspeed<=5){
    int speedTowrite=myDownspeed*53/5;
    DownSpinTime=Downangle*1000/myDownspeed;

    int sum=17+speedTowrite;
    QString cmd;
    cmd=QString("ff01001000");
    if(speedTowrite<16){
    QString myspeed=QString("0%1")
            .arg(speedTowrite,0,16);
    cmd.append(myspeed);
    }
    else{
        QString myspeed=QString("%1")
                .arg(speedTowrite,0,16);
     cmd.append(myspeed);
    }

    QString mysum=QString("%1")
            .arg(sum,0,16);
    cmd.append(mysum);
    qDebug()<<cmd;
    send(Hexarray(cmd));
    Downtimer->start(DownSpinTime);
    }
    else
    qDebug()<<"速度设定不在给定范围内,垂直转速在0-5度每秒内";
}
 void Holder::Rightslot()
 {
     Righttimer->stop();
     StopMove();
 }
 void Holder::Leftslot()
{
     Lefttimer->stop();
     StopMove();
 }
 void Holder::Upslot()
{
     Uptimer->stop();
     StopMove();
 }
 void Holder::Downslot()
{
     Downtimer->stop();
     StopMove();
 }

 void Holder::SlotMoveHolder(int rotate_direction, int rotate_speeed, int rotate_angle)
 {
     if(rotate_direction == 1)
     {
         this->UpMoveAngle(rotate_speeed, rotate_angle);
     }
     else if(rotate_direction == 2)
     {
         this->DownMoveAngle(rotate_speeed, rotate_angle);
     }
     else if(rotate_direction == 3)
     {
         this->LeftMoveAngle(rotate_speeed, rotate_angle);
     }
     else if(rotate_direction ==4)
     {
         this->RightMoveAngle(rotate_speeed, rotate_angle);
     }
 }

 void Holder::SlotStopHolder()
 {

     this->StopMove();
 }


