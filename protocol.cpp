#include "protocol.h"
#include <qdebug.h>



Protocol::Protocol()
{

}



QByteArray Protocol::ConvertLaserData(QByteArray distance_data)
{
    QByteArray data_to_send;

    QByteArray data_to_check;

    data_to_send[0] = 0xff;
    data_to_send[1] = 0x05;
    data_to_send[2] = 0x01;
    data_to_send[3] = 0x01;
    data_to_send[4] = 0x03;
    data_to_send += distance_data;

    data_to_check = data_to_send.mid(2,5);

    QByteArray crc_number = CrcOfData(data_to_check, 5);

    data_to_send = data_to_send + crc_number;

    return data_to_send;

}

QByteArray Protocol::ConvertGyroscopeData(QByteArray angle_data)
{
    QByteArray data_to_send;

    QByteArray data_to_check;

    qint16 deviation_angle;
    qint16 pitch_angle;
    qint8 pitch_angle_8;


    data_to_send[0] = 0xff;
    data_to_send[1] = 0x06;
    data_to_send[2] = 0x01;
    data_to_send[3] = 0x00;
    data_to_send[4] = 0x04;
    deviation_angle = (qint16)((double)(angle_data.mid(7,1)+angle_data.mid(6,1)).toHex().toInt(0,16)/32768*180);
    pitch_angle = (qint16)((double)(angle_data.mid(5,1)+angle_data.mid(4,1)).toHex().toInt(0,16)/32768*180);



    if(deviation_angle > 180)
    {
        deviation_angle = -(360-deviation_angle);
    }

    if(pitch_angle > 90)
    {
        pitch_angle_8 = -(qint8)(360-pitch_angle);
        //qDebug() << "pitch_angle" << pitch_angle_8;
    }
    else
    {
        pitch_angle_8 = (qint8)(pitch_angle);
    }

    char deviation_angle_low = (char)(deviation_angle & 0x00FF);          //低八位
    char deviation_angle_high = (char)((deviation_angle >> 8) & 0xFF);    //高八位

    qDebug() << "pitch_angle:" << pitch_angle_8;
    qDebug() << "deviation_angle:" << deviation_angle;
    data_to_send[5] = deviation_angle_high;
    data_to_send[6] = deviation_angle_low;
    data_to_send[7] = pitch_angle_8;


    data_to_check = data_to_send.mid(2, 6);

    QByteArray crc_number = CrcOfData(data_to_check, 6);

    data_to_send = data_to_send + crc_number;

    return data_to_send;
}



QByteArray Protocol::LaserResponse(int project_to_reply, int judge)
{
    QByteArray data_to_send;

    QByteArray data_to_check;

    data_to_send[0] = 0xff;
    data_to_send[1] = 0x05;
    data_to_send[2] = 0x01;
    data_to_send[3] = 0x01;
    data_to_send[4] = 0x02;
    data_to_send[5] = project_to_reply;

    //加判断：是否应答成功
    if( judge == SUCCESS)
    {
        data_to_send[6] = 0x01;
    }
    //加判断：是否应答成功
    if( judge == FAILURE)
    {
        data_to_send[6] = 0x00;
    }

    data_to_check = data_to_send.mid(2,5);

    QByteArray crc_number = CrcOfData(data_to_check, 5);

    data_to_send = data_to_send + crc_number;

    return data_to_send;

}

QByteArray Protocol::HolderResponse(int project_to_reply, int judge)
{

    QByteArray data_to_send;

    QByteArray data_to_check;

    data_to_send[0] = 0xff;
    data_to_send[1] = 0x05;
    data_to_send[2] = 0x01;
    data_to_send[3] = 0x00;
    data_to_send[4] = 0x02;
    data_to_send[5] = project_to_reply;

    if( judge == SUCCESS)
    {
        data_to_send[6] = 0x01;

    }
    //加判断：是否应答成功
    else if( judge == FAILURE)
    {
        data_to_send[6] = 0x00;

    }

    data_to_check = data_to_send.mid(2,5);

    QByteArray crc_number = CrcOfData(data_to_check, 5);

    data_to_send = data_to_send + crc_number;

    return data_to_send;
}

