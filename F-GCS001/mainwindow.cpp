#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "inifile.h"
#include "QKeyEvent"
#include "waypointitem.h"
#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->installEventFilter(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    //setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    //this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    //工具栏初始化
    /*
    ui->Airplane->setEnabled(false);
    ui->Setting->setEnabled(false);
    ui->SaveData->setEnabled(false);
    ui->MapSetting->setEnabled(false);
    ui->Mission->setEnabled(false);
    ui->Rule->setEnabled(false);
    */
    /*
    ui->RouteButton->setHidden(true);
    ui->LandingButton->setHidden(true);
    ui->TakeOffButton->setHidden(true);
    ui->LockButton->setHidden(true);
    */

    /*ui->TravelButton->setEnabled(false);
    ui->RouteButton->setEnabled(false);
    ui->LandingButton->setEnabled(false);
    ui->BackhomeButton->setEnabled(false);
    ui->TakeOffButton->setEnabled(false);
    ui->CutOff->setHidden(true);
    */
    //声音初始化
    IniFile *SoundIni = new IniFile();
    isOpenSound =  SoundIni->ReadIni("FGCS.ini","Sound","Open").toInt();
    AlertSound = new QSound("./Alert/Alert.wav");
    AlertSound->setLoops(-1);
    //AlertSound->play();
    //变量初始化
    datasave = NULL;
    SaveFreq = 100;//默认10Hz频率记录数据

    m_UseSpeed = 0;
    m_wzSpeed = 0;
    m_BoardHight = 0;

    //初始化飞机状态
    AirPlaneLockStatus = false;
    //心跳定时器初始化
    HeartBeat = new QTimer;
    connect(HeartBeat,SIGNAL(timeout()),
            this,SLOT(SendHeartBeat(void)));


    //计数器初始化
    TickTimer = new QTimer;
    connect(TickTimer,SIGNAL(timeout()),
            this,SLOT(TickTimerOut()));

    //窗口数量控制初始化
    isSetSerialOpen  = 0;
    isSetMissionOpen = 0;
    isAirplaneOpen   = 0;
    isRuleOpen       = 0;
    isSettingOpen    = 0;
    isMapSettingOpen = 0;
    isSaveDataOpen   = 0;


    //串口初始化
    myPort = NULL;
    myPort = new QextSerialPort(QextSerialPort::EventDriven,this);
    myPort->setPortName("-1");
    connect(myPort,SIGNAL(readyRead()),this,SLOT(ReadSerial()));



    myDecode = new Decode;
    DecodeThread = new QThread;
    myDecode->moveToThread(DecodeThread);

    //myDecode->start();

    datasaveThread = new QThread;
    datasave = new DataSave;
    datasave->moveToThread(datasaveThread);

    connect(myDecode,SIGNAL(ShowRemote(quint8,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint8,quint8,double,double,float,float,quint8,quint8,double,double,float,float,quint8,quint8,quint8,quint8,float,float,float,float,float,float,float,float,float,float,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint16,quint16,float,float,float,float,float,float,float,float,float,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
            datasave,SLOT(Record(quint8,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint8,quint8,double,double,float,float,quint8,quint8,double,double,float,float,quint8,quint8,quint8,quint8,float,float,float,float,float,float,float,float,float,float,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint16,quint16,float,float,float,float,float,float,float,float,float,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);

    connect(myDecode,SIGNAL(ShowRemote(quint8,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint8,quint8,double,double,float,float,quint8,quint8,double,double,float,float,quint8,quint8,quint8,quint8,float,float,float,float,float,float,float,float,float,float,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint16,quint16,float,float,float,float,float,float,float,float,float,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
            this,SLOT(DisplayRemote(quint8,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint8,quint8,double,double,float,float,quint8,quint8,double,double,float,float,quint8,quint8,quint8,quint8,float,float,float,float,float,float,float,float,float,float,quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,quint16,quint16,float,float,float,float,float,float,float,float,float,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);

    connect(this,SIGNAL(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
            myDecode,SLOT(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);


    connect(DecodeThread,SIGNAL(started()),
            myDecode,SLOT(DataThread()),Qt::QueuedConnection);


    DecodeThread->start();
    datasaveThread->start();

    SaveTimer = new QTimer;

    //==========地图==============

    IniFile *myinifile = new IniFile();
    QString wploop = myinifile->ReadIni("FGCS.ini","Map","WPLoop");
    isWPLoop = wploop.toInt();

    map = new mapcontrol::OPMapWidget();
    map->SetShowHome(false);
    map->SetUseOpenGL(true);
    QString maptype = myinifile->ReadIni("FGCS.ini","Map","Type");
    map->SetMapType(mapcontrol::Helper::MapTypeFromString(maptype));
    ui->MapgridLayout->addWidget(map);

    map->setFocus();
    map->SetShowCompass(false);
    map->setMouseTracking(true);

    //=========初始化UAV=========
    map->SetShowUAV(true);//先show才能设置
    map->SetUavPic("Rotor.png");
    ui->Panel->setPlaneMode("多旋翼");

    internals::PointLatLng position;
    position.SetLat(0);
    position.SetLng(0);
    map->UAV->SetUAVPos(position,10);
    map->UAV->SetShowTrail(false);

    connect(map,SIGNAL(EmitCurrentMousePosition(internals::PointLatLng)),
            this,SLOT(ShowMousePos(internals::PointLatLng)));
    connect(map,SIGNAL(WPChanged(int,double,double)),
            this,SLOT(WPChanged(int,double,double)));

    connect(map,SIGNAL(MouseDoubleClickEvent(QMouseEvent*)),
            this,SLOT(on_MouseDoubleClickEvent(QMouseEvent*)));

    delete myinifile;
}
//=================航点===========================
unsigned char point = 0;
mapcontrol::WayPointItem *OldItem;//如果清空那么要清除这个指针
mapcontrol::WayPointItem *NewItem;
mapcontrol::WayPointItem *CircleItem;
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << event;

    event = event;
    internals::PointLatLng LatLng;
    LatLng.SetLat(map->currentMousePosition().Lat());
    LatLng.SetLng(map->currentMousePosition().Lng());
    ui->statusBar->showMessage(map->currentMousePosition().ToString(),1000);
    if((isSetMissionOpen == true)&&(isRuleOpen == false))//任务窗口打开，并且尺子不打开才是航点选取
    {
        if(isWPLoop == false)
        {
            NewItem = map->WPCreate(LatLng,0);
            if(OldItem != NULL)
            {
                NewItem->SetNumber(OldItem->Number()+1);
            }

            map->WPLineCreate(OldItem,NewItem,Qt::green,false,2);
            map->WPCircleCreate(NewItem,800,1,Qt::blue,false,3);
            OldItem = NewItem;
            emit ChangePoint(NewItem);
        }
        else
        {
            if(OldItem != NULL)
            {
                mapcontrol::WayPointItem *Item1;
                mapcontrol::WayPointItem *Item2;
                mapcontrol::WayPointItem *Item3;
                //mapcontrol::WayPointItem *Item4;

                Item1 = map->WPCreate(OldItem->Coord(),0);
                int Number = OldItem->Number();

                map->WPDelete(OldItem->Number());
                Item1->SetNumber(Number);//删除和生成了
                Item2 = map->WPFind(Item1->Number() - 1);
                map->WPLineCreate(Item2,Item1,Qt::green,false,2);
                OldItem = Item1;

                Item3 = map->WPCreate(LatLng,0);
                map->WPLineCreate(OldItem,Item3,Qt::green,false,2);

                emit ChangePoint(Item3);//在列表里面生成这个
                OldItem = Item3;

                map->WPLineCreate(OldItem,map->WPFind(0),Qt::green,false,2);
            }
            else
            {
                NewItem = map->WPCreate(LatLng,0);
                if(OldItem != NULL)
                {
                    NewItem->SetNumber(OldItem->Number()+1);
                }

                map->WPLineCreate(OldItem,NewItem,Qt::green,false,2);
                OldItem = NewItem;
                emit ChangePoint(NewItem);
            }

        }

    }
    else if(isRuleOpen == true)//尺子窗口打开默认是测量
    {
        emit RulePoint(LatLng);
    }       

}

void MainWindow::on_MouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)//右键双击
    {

    }
    else if(event->button() == Qt::LeftButton)
    {

    }
    else if(event->button() == Qt::MiddleButton)
    {

    }
}



//=======================航点画圆=====================
void MainWindow::WPCircle(mapcontrol::WayPointItem *Item,mapcontrol::WayPointItem *CircleItem,double Radius)
{
        double Angle = Radius / 111318.0f;
        CircleItem = Item;

        CircleItem->Coord().SetLat(CircleItem->Coord().Lat() + Angle);
        qDebug() << Angle
                 << QString::number(Item->Coord().Lat(),'f',8)
                 << QString::number(Item->Coord().Lng(),'f',8)
                 << QString::number(CircleItem->Coord().Lat(),'f',8)
                 << QString::number(CircleItem->Coord().Lng(),'f',8);
}





//=======键盘按键处理
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case '-':
          {
                double zoom = map->ZoomTotal();
                zoom = zoom - 1;
                map->SetZoom(zoom);
          }break;
    case Qt::Key_Equal:
          {
                double zoom = map->ZoomTotal();
                zoom = zoom + 1;
                map->SetZoom(zoom);
          }break;
          case Qt::Key_Space:
          {
                map->SetCurrentPosition(CurrentPoint);
          }break;
          case Qt::Key_W:
          {
                internals::PointLatLng CurrentPoint;
                CurrentPoint.SetLat(map->CurrentPosition().Lat() + map->CurrentPosition().Lat()/(map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()));
                CurrentPoint.SetLng(map->CurrentPosition().Lng());
                map->SetCurrentPosition(CurrentPoint);
          }break;
          case Qt::Key_S:
          {
                internals::PointLatLng CurrentPoint;
                CurrentPoint.SetLat(map->CurrentPosition().Lat() - map->CurrentPosition().Lat()/(map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()));
                CurrentPoint.SetLng(map->CurrentPosition().Lng());
                map->SetCurrentPosition(CurrentPoint);
          }break;
          case Qt::Key_A:
          {
                internals::PointLatLng CurrentPoint;
                CurrentPoint.SetLat(map->CurrentPosition().Lat());
                CurrentPoint.SetLng(map->CurrentPosition().Lng() - map->CurrentPosition().Lng()/(map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()));
                map->SetCurrentPosition(CurrentPoint);
          }break;
          case Qt::Key_D:
          {
                internals::PointLatLng CurrentPoint;
                CurrentPoint.SetLat(map->CurrentPosition().Lat());
                CurrentPoint.SetLng(map->CurrentPosition().Lng() + map->CurrentPosition().Lng()/(map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()*map->ZoomTotal()));
                map->SetCurrentPosition(CurrentPoint);
          }break;
    }
    map->update();
}



//=====关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->closeAllWindows();
    event->accept();
}


//==============删除航点=========================
void MainWindow::WPdelete(int number)
{
    mapcontrol::WayPointItem *Item1;
    mapcontrol::WayPointItem *Item2;

    if(number == OldItem->Number())
    {
        OldItem = map->WPFind(OldItem->Number() - 1);
        if(isWPLoop == true)
        {
          map->WPLineCreate(OldItem,map->WPFind(0),Qt::green,false,2);
        }
    }

    map->WPDelete(number);

    if(number == 0)
    {
        if(isWPLoop == true)
        {
          map->WPLineCreate(OldItem,map->WPFind(0),Qt::green,false,2);
        }
    }

    if(number > 0)
    {
      Item1 = map->WPFind(number-1);
      Item2 = map->WPFind(number);
      map->WPLineCreate(Item1,Item2,Qt::green,false,2);
    }


}
//=============清除所有航点======================
void MainWindow::WPdeleteALL(void)
{
    map->WPDeleteAll();
    OldItem = NULL;
}
//=============设置所有航点=====================
void MainWindow::WPSetALL(void)//设置的时候要把高度和半径设置上
{
     mapcontrol::WayPointItem *w;
     int i = 0;
     while(i != -1)
     {
         w = map->WPFind(i);
         if(w)
         {
             emit ChangePoint(w);
             i++;
         }
         else
         {
             i = -1;
         }
     }
}
//===========航点循环设置================
void MainWindow::SetWPLoop(bool Flag)
{
     isWPLoop = Flag;//循环
     if(OldItem != 0)
     {
         if(isWPLoop == true)//循环
         {
               mapcontrol::WayPointItem *Item1;
               Item1 = map->WPFind(0);
               map->WPLineCreate(OldItem,Item1,Qt::green,false,2);
         }
         else//不循环
         {
             mapcontrol::WayPointItem *Item1;
             mapcontrol::WayPointItem *Item2;

             Item1 = map->WPCreate(OldItem->Coord(),0);
             int Number = OldItem->Number();

             map->WPDelete(OldItem->Number());

             Item1->SetNumber(Number);

             Item2 = map->WPFind(Item1->Number() - 1);

             map->WPLineCreate(Item2,Item1,Qt::green,false,2);
             OldItem = Item1;
         }
     }


     //map->update();
}

//=============航点变化===================
void MainWindow::WPChanged(int Number,double Lat,double Lng)
{
    Lat = Lat;
    Lng = Lng;
    mapcontrol::WayPointItem *Item;
    Item = map->WPFind(Number);
    emit WPChangedAPoint(Number,Item->Coord().Lat(),Item->Coord().Lng());
}
//============显示鼠标所在点===================
void MainWindow::ShowMousePos(internals::PointLatLng pos)
{
    QString str = QString::number(pos.Lat(),'f',8);
    str.append(" ");
    str.append(QString::number(pos.Lng(),'f',8));
    ui->statusBar->showMessage(str,1000);
}
//===============显示下载的航点==================
void MainWindow::ShowDownloadWaypoint(quint16 CurrentPoint, double Latitude, double Longitude)
{

    internals::PointLatLng LatLng;
    LatLng.SetLat(Latitude);
    LatLng.SetLng(Longitude);

    if((OldItem != NULL)&&(OldItem->Number() == CurrentPoint))
    {
        return;
    }
    if(isWPLoop == false)
    {
        NewItem = map->WPCreate(LatLng,0);
        NewItem->SetNumber(CurrentPoint);

        map->WPLineCreate(OldItem,NewItem,Qt::green,false,2);
        OldItem = NewItem;
    }
    else
    {
        if(OldItem != NULL)
        {
            mapcontrol::WayPointItem *Item1;
            mapcontrol::WayPointItem *Item2;
            mapcontrol::WayPointItem *Item3;
            //mapcontrol::WayPointItem *Item4;

            Item1 = map->WPCreate(OldItem->Coord(),0);
            int Number = OldItem->Number();

            map->WPDelete(OldItem->Number());
            Item1->SetNumber(Number);//删除和生成了
            Item2 = map->WPFind(Item1->Number() - 1);
            map->WPLineCreate(Item2,Item1,Qt::green,false,2);
            OldItem = Item1;

            Item3 = map->WPCreate(LatLng,0);
            map->WPLineCreate(OldItem,Item3,Qt::green,false,2);

           // emit ChangePoint(Item3);//在列表里面生成这个
            OldItem = Item3;

            map->WPLineCreate(OldItem,map->WPFind(0),Qt::green,false,2);
        }
        else
        {
            NewItem = map->WPCreate(LatLng,0);
            if(OldItem != NULL)
            {
                NewItem->SetNumber(OldItem->Number()+1);
            }

            map->WPLineCreate(OldItem,NewItem,Qt::green,false,2);
            OldItem = NewItem;
            //emit ChangePoint(NewItem);
        }
    }

}

//===============清除航迹=========================
void MainWindow::CleanTrail(bool Flag)
{
    Flag = Flag;
    map->UAV->DeleteTrail();
}

//===============窗口管理==========================

void MainWindow::WindowIDUpdata(char id)
{
     if(id == 1)
     {
         isSetSerialOpen = 0;
     }
     else if(id == 2)
     {
         isAirplaneOpen  = 0;
     }
     else if(id == 3)
     {
         isSettingOpen  = 0;
     }
     else if(id == 4)
     {
         isSetMissionOpen  = 0;
     }
     else if(id == 5)
     {
         isRuleOpen  = 0;
     }
     else if(id == 6)
     {
         isMapSettingOpen  = 0;
     }
     else if(id == 7)
     {
         isSaveDataOpen  = 0;
     }
}

//===============通讯管理=======================================
void MainWindow::on_DataLink_triggered()
{
    if(isSetSerialOpen == 0)
    {
        isSetSerialOpen = 1;
        DataLinkDialog *SetSerial = new DataLinkDialog();
        connect(SetSerial,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));
        connect(SetSerial,SIGNAL(SerialPortInfo(QString,BaudRateType,bool)),this,SLOT(SerialPortConnect(QString,BaudRateType,bool)));
        connect(this,SIGNAL(isSerialPortHasOpen(bool)),SetSerial,SLOT(UpdateWindow(bool)));
        SetSerial->show();
        emit isSerialPortHasOpen(myPort->isOpen());
        SetSerial->exec();
        delete SetSerial;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}
//============串口打开、关闭===========================
void MainWindow::SerialPortConnect(QString Name,BaudRateType Baud,bool Flag)
{

     if(Flag == true)
     {
         if(!myPort->isOpen())
         {
             myPort->close();
             myPort->setPortName(Name);
             myPort->open(QIODevice::ReadWrite);
             myPort->setBaudRate(Baud);
             myPort->setDataBits(DATA_8);
             myPort->setParity(PAR_NONE);
             myPort->setStopBits(STOP_1);
             myPort->setFlowControl(FLOW_OFF);
             if(myPort->isOpen())
             {
                 emit isSerialPortHasOpen(true);
                 //可能存在问题，因为new两次，或者没有new就delete了
                 connect(this,SIGNAL(SerialData(QByteArray)),myDecode,SLOT(GetSerialData(QByteArray)),Qt::QueuedConnection);

                 connect(myPort,SIGNAL(readyRead()),this,SLOT(ReadSerial()),Qt::QueuedConnection);

                 connect(myDecode,SIGNAL(SendFrame(QByteArray)),myPort,SLOT(SendFrame(QByteArray)),Qt::QueuedConnection);
                 connect(this,SIGNAL(HeartBeatSignal()),
                         myDecode,SLOT(SendBeat()),Qt::QueuedConnection);

                 //===========
                 ui->TakeOffButton->setEnabled(true);//如果成功打开串口，那么解开起飞按键
                 //===========
                 HeartBeat->start(1000);//1s来一次
                 SaveTimer->start(SaveFreq);//100ms
                 ui->DataLink->setIcon(QIcon(":/image/resource/datalink.png"));
             }
         }
         else
         {
             //失能起飞按键
             disconnect(myPort,SIGNAL(readyRead()),this,SLOT(ReadSerial()));
             disconnect(this,SIGNAL(SerialData(QByteArray)),myDecode,SLOT(GetSerialData(QByteArray)));

             disconnect(myDecode,SIGNAL(SendFrame(QByteArray)),myPort,SLOT(SendFrame(QByteArray)));
             disconnect(this,SIGNAL(HeartBeatSignal()),
                     myDecode,SLOT(SendBeat()));

             ui->TakeOffButton->setEnabled(false);
             myPort->close();

             SaveTimer->stop();
             HeartBeat->stop();
             emit isSerialPortHasOpen(false);
             ui->DataLink->setIcon(QIcon(":/image/resource/datalink_Dis.png"));
             AlertSound->stop();
         }
     }
     else
     {
         disconnect(myPort,SIGNAL(readyRead()),this,SLOT(ReadSerial()));
         disconnect(this,SIGNAL(SerialData(QByteArray)),myDecode,SLOT(GetSerialData(QByteArray)));

         disconnect(myDecode,SIGNAL(SendFrame(QByteArray)),myPort,SLOT(SendFrame(QByteArray)));
         disconnect(this,SIGNAL(HeartBeatSignal()),
                 myDecode,SLOT(SendBeat()));

         ui->TakeOffButton->setEnabled(false);
         myPort->close();
         SaveTimer->stop();
         HeartBeat->stop();
         emit isSerialPortHasOpen(false);
         ui->DataLink->setIcon(QIcon(":/image/resource/datalink_Dis.png"));
         AlertSound->stop();
     }
}
//==================设置数据记录频率============
void MainWindow::SaveTimerSetting(int Freq)//重新设置记录频率
{
    SaveFreq = Freq;
    SaveTimer->stop();
    SaveTimer->start(Freq);
}
//==================读取串口=================
quint8 DataCount = 0;
void MainWindow::ReadSerial(void)
{
    //qDebug() << "read";

    QByteArray Data = myPort->readAll();
    DataCount++;

    QColor Color(125,DataCount,0);
    ui->Panel->setLed(Color);


    emit SerialData(Data);
}
//================发送心跳帧=================
void MainWindow::SendHeartBeat(void)
{
    emit HeartBeatSignal();//发送心跳指令，防止飞机飞出数传半径之外
}
//================任务窗口==================
void MainWindow::on_Mission_triggered()
{
    if(isSetMissionOpen == 0)
    {
        isSetMissionOpen = 1;
        MissionDialog *SetMission = new MissionDialog();

        connect(SetMission,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));
        connect(this,SIGNAL(ChangePoint(mapcontrol::WayPointItem*)),SetMission,SLOT(GetWayPoint(mapcontrol::WayPointItem*)));
        connect(SetMission,SIGNAL(DeleteAll()),this,SLOT(WPdeleteALL()));
        connect(SetMission,SIGNAL(DeletePoint(int)),this,SLOT(WPdelete(int)));

       // connect(myDecode,SIGNAL(ShowEcho(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
               // SetMission,SLOT(isSendPointComplete(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)));

        connect(SetMission,SIGNAL(SendDot(quint8,quint8,quint16,quint16,double,double,float,quint16,quint16,quint16)),
                myDecode,SLOT(SendDot(quint8,quint8,quint16,quint16,double,double,float,quint16,quint16,quint16)),Qt::QueuedConnection);

        connect(myDecode,SIGNAL(ShowEcho(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
                SetMission,SLOT(ShowEcho(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);


        connect(this,SIGNAL(WPChangedAPoint(int,double,double)),
                SetMission,SLOT(ChangePoint(int,double,double)));

        connect(SetMission,SIGNAL(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
                myDecode,SLOT(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);

        connect(myDecode,SIGNAL(ShowDot(quint8,quint8,quint16,quint16,double,double,float,quint16,quint16,quint16)),
                SetMission,SLOT(ReceiverDot(quint8,quint8,quint16,quint16,double,double,float,quint16,quint16,quint16)),Qt::QueuedConnection);

        connect(SetMission,SIGNAL(ShowDownLoadWayPoint(quint16,double,double)),
                this,SLOT(ShowDownloadWaypoint(quint16,double,double)));

        connect(SetMission,SIGNAL(SetWPLoop(bool)),
                this,SLOT(SetWPLoop(bool)));

        SetMission->show();
        WPSetALL();
        SetMission->exec();
        delete SetMission;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}
//==============飞机窗口==========================
void MainWindow::on_Airplane_triggered()
{
    if(isAirplaneOpen == 0)
    {
        isAirplaneOpen = 1;
        AirPlaneDialog *AirPlane = new AirPlaneDialog();
        connect(AirPlane,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));

        connect(AirPlane,SIGNAL(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
                myDecode,SLOT(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);



        connect(AirPlane,SIGNAL(SendTaskSetting(quint8,float,float,float,float,float,float,float,float)),
                myDecode,SLOT(SendTaskSetting(quint8,float,float,float,float,float,float,float,float)),Qt::QueuedConnection);

        connect(myDecode,SIGNAL(ShowTaskSet(quint8,float,float,float,float,float,float,float,float)),
                AirPlane,SLOT(ShowTaskSetting(quint8,float,float,float,float,float,float,float,float)),Qt::QueuedConnection);

        AirPlane->show();
        AirPlane->exec();
        delete AirPlane;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}
//================尺子窗口=======================
void MainWindow::on_Rule_triggered()
{
    if(isRuleOpen == 0)
    {
        isRuleOpen = 1;
        RuleDialog *Rule = new RuleDialog();
        connect(Rule,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));

        connect(this,SIGNAL(RulePoint(internals::PointLatLng)),
                Rule,SLOT(RulePoint(internals::PointLatLng)));

        connect(map,SIGNAL(EmitCurrentMousePosition(internals::PointLatLng)),
                Rule,SLOT(ShowMousePos(internals::PointLatLng)),Qt::QueuedConnection);

        Rule->show();
        Rule->exec();
        delete Rule;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}
//====================设置窗口=====================
void MainWindow::on_Setting_triggered()
{
    if(isSettingOpen == 0)
    {
        isSettingOpen = 1;
        SettingDialog *Setting = new SettingDialog();
        connect(Setting,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));

        connect(myDecode,SIGNAL(ShowPID(quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
                Setting,SLOT(ShowPID(quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),Qt::QueuedConnection);

        connect(myDecode,SIGNAL(ShowCalibration(quint8,float,float,float)),
                Setting,SLOT(ShowCalibration(quint8,float,float,float)),Qt::QueuedConnection);

        connect(myDecode,SIGNAL(ShowAttitude(float,float,float)),
                Setting,SLOT(ShowAttitude(float,float,float)),Qt::QueuedConnection);

        connect(Setting,SIGNAL(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
                myDecode,SLOT(SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)));

        connect(Setting,SIGNAL(SendPID(quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
                myDecode,SLOT(SendPID(quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),Qt::QueuedConnection);

        connect(Setting,SIGNAL(SendScale(quint8,float,float,float,float,float,float)),
                myDecode,SLOT(SendScale(quint8,float,float,float,float,float,float)));

        //输出设置
        connect(Setting,SIGNAL(SendServoPolar(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),
                myDecode,SLOT(SendServoPolar(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)),Qt::QueuedConnection);

        connect(Setting,SIGNAL(SendServoPolynomial(quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
                myDecode,SLOT(SendServoPolynomial(quint8,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),Qt::QueuedConnection);


        connect(myDecode,SIGNAL(ShowServoPolar(qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8)),
                Setting,SLOT(ShowServoPolar(qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8,qint8)),Qt::QueuedConnection);

        connect(myDecode,SIGNAL(ShowServoPoly(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
                Setting,SLOT(ShowServoPoly(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),Qt::QueuedConnection);

        Setting->show();
        Setting->exec();
        delete Setting;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}

//===================地图======================
void MainWindow::on_MapSetting_triggered()
{
    if(isMapSettingOpen == 0)
    {
        isMapSettingOpen = 1;
        MapSettingDialog *MapSetting = new MapSettingDialog();
        connect(MapSetting,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));

        connect(MapSetting,SIGNAL(SetMapType(QString)),
                this,SLOT(SetMapType(QString)));

        MapSetting->show();
        MapSetting->exec();
        delete MapSetting;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}
//================设置地图类型==========================
void MainWindow::SetMapType(QString str)
{
    if (map->isStarted()) {
        map->SetMapType(mapcontrol::Helper::MapTypeFromString(str));
        map->ReloadMap();
    }
}

//===================程序退出按键=================
void MainWindow::on_Exit_triggered()
{
    DecodeThread->deleteLater();
    datasaveThread->deleteLater();
    map->deleteLater();


    delete DecodeThread;
    delete datasaveThread;

    if(myPort != 0)
    {
        delete myPort;
    }
    if(myDecode !=0)
    {
        delete myDecode;
    }

    delete datasave;
    delete SaveTimer;
    delete TickTimer;
    delete map;
    delete HeartBeat;

    qApp->closeAllWindows();
    delete ui;
    exit(0);
    close();
}
//==================数据保存窗口================
void MainWindow::on_SaveData_triggered()
{
    if(isSaveDataOpen == 0)
    {
        isSaveDataOpen = 1;
        SaveDialog *SaveData = new SaveDialog();
        connect(SaveData,SIGNAL(isWindowClose(char)),this,SLOT(WindowIDUpdata(char)));

        if(datasave != NULL)//这个出现bug，在关闭串口之后会出现死机现象
        {
            connect(SaveData,SIGNAL(SendSaveDir(QString)),
                    datasave,SLOT(setFileDir(QString)),Qt::QueuedConnection);
        }

        connect(SaveData,SIGNAL(ShowAttitude(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,float,float,float)),
                this,SLOT(DisplayAttitude(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,float,float,float)));
        connect(SaveData,SIGNAL(ShowGPS1(quint8,quint8,double,double,float,float,float)),
                this,SLOT(DisplayGPS1(quint8,quint8,double,double,float,float,float)));
        connect(SaveData,SIGNAL(ShowStatus(float,float,float,float,float,float)),
                this,SLOT(DisplayStatus(float,float,float,float,float,float)));
        connect(SaveData,SIGNAL(ShowRouteInfo(quint8,quint8,quint8,quint8,quint8,quint16,float,float,float,float,float,float,qint16,qint16,qint16,qint16,float,float,float)),
                this,SLOT(DispalyRouteInfo(quint8,quint8,quint8,quint8,quint8,quint16,float,float,float,float,float,float,qint16,qint16,qint16,qint16,float,float,float)));

        connect(SaveData,SIGNAL(ShowSignals(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16)),
                this,SLOT(DisplaySignals(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16)));

        connect(SaveData,SIGNAL(SendFreq(int)),
                this,SLOT(SaveTimerSetting(int)));
        connect(SaveData,SIGNAL(CleanTrail(bool)),
                this,SLOT(CleanTrail(bool)));

        SaveData->show();
        SaveData->exec();
        delete SaveData;
    }
    else
    {
        ui->statusBar->showMessage(tr("该窗口已经存在"),1000);
    }
}

//=================================
//==========析构函数=================
//=================================
MainWindow::~MainWindow()
{
    //要是有窗口打开了先关闭窗口
    //DecodeThread->sleep(1000);
    DecodeThread->deleteLater();
    datasaveThread->deleteLater();

    if(myPort != 0)
    {
        delete myPort;
    }
    if(myDecode !=0)
    {
        delete myDecode;
    }

    delete datasave;
    delete SaveTimer;
    delete TickTimer;
    delete map;
    delete HeartBeat;

    qApp->closeAllWindows();
    delete ui;
    exit(0);
    close();
}

//==================================
//============显示函数================
//==================================
void MainWindow::DisplayRemote(quint8 RSSI,
                               quint8 IMUSelect,
                               float ACC1_X, float ACC1_Y, float ACC1_Z, float GYRO1_X, float GYRO1_Y, float GYRO1_Z, float ROLL1, float PITCH1, float YAW1,
                               float ACC2_X, float ACC2_Y, float ACC2_Z, float GYRO2_X, float GYRO2_Y, float GYRO2_Z, float ROLL2, float PITCH2, float YAW2,
                               quint8 GPS1_Status, quint8 GPS1_Stars, double Latitude1, double Longitude1, float Altitude1, float Course1,
                               quint8 GPS2_Status, quint8 GPS2_Stars, double Latitude2, double Longitude2, float Altitude2, float Course2,
                               quint8 Hour, quint8 Minute, quint8 Second,
                               quint8 SpeedSelect,
                               float AirSpeed,
                               float NorthSpeed1, float EastSpeed1, float DownSpeed1,
                               float NorthSpeed2, float EastSpeed2, float DownSpeed2,
                               float VotageMultiRotor, float VotageFixedWing, float VotageSystem,
                               quint8 isRCValid,
                               float ROL, float PIT, float THR, float RUD,
                               float RotorThrottle, float Rotor1, float Rotor2, float Rotor3, float Rotor4, float Throttle1, float Throttle2, float Aileron, float Elevator, float Rudder,
                               quint16 CurrentTargetPoint, quint16 WaitToFlight, float DiffrentDistance,
                               float DiffrentAngle, float DiffrentHeigh, float DiffrentSpeed,
                               float TargetRoll, float TargetPitch, float TargetYaw, float TargetHeigh,
                               float RelativeHeight, quint8 mode1, quint8 mode2, quint8 LockStatus, quint8 TurningStatus, quint8 CurrentStatus,
                               quint8 CtrlMode, quint8 AirPlane, quint8 FlightMode,
                               quint8 Command1,
                               quint8 Command2
)
{
    //qDebug() << QThread::currentThreadId();

    if(YAW1 < 0)
    {
        YAW1 += 360;
    }
    map->UAV->SetUAVHeading(YAW1);//0~359
    ui->Panel->setValue(PITCH1,ROLL1,YAW1);

    ui->Panel->setPressureAltitude(Altitude2);
    ui->Panel->setBattery(VotageSystem);
    ui->Panel->setAirSpeed(AirSpeed);

    switch(Command1)
    {
        case 0x00: ui->Panel->setProtect(Command1,tr("       "));
        case 0x01: ui->Panel->setProtect(Command1,tr("未知保护"));break;
        case 0x02: ui->Panel->setProtect(Command1,tr("低压保护"));break;
        case 0x03: ui->Panel->setProtect(Command1,tr("数传中断"));break;
        case 0x04: ui->Panel->setProtect(Command1,tr("失速保护"));break;
    }



    switch(CtrlMode)
    {
        case 0: ui->Panel->setCtrlMode("手动");break;
        case 1: ui->Panel->setCtrlMode("增稳");break;
        case 2: ui->Panel->setCtrlMode("自动");break;
    }

    switch(FlightMode)
    {
        case 0: ui->Panel->setPlaneMode("多旋翼"); map->SetUavPic("Rotor.png");   map->UAV->update();break;
        case 1: ui->Panel->setPlaneMode("固定翼"); map->SetUavPic("Fixwing.png"); map->UAV->update();break;
        case 2:
        {
             ui->Panel->setPlaneMode("复合翼");
             switch(CurrentStatus)
             {
                 case 0: map->SetUavPic("Rotor.png");   map->UAV->update();break;
                 case 1: map->SetUavPic("Rotor.png");   map->UAV->update();break;
                 case 2: map->SetUavPic("Fixwing.png"); map->UAV->update();break;
                 case 3: map->SetUavPic("Fixwing.png"); map->UAV->update();break;
                 case 4: map->SetUavPic("Rotor.png");   map->UAV->update();break;
                 case 5: map->SetUavPic("Rotor.png");   map->UAV->update();break;
                 default:map->SetUavPic("Rotor.png");   map->UAV->update();break;
            }
        }break;
    }

    switch(CurrentStatus)
    {
        case 0:  ui->Panel->setCurrentStatusMode("锁定");break;
        case 1:  ui->Panel->setCurrentStatusMode("起飞");break;
        case 2:  ui->Panel->setCurrentStatusMode("过渡");break;
        case 3:  ui->Panel->setCurrentStatusMode("巡航");break;
        case 4:  ui->Panel->setCurrentStatusMode("过渡");break;
        case 5:  ui->Panel->setCurrentStatusMode("降落");break;
        default: ui->Panel->setCurrentStatusMode("未知");break;
   }

   switch(LockStatus)
   {
        case 0:
            {
                ui->Panel->setLockMode("未解锁");
                ui->LockButton->setText(tr("解锁"));
                AirPlaneLockStatus = false;
            }break;
        case 1:
            {
                ui->Panel->setLockMode("已解锁");
                ui->LockButton->setText(tr("上锁"));
                AirPlaneLockStatus = true;
            }break;
   }

   switch(TurningStatus)
   {
        case 0: ui->Panel->setTurningStatus("直线");break;
        case 1: ui->Panel->setTurningStatus("右转");break;
        case 2: ui->Panel->setTurningStatus("盘旋");break;
        case 3: ui->Panel->setTurningStatus("左转");break;
   }

   internals::PointLatLng position;
   position.SetLat(Latitude1);
   position.SetLng(Longitude1);
   CurrentPoint = position;
   map->UAV->SetUAVPos(position,Altitude1);
   ui->Panel->setGPSStarNumber(GPS1_Stars);
   ui->Panel->setGPSStatus(GPS1_Status);
   ui->Panel->setGPSSpeed(sqrt(NorthSpeed1 * NorthSpeed1 + EastSpeed1 + EastSpeed1));
   ui->Panel->setGPSAltitude(Altitude1);
   ui->Panel->setCourseValue(Course1);

   if(ui->tabWidget->currentIndex() == 2)
   {
       ui->ACC_X->setText(QString::number(ACC1_X));
       ui->ACC_Y->setText(QString::number(ACC1_Y));
       ui->ACC_Z->setText(QString::number(ACC1_Z));

       ui->ACCR_X->setText(QString::number(ACC2_X));
       ui->ACCR_Y->setText(QString::number(ACC2_Y));
       ui->ACCR_Z->setText(QString::number(ACC2_Z));

       ui->ACCG_X->setText(QString::number(GYRO2_X));
       ui->ACCG_Y->setText(QString::number(GYRO2_Y));
       ui->ACCG_Z->setText(QString::number(GYRO2_Z));

       ui->GYRO_X->setText(QString::number(GYRO1_X));
       ui->GYRO_Y->setText(QString::number(GYRO1_Y));
       ui->GYRO_Z->setText(QString::number(GYRO1_Z));

       ui->TarRoll->setText(QString::number(TargetRoll));
       ui->TarPitch->setText(QString::number(TargetPitch));
       ui->TarYaw->setText(QString::number(TargetYaw));
       ui->TarHeight->setText(QString::number(TargetHeigh));


       ui->DiffrentHight->setText(QString::number(DiffrentHeigh));
       ui->DiffrentDistance->setText(QString::number(DiffrentDistance));
       ui->DiffrentAngle->setText(QString::number(DiffrentAngle));
       ui->WaitToFlight->setText(QString::number(WaitToFlight));
       ui->CurrentTargetPoint->setText(QString::number(CurrentTargetPoint));

       ui->GPS_NorthVelocity->setText(QString::number(NorthSpeed1));
       ui->GPS_EastVelocity->setText(QString::number(EastSpeed1));
       ui->GPS_DownVelocity->setText(QString::number(DownSpeed1));

       ui->Body_XVelocity->setText(QString::number(NorthSpeed2));
       ui->Body_YVelocity->setText(QString::number(EastSpeed2));
       ui->Body_ZVelocity->setText(QString::number(DownSpeed2));


       ui->GPS_Longitude->setText(QString::number(Longitude1,'f',8));
       ui->GPS_Latitude->setText(QString::number(Latitude1,'f',8));
       ui->GPS_Altitude->setText(QString::number(Altitude1));

       //ui->GPS_NorthVelocity->setText(QString::number(NorthSpeed1));
       //ui->GPS_EastVelocity->setText(QString::number(NorthSpeed1));
       //ui->GPS_DownVelocity->setText(QString::number(NorthSpeed1));

       //显示输出信息
       ui->RC_AIL->setText(QString::number(ROL));
       ui->RC_ELE->setText(QString::number(PIT));
       ui->RC_THR->setText(QString::number(THR));
       ui->RC_RUD->setText(QString::number(RUD));

       ui->ROTOR_AIL->setText(QString::number(ROL));
       ui->ROTOR_ELE->setText(QString::number(PIT));
       ui->ROTOR_THR->setText(QString::number(RotorThrottle));
       ui->ROTOR_RUD->setText(QString::number(RUD));

       ui->FIXED_AIL->setText(QString::number(Aileron));
       ui->FIXED_ELE->setText(QString::number(Elevator));
       ui->FIXED_THR->setText(QString::number(Throttle1));
       ui->FIXED_RUD->setText(QString::number(Rudder));

       ui->MultiRotor1->setText(QString::number(Rotor1));
       ui->MultiRotor2->setText(QString::number(Rotor2));
       ui->MultiRotor3->setText(QString::number(Rotor3));
       ui->MultiRotor4->setText(QString::number(Rotor4));

   }

}



void MainWindow::DisplayEcho(quint8 Echo_CMD1,quint8 Echo_CMD2,quint8 Echo_CMD3,quint8 Echo_CMD4,
                             quint8 Echo_GetRoute,quint8 Echo_Error2,quint8 Echo_Error3,quint8 Echo_Error4)
{
        Echo_CMD1 = Echo_CMD1;
        Echo_CMD2 = Echo_CMD2;
        Echo_CMD3 = Echo_CMD3;
        Echo_CMD4 = Echo_CMD4;
        Echo_GetRoute = Echo_GetRoute;
        Echo_Error3 = Echo_Error3;
        Echo_Error4 = Echo_Error4;

        switch(Echo_Error2)
        {
             case 0:
               {
                    Echo_Error2 = 0;
               }break;
             case 1:
               {
                    ui->TravelButton->setEnabled(false);
                    ui->RouteButton->setEnabled(false);
                    ui->LandingButton->setEnabled(false);
                    ui->BackhomeButton->setEnabled(false);
                    DisplaySenserERR(Echo_Error2);
               }break;
            case 2:
              {
                    DisplaySenserERR(Echo_Error2);
              }break;
            case 3:
              {
                    ui->TravelButton->setEnabled(false);
                    ui->RouteButton->setEnabled(false);
                    ui->LandingButton->setEnabled(false);
                    ui->BackhomeButton->setEnabled(false);
                    DisplaySenserERR(Echo_Error2);
              }break;
            case 4:
              {
                    ui->TravelButton->setEnabled(false);
                    ui->RouteButton->setEnabled(false);
                    ui->LandingButton->setEnabled(false);
                    ui->BackhomeButton->setEnabled(false);
                    DisplaySenserERR(Echo_Error2);
              }break;
           case 5:
              {
                    ui->TravelButton->setEnabled(false);
                    ui->RouteButton->setEnabled(false);
                    ui->LandingButton->setEnabled(false);
                    ui->BackhomeButton->setEnabled(false);
                    DisplaySenserERR(Echo_Error2);
              }break;
           case 6:
           {
                 DisplaySenserERR(Echo_Error2);
           }break;
        default:Echo_Error2 = 0;break;
        }


}
bool DialogOpen=0;
void MainWindow::DisplaySenserERR(unsigned char Data)
{
    //弹出一个小窗口，选择航线和航点
    QDialog *myabout = new QDialog(this);
    myabout->setFixedSize(200,100);
    myabout->setWindowTitle(tr("警告"));
    myabout->setModal(true);
    myabout->move(this->geometry().x(),this->geometry().y());
   switch(Data)
   {
      case 1:{
       QLabel *ERR = new QLabel(tr("<font color=blue><b>惯导EKF未准备就绪，请等待...</b></font>"));
       QHBoxLayout *mylayout = new QHBoxLayout;
       mylayout->addWidget(ERR);
       myabout->setLayout(mylayout);
   }break;
   case 2:{
       QLabel *ERR = new QLabel(tr("<font color=blue><b>飞机尚未起飞，请检查飞机当前位置</b></font>"));
       QHBoxLayout *mylayout = new QHBoxLayout;
       mylayout->addWidget(ERR);
       myabout->setLayout(mylayout);
   }break;
   case 3:{
       QLabel *ERR = new QLabel(tr("<font color=green><b>请先选择自动控制模式再起飞</b></font>"));
       QHBoxLayout *mylayout = new QHBoxLayout;
       mylayout->addWidget(ERR);
       myabout->setLayout(mylayout);
   }break;
   case 4:{
       QLabel *ERR = new QLabel(tr("<font color=red><b>请先给飞机上锁再点击起飞</b></font>"));
       QHBoxLayout *mylayout = new QHBoxLayout;
       mylayout->addWidget(ERR);
       myabout->setLayout(mylayout);
   }break;
   case 5:{
       QLabel *ERR = new QLabel(tr("<font color=red><b>飞行故障，请摆正飞机</b></font>"));
       QHBoxLayout *mylayout = new QHBoxLayout;
       mylayout->addWidget(ERR);
       myabout->setLayout(mylayout);
   }break;
   case 6:{
       QLabel *ERR = new QLabel(tr("<font color=red><b>飞机加速超过设定限制距离</b></font>"));
       QHBoxLayout *mylayout = new QHBoxLayout;
       mylayout->addWidget(ERR);
       myabout->setLayout(mylayout);
   }break;

   }//end of switch

    myabout->exec();
}


//============命令==================
void MainWindow::on_TakeOffButton_clicked()//起飞
{
    //弹出一个小窗口，选择航线和航点
    QDialog *LittleDialog = new QDialog(this);
    LittleDialog->setFixedSize(200,100);
    LittleDialog->setWindowTitle(tr("确认起飞"));
    LittleDialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    QPushButton *pushbutton = new QPushButton(tr("确定"));

    pushbutton->setWhatsThis(tr("点击后立即开启旋翼自动起飞"));

    QGridLayout *mylayout = new QGridLayout;

    mylayout->addWidget(pushbutton,1,1,1,1);
    LittleDialog->setLayout(mylayout);

    connect(pushbutton,SIGNAL(released()),
            this,SLOT(TravelCMD()));
    OrderGroupNumber = 0;//初始化为3
    OrderDotNumber = 0;//初始化为1
    AirPlaneCMDStatus = 0x11;//初始化飞机命令状态为起飞


    LittleDialog->exec();

    delete pushbutton;
    delete LittleDialog;
}

//-//////////////////////////////////////////////
void MainWindow::on_RouteButton_clicked()//巡航
{
    //弹出一个小窗口，选择航线和航点
    QDialog *LittleDialog = new QDialog(this);
    LittleDialog->setFixedSize(200,100);
    LittleDialog->setWindowTitle(tr("选择盘旋航点"));
    LittleDialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    QSpinBox *Dotspinbox = new QSpinBox();
    QSpinBox *Groupspinbox = new QSpinBox();

    QPushButton *pushbutton = new QPushButton(tr("确定"));

    QLabel *DotLable = new QLabel();
    QLabel *GroupLable = new QLabel();

    Dotspinbox->setWhatsThis(tr("目标点的编号，范围1~65536"));
    Groupspinbox->setWhatsThis(tr("目标航线的编号，范围1~5"));
    pushbutton->setWhatsThis(tr("点击后立即绕指定航线的指定点飞行"));

    DotLable->setText(tr("目标航点"));
    GroupLable->setText(tr("航线组别"));

    Groupspinbox->setValue(3);
    Groupspinbox->setMinimum(1);
    Groupspinbox->setMaximum(5);

    Dotspinbox->setValue(1);
    Dotspinbox->setMinimum(1);
    Dotspinbox->setMaximum(65536);

    QGridLayout *mylayout = new QGridLayout;

    mylayout->addWidget(GroupLable,1,1,2,2);
  mylayout->addWidget(Groupspinbox,1,3,2,2);
      mylayout->addWidget(DotLable,3,1,2,2);
    mylayout->addWidget(Dotspinbox,3,3,2,2);
    mylayout->addWidget(pushbutton,5,1,4,4);
    LittleDialog->setLayout(mylayout);

    connect(Groupspinbox,SIGNAL(valueChanged(int)),
            this,SLOT(OrderGroupChange(int)));

    connect(Dotspinbox,SIGNAL(valueChanged(int)),
            this,SLOT(OrderDotChange(int)));

    connect(pushbutton,SIGNAL(released()),
            this,SLOT(TravelCMD()));
    OrderGroupNumber = 4;//初始化为3
    OrderDotNumber = 1;//初始化为1
    AirPlaneCMDStatus = 0x22;//初始化飞机命令状态为盘旋


    LittleDialog->exec();

    delete Dotspinbox;
    delete Groupspinbox;
    delete pushbutton;
    delete GroupLable;
    delete DotLable;
    delete LittleDialog;
}

//=///////////////////////////////////////////////////
void MainWindow::on_TravelButton_clicked()//切入航线
{
    //弹出一个小窗口，选择航线和航点
    QDialog *LittleDialog = new QDialog(this);
    LittleDialog->setFixedSize(200,100);
    LittleDialog->setWindowTitle(tr("选择切入航点"));
    LittleDialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    QSpinBox *Dotspinbox = new QSpinBox();
    QSpinBox *Groupspinbox = new QSpinBox();

    QPushButton *pushbutton = new QPushButton(tr("确定"));

    QLabel *DotLable = new QLabel();
    QLabel *GroupLable = new QLabel();

    Dotspinbox->setWhatsThis(tr("目标点的编号，范围1~65536"));
    Groupspinbox->setWhatsThis(tr("目标航线的编号，范围1~5"));
    pushbutton->setWhatsThis(tr("点击后立即向指定航线的指定点飞行"));

    DotLable->setText(tr("目标航点"));
    GroupLable->setText(tr("航线组别"));

    Groupspinbox->setValue(1);
    Groupspinbox->setMinimum(1);
    Groupspinbox->setMaximum(5);

    Dotspinbox->setValue(1);
    Dotspinbox->setMinimum(1);
    Dotspinbox->setMaximum(65536);

    QGridLayout *mylayout = new QGridLayout;

    mylayout->addWidget(GroupLable,1,1,2,2);
  mylayout->addWidget(Groupspinbox,1,3,2,2);
      mylayout->addWidget(DotLable,3,1,2,2);
    mylayout->addWidget(Dotspinbox,3,3,2,2);
    mylayout->addWidget(pushbutton,5,1,4,4);
    LittleDialog->setLayout(mylayout);

    connect(Groupspinbox,SIGNAL(valueChanged(int)),
            this,SLOT(OrderGroupChange(int)));

    connect(Dotspinbox,SIGNAL(valueChanged(int)),
            this,SLOT(OrderDotChange(int)));

    connect(pushbutton,SIGNAL(released()),
            this,SLOT(TravelCMD()));
    OrderGroupNumber = 1;//初始化为1
    OrderDotNumber = 1;//初始化为1
    AirPlaneCMDStatus = 0x33;//初始化飞机命令状态为航线

    LittleDialog->exec();

    delete Dotspinbox;
    delete Groupspinbox;
    delete pushbutton;
    delete GroupLable;
    delete DotLable;
    delete LittleDialog;
}

void MainWindow::on_BackhomeButton_clicked()//回航
{
    //弹出一个小窗口，选择航线和航点
    QDialog *LittleDialog = new QDialog(this);
    LittleDialog->setFixedSize(200,100);
    LittleDialog->setWindowTitle(tr("确认返航"));
    LittleDialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    QPushButton *pushbutton = new QPushButton(tr("确定"));

    pushbutton->setWhatsThis(tr("点击后立即返航"));

    QGridLayout *mylayout = new QGridLayout;

    mylayout->addWidget(pushbutton,1,1,1,1);
    LittleDialog->setLayout(mylayout);

    connect(pushbutton,SIGNAL(released()),
            this,SLOT(TravelCMD()));
    OrderGroupNumber = 0;//初始化为0
    OrderDotNumber = 0;//初始化为0
    AirPlaneCMDStatus = 0x44;//初始化飞机命令状态为返航


    LittleDialog->exec();

    delete pushbutton;
    delete LittleDialog;

}

void MainWindow::on_LandingButton_clicked()//降落
{
    //弹出一个小窗口，选择航线和航点
    QDialog *LittleDialog = new QDialog(this);
    LittleDialog->setFixedSize(200,100);
    LittleDialog->setWindowTitle(tr("选择降落航点"));
    LittleDialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    QSpinBox *Dotspinbox = new QSpinBox();
    QSpinBox *Groupspinbox = new QSpinBox();

    QPushButton *pushbutton = new QPushButton(tr("确定"));

    QLabel *DotLable = new QLabel();
    QLabel *GroupLable = new QLabel();

    Dotspinbox->setWhatsThis(tr("目标点的编号，范围1~65536"));
    Groupspinbox->setWhatsThis(tr("目标航线的编号，范围1~5"));
    pushbutton->setWhatsThis(tr("点击后立即向指定航线的指定点飞行"));

    DotLable->setText(tr("目标航点"));
    GroupLable->setText(tr("航线组别"));

    Groupspinbox->setValue(1);
    Groupspinbox->setMinimum(1);
    Groupspinbox->setMaximum(5);

    Dotspinbox->setValue(1);
    Dotspinbox->setMinimum(1);
    Dotspinbox->setMaximum(65536);

    QGridLayout *mylayout = new QGridLayout;

    mylayout->addWidget(GroupLable,1,1,2,2);
  mylayout->addWidget(Groupspinbox,1,3,2,2);
      mylayout->addWidget(DotLable,3,1,2,2);
    mylayout->addWidget(Dotspinbox,3,3,2,2);
    mylayout->addWidget(pushbutton,5,1,4,4);
    LittleDialog->setLayout(mylayout);

    connect(Groupspinbox,SIGNAL(valueChanged(int)),
            this,SLOT(OrderGroupChange(int)));

    connect(Dotspinbox,SIGNAL(valueChanged(int)),
            this,SLOT(OrderDotChange(int)));

    connect(pushbutton,SIGNAL(released()),
            this,SLOT(TravelCMD()));
    OrderGroupNumber = 1;//初始化为1
    OrderDotNumber = 1;//初始化为1
    AirPlaneCMDStatus = 0x55;//初始化飞机命令状态为航线
    LittleDialog->exec();

    delete Dotspinbox;
    delete Groupspinbox;
    delete pushbutton;
    delete GroupLable;
    delete DotLable;
    delete LittleDialog;
}

void MainWindow::on_LockButton_clicked()//锁定
{
    if(AirPlaneLockStatus == false)//目前是未解锁
    {
           emit SendCMD(5,0,0,0,0,0,0,0,0x34);//发送解锁的命令
    }
    else if(AirPlaneLockStatus == true)//目前是已解锁
    {
           emit SendCMD(5,0,0,0,0,0,0,0,0xCB);//发送上锁命令
    }


}

void MainWindow::on_CutOff_clicked()//关车
{
    //弹出一个小窗口，选择航线和航点
    QDialog *LittleDialog = new QDialog(this);
    LittleDialog->setFixedSize(200,100);
    LittleDialog->setWindowTitle(tr("确认关车"));
    LittleDialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    QPushButton *pushbutton = new QPushButton(tr("确定"));

    pushbutton->setWhatsThis(tr("点击后立即关车"));

    QGridLayout *mylayout = new QGridLayout;

    mylayout->addWidget(pushbutton,1,1,1,1);
    LittleDialog->setLayout(mylayout);

    connect(pushbutton,SIGNAL(released()),
            this,SLOT(TravelCMD()));
    OrderGroupNumber = 0;//初始化为3
    OrderDotNumber = 0;//初始化为1
    AirPlaneCMDStatus = 0x66;//初始化飞机命令状态为关车


    LittleDialog->exec();

    delete pushbutton;
    delete LittleDialog;
}



void MainWindow::OrderGroupChange(int number)
{
    OrderGroupNumber = number;
}

void MainWindow::OrderDotChange(int number)
{
    OrderDotNumber = number;
}

void MainWindow::TravelCMD(void)
{
    union {quint8 B[2];quint16 D;}src;
    quint8 Data_To_Send[4];

    src.D = OrderGroupNumber;
    Data_To_Send[0] = src.B[0];
    Data_To_Send[1] = src.B[1];

    src.D = OrderDotNumber;
    Data_To_Send[2] = src.B[0];
    Data_To_Send[3] = src.B[1];

    emit SendCMD(5,AirPlaneCMDStatus,Data_To_Send[0],Data_To_Send[1],Data_To_Send[2],Data_To_Send[3],0,0,0);
}

//=///////////////////////////////////////////////////


void MainWindow::on_ClearTrail_clicked()
{
    CleanTrail(1);
}


void MainWindow::on_StartTiming_clicked()
{
    if(ui->StartTiming->text() == "计时")
    {
         TickTimer->start(1000);//1s一次
         Second = 0;
         Minute = 0;
         Hour = 0;
         ui->SecondlcdNumber->display(0);
         ui->MinutelcdNumber->display(0);
         ui->HourlcdNumber->display(0);

         ui->StartTiming->setText(tr("停止"));
    }
    else
    {
         TickTimer->stop();
         ui->StartTiming->setText(tr("计时"));
    }
}

void MainWindow::TickTimerOut(void)
{
    Second++;
    if(Second>59)
    {
        Second = 0;
        Minute++;
        if(Minute>59)
        {
            Minute = 0;
            Hour++;
            if(Hour>23)
            {
                Hour = 0;
            }
        }
    }
    ui->SecondlcdNumber->display(Second);
    ui->MinutelcdNumber->display(Minute);
    ui->HourlcdNumber->display(Hour);
}




void MainWindow::on_SimulationButton_clicked()
{
    if(ui->SimulationButton->text() == "进入仿真")
    {
       ui->SimulationButton->setText(tr("退出仿真"));
       emit SendCMD(0x07,0x6d,0,0,0,0,0,0,0);
    }
    else
    {
       ui->SimulationButton->setText(tr("进入仿真"));
       emit SendCMD(0x07,0,0,0,0,0,0,0,0);
    }
}
