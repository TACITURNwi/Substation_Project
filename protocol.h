#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QObject>


class Protocol
{
public:
    Protocol();
    QByteArray ConvertLaserData(QByteArray data);
    QByteArray ConvertGyroscopeData(QByteArray angle_data);

    QByteArray LaserResponse(int project_to_reply, int judge);
    QByteArray HolderResponse(int project_to_reply, int judge);

    qint32 Convert2Cmd(QByteArray b_data);
    qint32 CmdLengthComparison(QByteArray data);

    QByteArray CrcOfData(QByteArray data, int data_length);

    qint32 GetRotateDirection(QByteArray data);
    qint32 GetRotateSpeed(QByteArray data);
    qint32 GetRotateAngle(QByteArray data);



};


enum PROJECT_TYPE
{
    DATA_ERROR,
    HOLDER_START_RECEIVE_CMD,
    HOLDER_STOP_RECEIVE_CMD,
    HOLDER_ROTATE_CMD,
    HOLDER_SELF_CHECK,
    LASER_START_SEND_DATA,
    LASER_STOP_SEND_DATA,
    LASER_SELF_CHECK

};

enum CMD_Length_COMPARISON
{
    COMPARISON_ERROR,
    LONG_ROTATE_CMD,
    SHORT_COMMON_CMD

};

enum ACK_SUCCESS_OR_FAILURE
{
    FAILURE,
    SUCCESS
};

#endif // PROTOCOL_H


