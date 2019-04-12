#include "gyroscope.h"

Gyroscope::Gyroscope()
{


}

QByteArray Gyroscope::AngleDataParsing(QByteArray data)
{

    //qDebug() << "data" << data;
    //data_to_cut.append(data);
    QByteArray single_data ;

    if(data.indexOf("\x55")!= -1)
    {
         if(data.indexOf("\x53") != -1)
         {
            //single_data = data_to_cut.right(data_to_cut.length()-data_to_cut.indexOf("\x55"));
            single_data = data.right(data.length()-data.indexOf("\x53") + 1);

            //qDebug() << "single_data" << single_data;
            //qDebug() << "\x53" << single_data.indexOf("\x53") ;
            if( single_data.indexOf("\x53") == 1 && single_data.length() >= 11)
            {
                single_data = single_data.left(11);

                //qDebug() << "single_data" << single_data ;

                if(CheckBcc(single_data))
                {

                    //qDebug() << "single_data" << single_data;
                    //qDebug() << "single_data" << single_data.mid(2,8) ;
                    //qDebug() << "x:" << (int)((double)(single_data.mid(3,1)+single_data.mid(2,1)).toHex().toInt(0,16)/32768*180);
                    //qDebug() << "y:" << (int)((double)(single_data.mid(5,1)+single_data.mid(4,1)).toHex().toInt(0,16)/32768*180);
                    //qDebug() << "z:" << (int)((double)(single_data.mid(7,1)+single_data.mid(6,1)).toHex().toInt(0,16)/32768*180);

                    return single_data;
                }

            }
         }

    }

    return 0;
}

void Gyroscope::Angle_init()
{
//    QString cmd;
//    cmd="ffaa52";
//    qDebug() << Hexarray(cmd) <<endl;
//    send(Hexarray(cmd));
    QByteArray test;
    test[0] = 0xff;
    test[1] = 0xaa;
    test[2] = 0x52;
    qDebug() << test ;
    send(test);

}

bool Gyroscope::CheckBcc(QByteArray q_data)
{
    QString s_data = q_data.toHex();

    qint16 i_bcc;
    qint16 i_head = 85;
    qint16 i_type = 83;
    qint16 i_roll_l;
    qint16 i_roll_h;
    qint16 i_pitch_l;
    qint16 i_pitch_h;
    qint16 i_yaw_l;
    qint16 i_yaw_h;
    qint16 i_t_l;
    qint16 i_t_h;
    qint16 i_sum;


    i_roll_l = s_data.mid(4,2).toInt(0,16);

    i_roll_h = s_data.mid(6,2).toInt(0,16);

    i_pitch_l = s_data.mid(8,2).toInt(0,16);

    i_pitch_h = s_data.mid(10,2).toInt(0,16);

    i_yaw_l = s_data.mid(12,2).toInt(0,16);

    i_yaw_h = s_data.mid(14,2).toInt(0,16);

    i_t_l = s_data.mid(16,2).toInt(0,16);

    i_t_h = s_data.mid(18,2).toInt(0,16);

    i_bcc= s_data.right(2).toInt(0,16);


    i_sum=i_head + i_type + i_roll_l + i_roll_h + i_pitch_l + i_pitch_h + i_yaw_l + i_yaw_h + i_t_l + i_t_h;     //加上数据方向和结束码

    while(i_sum>255)
    {
        i_sum=i_sum - 255 -1;
    }
    //校验判断
    if(i_bcc == i_sum)
    {
        qDebug() << "check success" << endl;

        return 1;

    }

    return 0;
}

void Gyroscope::SlotGyroscopeAngleInit()
{
    this->Angle_init();
}

