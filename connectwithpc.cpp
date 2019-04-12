#include "connectwithpc.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>



//构造函数
ConnectWithPC::ConnectWithPC(QObject *parent) : QObject(parent)
{
    InitServer(); //初始化服务端

}

//析构函数
ConnectWithPC::~ConnectWithPC()
{
    if(server!=NULL)
    {
        delete server; //将new出来的server给删除掉
    }
}



//初始化服务端的函数
void ConnectWithPC::InitServer()
{
    server=new QTcpServer(this);
    socket=NULL;       //默认还没有客户端连接上
    listenflags=false; //默认没有开启监听
    QObject::connect(server,&QTcpServer::newConnection,this,&ConnectWithPC::SlotHandleNewConnect); //将接收新连接请求信号和槽函数连接起来
    my_thread = new QThread();
    this->moveToThread(my_thread);
    my_thread->start();
    return;
}


//开始监听的函数
bool ConnectWithPC::StartListen(QHostAddress qhostaddress,int port)
{
    if(listenflags==false) //监听还没有开启
    {   //开启监听

        if(!server->listen(QHostAddress(qhostaddress),port)) //开始监听指定的地址和端口
        {    //开启监听失败
            qDebug()<<server->errorString(); //输出错误信息
            return false;
        }
        else
        {  //开启监听成功
            qDebug()<<"Open Listen success!";
            listenflags=true; //监听标志
            return true;
        }
    }
    else  //监听已经开启
    {
        qDebug()<<"error: Listen is opened before!";
    }
    return true;
}


//给客户端发送数据的函数
void ConnectWithPC::SendData(const QByteArray bMsg)
{
    if(socket!=NULL) //客户端不为空
    {
       if(socket->isOpen())  //客户端网络会话开启着
       {
           //QString sMsg="server send client:"+msg;
           //QString sMsg = msg;
           //QByteArray bMsg=sMsg.toLocal8Bit();  //转换编码，编码需一致
           socket->write(bMsg); //将数据给到客户端
       }
    }
    else
    {
        qDebug()<<"error: There is not a client connected! Please try later!";
    }
}


//停止监听的函数
bool ConnectWithPC::StopListen()
{
    if(server->isListening()) //正在监听
    {
        server->close(); //关闭监听
        listenflags=false;
        return true;
    }
    else
    {
        qDebug()<<"error: no listen to close!";
        return false;
    }
}



//处理新连接请求槽函数
void ConnectWithPC::SlotHandleNewConnect()
{
    QTcpSocket *qtcpsocket=server->nextPendingConnection(); //下一个等待连接
    QString ipstr=qtcpsocket->peerAddress().toString(); //获取等待连接的地址

    QString str="等待连接的下位机的地址是:";
    qDebug()<<str+ipstr;  //将地址打印出来，调试用

    if(socket==NULL)  //还没有客户端连接上
    {
        socket=qtcpsocket; //将客户端指向刚连接的客户端

        QObject::connect(socket,&QTcpSocket::disconnected,this,&ConnectWithPC::SlotHandleDisconnect); //将已连接上的客户端的断开连接消息和本server处理槽函数连接起来
        QObject::connect(socket,&QTcpSocket::readyRead,this,&ConnectWithPC::SlotHandleReceiveData); //将客户端消息和接收槽函数连接起来
    }
    else
    {   //已经用客户端（上位机）连接上了
        qDebug()<<"error: It has a connection before! Can't connect!";
    }
    return;
}


 //处理接收数据的槽函数
void ConnectWithPC::SlotHandleReceiveData()
{
    QTcpSocket *qtcpsocket=dynamic_cast<QTcpSocket*>(sender());
    if( (socket==qtcpsocket) && (socket!=NULL) ) //正好是对应的上位机
    {
        //QString ipStr=qtcpsocket->peerAddress().toString();  //获取到对应的地址
        while(qtcpsocket->bytesAvailable())
        {
            QByteArray data = qtcpsocket->readAll();
            //data = data.toHex();
            qDebug() << "TcpThread" << QThread::currentThread();
            qDebug() << data;
            emit SignalTcpReadDataReady(data);
            /*
            QByteArray temp=qtcpsocket->readAll();  //读取数据
            QString msg="收到"+ipStr+"发来数据:"+QString::fromLocal8Bit(temp);
            qDebug()<<msg;  //将消息打印出来
            */
        }
    }
}

void ConnectWithPC::SlotTcpStartListen()
{
    QHostAddress ip_adress;
    QFile file(IPADDRESSPATH);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Can't open the file!" << endl;
    }
    QTextStream stream(&file);
    ip_adress = stream.readAll();
    //QHostAddress qhostaddress=QHostAddress("192.168.31.108");
    //qDebug() << "SlotTcpStartListen" ;
    this->StartListen(ip_adress,8080);
}

void ConnectWithPC::SlotTcpSendData(QByteArray data)
{
    this->SendData(data);
}



//处理连接断开消息槽函数
void ConnectWithPC::SlotHandleDisconnect()
{
    QTcpSocket *qtcpsocket=dynamic_cast<QTcpSocket*>(sender());
    if( (socket==qtcpsocket) && (socket!=NULL) ) //正好是对应的上位机
    {
        QString ipstr=qtcpsocket->peerAddress().toString();
        socket=NULL;   //将上位机标志重新指向为空
        QString str="error:上位机已断开连接！";
        qDebug()<<str;
    }
    qtcpsocket->deleteLater(); //删掉
}
































