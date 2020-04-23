#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include "ControlCAN.h"
#include <QMainWindow>
#include <string>
#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QtMath>
#include <QMessageBox>
#include <synchapi.h>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QThread>
#include <QTimer>
#include <iterator>
#include <float.h>
#include "model.h"
#include "qsqlite.h"



#define StandardFrame 0
#define ExpandFrame 1
#define DataFrame 0
#define RemoteFrame 1
#define CanDataLength 8
#define PI 3.14159265358979323846

#define Link1Length 120
#define Link2Length 264
#define Link3WLength 127
#define Link3HLength 213

using namespace std;


class Controller: public QObject
{
    Q_OBJECT

public:
    Controller();
    int DouHex2Dec(QString s);
    int Hex2Dec(char c);
    void CanSend();

    void MoterRunFor(UINT index,QString angle);
    void MoterRunRev(UINT index,QString angle);

    pointData ForwardKinematic(vector<double> angleData);
    void InverseKinematic(pointData point,bool isLeft);

    void btnMoterRunClick(UINT index,double angle);//用于反解

public:
    //    DWORD devindex;//CAN设备索引
    //    DWORD devtype;//CAN设备类型
    VCI_CAN_OBJ sendbuf[1];//发送帧

    UINT ReceiveId;//receive线程中收到的id
    BYTE ReceiveData[8];//receive线程中收到的data


signals:
    void rec(QString receiveId,QString receiveData);

    void recAbsAngle(vector<double> absAngle);

    void recIncNum(vector<long> incNum);

    void recEndPos(pointData point,bool isLeft);

    void recInverseCal(vector<double> calAngel,bool isLeft);


public slots:
    void btnOpenClick();
    void btnCloseClick();
    void btnSendClick(QString Id,QString Data);
    void btnEnableClick();

    void btnMoterRunClick(bool isForward,UINT index,QString angle);
    void btnMoterStopClick(UINT index);


    void receive();
    void inquire();

private:
    CRITICAL_SECTION send_syn;
    double ArmWeight1=0.5,ArmWeight2=0.3,ArmWeight3=0.2;//反解各关节权重
};

#endif // CONTROLLER_H
