#ifndef HOLDER
#define HOLDER
#include"serialport.h"
#include "qmath.h"



class Holder : public SerialPort
{
    Q_OBJECT
public:
    Holder();
    ~Holder();
    void LeftMoveAngle(double myLeftspeed, double Leftangle);
    void RightMoveAngle(double myRightspeed,double Rightangle);
    void UpMoveAngle(double myUpspeed,double Upangle);
    void DownMoveAngle(double myDownspeed, double Downangle);
    void StopMove();
private slots:
    void Leftslot();
    void Rightslot();
    void Upslot();
    void Downslot();
    void SlotMoveHolder(int rotate_direction, int rotate_speeed, int rotate_angle);
    void SlotStopHolder();

private:
   int LeftSpinTime;
   int RightSpinTime;
   int UpSpinTime;
   int DownSpinTime;
   QTimer * Lefttimer;
   QTimer * Righttimer;
   QTimer * Uptimer;
   QTimer * Downtimer;
};
#endif // HOLDER

