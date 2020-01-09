#include "controller.h"
#include "view.h"

Controller::Controller()
{
    sendbuf->ExternFlag=StandardFrame;//CAN帧初始化，ID为标准帧
    sendbuf->DataLen=static_cast<BYTE>(CanDataLength);//数据长度为8位
    sendbuf->RemoteFlag=DataFrame;//DATA为数据帧
    for(int i=0;i<8;++i){
        sendbuf->Data[i]=0x00;//DATA数据默认为0x00
    }
    //    View *m_view=new View();
    //    connect(this,&Controller::rec,m_model,)

}


void Controller::btnOpenClick()
{
    devindex=0;//开启第一个CAN适配器
    int Doublechoose=1;
    devtype=Doublechoose==1?VCI_USBCAN2:VCI_USBCAN1;//设备类型为CANalyst-II
    DWORD reserved=0;

    if(VCI_OpenDevice(devtype,devindex,reserved)!=1){
        //        QMessageBox::warning(this,QString::fromLocal8Bit("Warning"),
        //                                 QString::fromLocal8Bit("Open Device failed!"));
        qDebug()<<"Open Device failed!";
        return;
    }
    VCI_INIT_CONFIG init_info[1];
    init_info->Timing0=0x00;
    init_info->Timing1=0x14;//波特率设为1M
    init_info->Filter=0;//同时对标准帧和拓展帧进行过滤
    init_info->AccCode=0x80000008;
    init_info->AccMask=0xFFFFFFFF;//接收所有消息
    init_info->Mode=0;//工作模式为正常工作模式

    if(VCI_InitCAN(devtype,devindex,0,init_info)!=1)//初始化通道0
    {
        //        QMessageBox::warning(this,QString::fromLocal8Bit("Warning"),
        //                                 QString::fromLocal8Bit("CAN init failed!"));
        qDebug()<<"CAN init failed!";
        return;
    }
    Sleep(100);
    if(VCI_StartCAN(devtype,devindex,0)!=1)//开启通道0
    {
        //        QMessageBox::warning(this,QString::fromLocal8Bit("Warning"),
        //                                 QString::fromLocal8Bit("Start CAN failed!"));
        qDebug()<<"Start CAN failed!";
        return;
    }
    qDebug()<<"Open CAN success!";
}


void Controller::btnCloseClick()
{
    if(VCI_CloseDevice(devtype,devindex)!=1)
    {
        //        QMessageBox::warning(this,QString::fromLocal8Bit("Warning"),
        //                                 QString::fromLocal8Bit("Close failed!"));
        qDebug()<<"Close failed!";
        return;
    }

    //    QMessageBox::information(this,QString::fromLocal8Bit("Information"),
    //                             QString::fromLocal8Bit("Close success!"));
    qDebug()<<"Close success!";
    //    flagReceive=2;
}

int Controller::Hex2Dec(char c)
{
    if((c>='0') && (c<='9'))
        return c-0x30;
    else if((c>='A') && (c<='F'))
        return c-'A'+10;
    else if((c>='a') && (c<='f'))
        return c-'a'+10;
    else
        return 0x10;
}

int Controller::DouHex2Dec(QString s)
{
    int len=s.length();
    string str=s.toStdString();
    if(len==2)
    {
        int a=Hex2Dec(str[0]);
        int b=Hex2Dec(str[1]);
        if(a==16||b==16)
        {
            qDebug()<<"Format error";
            return 256;
        }
        else
        {
            return a*16+b;
        }
    }
    else
    {
        qDebug()<<"input length must be 2";
        return 256;
    }
}

void Controller::btnSendClick(QString Id,QString Data)
{
    QString str5[32];
    BYTE databuf[50];
    BYTE idbuf[10];
    QString strtemp,strtemp1;
    QString str;
    int len,datanum=0,idnum=0,flag=1,i;
    QString qstr_id=Id;
    QString qstr_data=Data;

    len=qstr_id.length();
    for(i=0;i<len;++i)
    {
        strtemp=qstr_id.at(i);
        if(strtemp==" ")
            flag=1;
        else if(flag==1)
        {
            flag=0;
            strtemp=qstr_id.at(i);
            if(i==(len-1))
            {
                str5[idnum]="0"+strtemp;
            }
            else
            {
                strtemp1=qstr_id.at(i+1);
                if(strtemp1==" ")
                    str5[idnum]="0"+strtemp;
                else
                    str5[idnum]=strtemp+strtemp1;
            }
            idbuf[idnum]=static_cast<BYTE>(DouHex2Dec(str5[idnum]));
            idnum++;
            if(idnum>=4)
                break;
        }
    }

    flag=1;
    len=qstr_data.length();
    for(i=0;i<len;++i)
    {
        strtemp=qstr_data.at(i);
        if(strtemp==" ")
            flag=1;
        else if(flag==1)
        {
            flag=0;
            strtemp=qstr_data.at(i);
            if(i==(len-1))
            {
                str5[datanum]="0"+strtemp;
            }
            else
            {
                strtemp1=qstr_data.at(i+1);
                if(strtemp1==" ")
                    str5[datanum]="0"+strtemp;
                else
                    str5[datanum]=strtemp+strtemp1;
            }
            databuf[datanum]=static_cast<BYTE>(DouHex2Dec(str5[datanum]));
            datanum++;
            if(datanum>=8)
                break;
        }
    }
    sendbuf->ID=static_cast<UINT>(idbuf[0]*pow(16,6)+idbuf[1]*pow(16,4)+idbuf[2]*pow(16,2)+idbuf[3]);
    for(i=0;i<datanum;++i)
    {
        sendbuf->Data[i]=databuf[i];
    }

    CanSend();
}

