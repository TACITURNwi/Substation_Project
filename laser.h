 #ifndef LASER_H
#define LASER_H
#include "serialport.h"
#include "usart_ctrl_api.h"



class Laser : public SerialPort
{
    Q_OBJECT
public:
    Laser();
    ~Laser();
    void SingleMeasure();
    void AutomaticContinuousMeasure();
    void StopMeasure();
    //bool JudgeDataValid(QByteArray);


private slots:
    void SlotLaserMeasure();




};

#endif // LASER_H
