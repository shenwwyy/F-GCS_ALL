#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QEvent"
#include "QShortcutEvent"
#include "qextserialenumerator.h"
#include "qextserialport.h"
#include "datalinkdialog.h"
#include "missiondialog.h"
#include "airplanedialog.h"
#include "ruledialog.h"
#include "settingdialog.h"
#include "mapsettingdialog.h"
#include "savedialog.h"
#include "decode.h"
#include "QThread"
#include "opmap/mapwidget/opmapwidget.h"
#include "opmap/core/maptype.h"
#include "datasave.h"
#include "opmap/mapwidget/gpsitem.h"
#include "opmapwidget.h"
#include "waypointitem.h"
#include "QTimer"
#include "uavtrailtype.h"
#include "QSpinBox"
#include <QSound>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 0 };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     //void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent* event);
    //void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);


protected:


public:
    //用于控制窗口打开的数量
    bool isSetSerialOpen;
    bool isSetMissionOpen;
    bool isAirplaneOpen;
    bool isRuleOpen;
    bool isSettingOpen;
    bool isMapSettingOpen;
    bool isSaveDataOpen;
    //用于控制航点的循环
    bool isWPLoop;


signals:
    void isSerialPortHasOpen(bool);
    void SerialData(QByteArray);

    void ChangePoint(mapcontrol::WayPointItem *Item);
    void WPChangedAPoint(int Number,double Lat,double Lng);

    void RulePoint(internals::PointLatLng LatLng);

    void SendCMD4(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);

    void HeartBeatSignal(void);

    void SendRouteInfo_Hight(float,float,float);

    void Signals(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,
                    qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,float,float,float,float);

    void SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);

private slots:

    //窗口管理
    void WindowIDUpdata(char id);

    //串口
    void on_DataLink_triggered();

    void SerialPortConnect(QString Name,BaudRateType Baud,bool Flag);
    void SaveTimerSetting(int Freq);
    void ReadSerial(void);

    //心跳
    void SendHeartBeat(void);

    //计数器
    void TickTimerOut(void);

    //任务
    void on_Mission_triggered();

    void on_Airplane_triggered();

    void on_Rule_triggered();

    void on_Exit_triggered();

    void on_Setting_triggered();

    void on_MapSetting_triggered();

    void on_SaveData_triggered();

    //子窗口显示

    //航线
    void WPCircle(mapcontrol::WayPointItem *Item,mapcontrol::WayPointItem *CircleItem,double Radius);

    void WPdelete(int number);
    void WPdeleteALL(void);
    void WPSetALL(void);
    void WPChanged(int Number,double Lat,double Lng);

    void ShowMousePos(internals::PointLatLng pos);

    void ShowDownloadWaypoint(quint16 CurrentPoint,double Latitude,double Longitude);

    void CleanTrail(bool Flag);

    void SetWPLoop(bool Flag);
    //地图设置

    void SetMapType(QString str);

    void on_MouseDoubleClickEvent(QMouseEvent *event);

    //显示
    void DisplayRemote(quint8 RSSI,
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
                float RelativeHeight,
                quint8 mode1, quint8 mode2, quint8 LockStatus, quint8 TurningStatus, quint8 CurrentStatus,
                quint8 CtrlMode, quint8 AirPlane,quint8 FlightMode,
                quint8 Command1,
                quint8 Command2
               );

    void DisplayEcho(quint8 Echo_CMD1,quint8 Echo_CMD2,quint8 Echo_CMD3,quint8 Echo_CMD4,
                     quint8 Echo_GetRoute,quint8 Echo_Error2,quint8 Echo_Error3,quint8 Echo_Error4);

    void DisplaySenserERR(unsigned char Data);
    //命令按键
    void on_TakeOffButton_clicked();
    void on_TravelButton_clicked();
    void on_RouteButton_clicked();
    void on_LandingButton_clicked();
    void on_BackhomeButton_clicked();
    void on_LockButton_clicked();

    void OrderGroupChange(int number);
    void OrderDotChange(int number);

    void TravelCMD(void);

    //保存设置

    void on_ClearTrail_clicked();

    void on_CutOff_clicked();

    void on_StartTiming_clicked();

    void on_SimulationButton_clicked();

private:
    Ui::MainWindow *ui;
    QextSerialPort *myPort;
    Decode *myDecode;
    QThread *DecodeThread;

    DataSave *datasave;
    QThread *datasaveThread;

    QTimer *SaveTimer;
    int SaveFreq;

    QTimer *TickTimer;
    qint8  Hour,Minute,Second;

    mapcontrol::OPMapWidget *map;
    mapcontrol::WayPointItem *EndItem;//最后一个航点
    internals::PointLatLng   CurrentPoint;//当前点
    QTimer *HeartBeat;


    //定高控制；
    float m_UseSpeed;
    float m_wzSpeed;
    float m_BoardHight;
    float m_Altitude;

    //航线点选择
    int  OrderDotNumber;//要切换的航点
    int   OrderGroupNumber;//要切换的航线组


    //飞机命令状态（0关车，1起飞，2巡航，3航线，4返回，5降落）
    quint8 AirPlaneCMDStatus;
    bool   AirPlaneLockStatus;

    /*quint8 TakeOffStatus;
    quint8 TravelStatus;
    quint8 CircleStatus;
    quint8 LandingStatus;
    quint8 LockStatus;
    quint8 LockStatus;
    quint8 LockStatus;
    quint8 LockStatus;*/


    QSound *AlertSound;
    bool   isOpenSound;

};

#endif // MAINWINDOW_H