void Controller::CanSend()
{
    int NumCanSend=static_cast<int>(VCI_Transmit(devtype,devindex,static_cast<DWORD>(0),sendbuf,1));
    if(NumCanSend<1){
        switch (NumCanSend) {
        case -1:qDebug()<<"Device isn't open";return;
            //        case 0:qDebug()<<"send error";return;
        default:return;
        }
    }
    qDebug()<<"Cansend success";
    //重新初始化sendbuf
    sendbuf->ExternFlag=StandardFrame;//CAN帧初始化，ID为标准帧
    sendbuf->DataLen=static_cast<BYTE>(CanDataLength);//数据长度为8位
    sendbuf->RemoteFlag=DataFrame;//DATA为数据帧
    for(int i=0;i<8;++i){
        sendbuf->Data[i]=0x00;//DATA数据默认为0x00
    }
    Sleep(10);
}

void Controller::btnEnableClick()
{
    sendbuf->ID=0x00000302;
    sendbuf->Data[0]=0x4D;
    sendbuf->Data[1]=0x4F;
    sendbuf->Data[4]=0x01;
    CanSend();
    Sleep(50);

    sendbuf->ID=0x00000303;
    sendbuf->Data[0]=0x4D;
    sendbuf->Data[1]=0x4F;
    sendbuf->Data[4]=0x01;
    CanSend();
    Sleep(50);

    sendbuf->ID=0x00000304;
    sendbuf->Data[0]=0x4D;
    sendbuf->Data[1]=0x4F;
    sendbuf->Data[4]=0x01;
    CanSend();
    Sleep(50);
}

void Controller::inquire()
{
    int i=0;
    while(flagReceive)
    {
        sendbuf->ID=0x00000302;
        sendbuf->Data[0]=0x41;
        sendbuf->Data[1]=0x45;
        sendbuf->Data[2]=0x01;
        CanSend();
        Sleep(50);

        //        sendbuf->ID=0x00000303;
        //        sendbuf->Data[0]=0x41;
        //        sendbuf->Data[1]=0x45;
        //        sendbuf->Data[2]=0x01;
        //        CanSend();
        //        Sleep(50);

        //        sendbuf->ID=0x00000304;
        //        sendbuf->Data[0]=0x41;
        //        sendbuf->Data[1]=0x45;
        //        sendbuf->Data[2]=0x01;
        //        CanSend();
        //        Sleep(50);

        Sleep(1000);
        QString log;
        log.sprintf("%p",QThread::currentThread());
        qDebug()<<"testinquire"<<++i<<log;
        Sleep(1000);
        if(flagReceive==2)
        {
            return;
        }
    }
}


void Controller::receive()
{
    int i=0;
    while(flagReceive)
    {
        VCI_CAN_OBJ pCanObj[200];
        int NumCanReceive;
        QString strRecId,strRecData,str;
        NumCanReceive=VCI_Receive(devtype,devindex,0,pCanObj,200,0);
        if(NumCanReceive<=0)
        {
            //            qDebug()<<"receive error";
//            QString log;
//            log.sprintf("%p",QThread::currentThread());
//            qDebug()<<"test"<<++i<<log;
        }
        else
        {
            QString log;
            log.sprintf("%p",QThread::currentThread());
            qDebug()<<"test"<<++i<<log;
            for(int ind=0;ind<NumCanReceive;++ind)
            {
                ReceiveId=pCanObj[ind].ID;
                for(int i=0;i<8;++i)
                {
                    ReceiveData[i]=pCanObj[ind].Data[i];
                    str=QString::number(ReceiveData[i],16);
                    str=QString("%2").arg(ReceiveData[i],2,16,QLatin1Char('0'));
                    if(i<7)
                    {
                        strRecData=strRecData+str+" ";
                    }
                    else
                    {
                        strRecData+=str;
                        strRecData=strRecData.toUpper();
                    }
                }
                strRecId=QString("%8").arg(ReceiveId,8,16,QLatin1Char('0')).toUpper();

                emit rec(strRecId,strRecData);


                if(ReceiveId==0x281)
                {

                }


            }
        }
        Sleep(1000);
        if(flagReceive==2)
        {
            return;
        }
    }
}