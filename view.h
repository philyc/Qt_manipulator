#ifndef VIEW_H
#define VIEW_H
#pragma once

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QDateTime>

#include "controller.h"
#include "model.h"
#include "qcustomplot.h"
#include "qsqlite.h"




QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE



class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);

    void widgetshow(QCustomPlot *widget,int recIndex);

    ~View();

public:
    Controller* m_receiver;//receive线程对应的QObject
    Controller* m_inquirer;//inquire线程对应的QObject
    Controller* m_controller;//其他引用函数

    QSqlite* m_sqlite;//sqlite相关

    Model* m_model;

    QThread *receiveThread;
    QThread *inquireThread;



    //绘图使用
    double timecount=0;//计时
    int refreshTimer;//刷新图像的定时器
    int sampleTimer;//模拟采样的定时器
    QPointF newPoint[6];
    QPointF lastPoint[6];//两点绘图，减少数据量
    void initChart();
    void getNow();//更新timecount,现为每30ms更新一次
    void timerEvent(QTimerEvent *event);//绘图主函数


signals:

    void open();
    void close();
    void send(QString Id,QString Data);
    void enable();
    void moterRun(bool isForward,UINT index,QString angle);

    void initDB(QSqlDatabase db);
    void closeDB(QSqlDatabase db);
//    void openDB(QSqlDatabase db);
    void excInsertSql(robotData recData,QString m_tablename,QSqlDatabase db);


public slots:


private slots:
    void on_btnOpen_clicked();

    void on_btnClose_clicked();

    void on_btnSend_clicked();

    void on_BtnEnable_clicked();

    void updateTest(QString receiveId,QString receiveData);//显示接收到的ID和Data

    void updateAbsAngle(vector<double> absAngle);//更新绝对值编码器反馈的角度

    void updateIncNum(vector<long> incNum);//更新增量式编码器反馈的数值

    void on_btnMoter1RunFor_clicked();

    void on_btnMoter1RunRev_clicked();

    void on_btnMoter2RunFor_clicked();

    void on_btnMoter2RunRev_clicked();

    void on_btnMoter3RunFor_clicked();

    void on_btnMoter3RunRev_clicked();

    void on_btnMoter4RunFor_clicked();

    void on_btnMoter4RunRev_clicked();

    void on_btnMoter5RunFor_clicked();

    void on_btnMoter5RunRev_clicked();

    void on_btnMoter6RunFor_clicked();

    void on_btnMoter6RunRev_clicked();

    void getDBpara(QSqlDatabase db);

    void insertSql(robotData recData,QString m_tablename);

private:
    Ui::View *ui;

};
#endif // VIEW_H