qint32 Protocol::CmdLengthComparison(QByteArray data)
{

    //if(data.indexOf("ff") != -1)
    if(data.indexOf("\xff") != -1)
    {

        QByteArray data_to_check_crc;
        data = data.right(data.length() - data.indexOf("\xff") - 1);
        data_to_check_crc = data.right(data.length() - 1);
        data_to_check_crc = data_to_check_crc.left(data_to_check_crc.length() - 2);

        if(data.left(1) == "\x03" && data.mid(1,1) == "\x01")
        {

            QByteArray crc_number_short = CrcOfData(data_to_check_crc, 3);
            if(crc_number_short == data.right(2))
            {
                qDebug() << "short_cmd check success"  << endl;
                return SHORT_COMMON_CMD;
            }
            else
            {
                qDebug() << "short_cmd check fail" << endl;
                return COMPARISON_ERROR;
                //return SHORT_COMMON_CMD;
            }

        }
        else if(data.left(1) == "\x06" && data.mid(1,1) == "\x01")
        {
            QString crc_number_long = CrcOfData(data_to_check_crc, 6);
            if(crc_number_long == data.right(2))
            {
                qDebug() << "long_cmd check success" << endl;
                return LONG_ROTATE_CMD;
            }
            else
            {
                qDebug() << "long_cmd check fail" << endl;
                return COMPARISON_ERROR;
                //return LONG_ROTATE_CMD;
            }

        }
        else
        {
            return COMPARISON_ERROR;
        }
    }
    else
    {
        return COMPARISON_ERROR;
    }
}

qint32 Protocol::Convert2Cmd(QByteArray b_data)
{
    QString s_data = b_data.toHex();

    if(s_data.mid(6, 2) == "00")               //云台
        {
            if(s_data.mid(8, 2) == "00")
            {
                return HOLDER_START_RECEIVE_CMD;
            }
            else if(s_data.mid(8, 2) == "01")
            {
                return HOLDER_STOP_RECEIVE_CMD;
            }
            else if(s_data.mid(8, 2) == "ee")
            {
                return HOLDER_SELF_CHECK;
            }
            else
            {
                return DATA_ERROR;
            }
        }
        else if(s_data.mid(6, 2) == "01")          //激光
        {
            //
            if(s_data.mid(8, 2) == "00")
            {
                return LASER_START_SEND_DATA;
            }
            //
            else if(s_data.mid(8, 2) == "01")
            {
                return LASER_STOP_SEND_DATA;
            }
            else if(s_data.mid(8, 2) == "ee")
            {
                return LASER_SELF_CHECK;
            }
            else
            {
                return DATA_ERROR;
            }
        }
        else
        {
            return DATA_ERROR;
        }
}



qint32 Protocol::GetRotateDirection(QByteArray b_data)
{
    QString s_data = b_data.toHex();

    if(s_data.mid(4,2) == "01" && s_data.mid(6,2) == "00" && s_data.mid(8,2) == "03")
    {
        qint32 rotate_direction;
        rotate_direction =  s_data.mid(10,2).toInt();
        qDebug() << "rotate_direction:" << rotate_direction << endl;
        return rotate_direction;
    }
    else
    {
        return 0;
    }

}

qint32 Protocol::GetRotateSpeed(QByteArray b_data)
{
    QString s_data = b_data.toHex();

    if(s_data.mid(4,2) == "01" && s_data.mid(6,2) == "00" && s_data.mid(8,2) == "03")
    {

        qint32 rotate_speed;
        rotate_speed =  s_data.mid(12,2).toInt();
        return rotate_speed;
    }
    else
    {
        return 0;
    }

}

qint32 Protocol::GetRotateAngle(QByteArray b_data)
{
    QString s_data = b_data.toHex();

    if(s_data.mid(4,2) == "01" && s_data.mid(6,2) == "00" && s_data.mid(8,2) == "03")
    {

        qint32 rotate_angle;
        rotate_angle =  s_data.mid(14,2).toInt();
        return rotate_angle;
    }
    else
    {
        return 0;
    }
}

QByteArray Protocol::CrcOfData(QByteArray data, int data_length)
{
    //data = QByteArray::fromHex(data);

    unsigned char* buf = (unsigned char *)data.data();

    int i=0, j=0;
    QByteArray crc_ret ;

    quint16 crc_reg = 0xFFFF;
    quint16 check =0;


    for(i = 0; i < data_length ; i++)
    {
        crc_reg = (crc_reg >> 8) ^ buf[i];
        for(j = 0; j < 8; j++)
        {
            check = crc_reg & 0x0001;
            crc_reg >>= 1;
            if(check == 0x0001)
            {
                crc_reg ^= 0xA001;
            }
        }
    }


    //crc_ret = QString::number((int)crc_reg, 16);

    unsigned char crc_low = (unsigned char)(crc_reg & 0x00FF);          //低八位
    unsigned char crc_high = (unsigned char)((crc_reg >> 8) & 0xFF);    //高八位


    //crc_ret = QByteArray::number(crc_high, 16) + QByteArray::number(crc_low, 16);
    QString test = QString::number(crc_high, 16) + QString::number(crc_low ,16);
    qDebug() << test;
    crc_ret[0] = crc_high;
    crc_ret[1] = crc_low;


    return crc_ret;
}

