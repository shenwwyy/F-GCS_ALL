#ifndef MISSIONDIALOG_H
#define MISSIONDIALOG_H

#include "QTableWidgetItem"
#include <QDialog>
#include "opmapwidget.h"
#include "QFileDialog"
#include "QDateTime"
#include "QStringList"
#include "missionsettingdialog.h"
#include "waypointcsv.h"
#include "inifile.h"

namespace Ui {
class MissionDialog;
}

class MissionDialog : public QDialog
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 4 };

     struct RoutePoint{
         quint16  Numer;
         double   Longitide;
         double   Latitude;
         quint16  Altitude;
         quint16  Radius;
         quint16  Velocity;
         quint8   Action;
     };

    explicit MissionDialog(QWidget *parent = 0);
    ~MissionDialog();



signals:
    void CloseChildrenWindows(bool);
    void isWindowClose(char);
    void LoopChecked(quint16,bool);

    void SetWPLoop(bool);

    void DeletePoint(int Point);
    void DeleteAll(void);
    void SendDot(quint8,quint8,quint16,quint16,double,double,float,quint16,quint16,quint16);

    void SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);
    void ShowDownLoadWayPoint(quint16,double,double);

private slots:

    void ShowEcho(quint8 ID, quint8 A, quint8 B, quint8 C, quint8 D, quint8 E, quint8 F, quint8 G, quint8 H);

    void GetWayPoint(mapcontrol::WayPointItem *Item);
    //void DownLoadPoint(quint8 Groups,quint16 TotalPoint,quint16 CurrentPoint,double latitude,double longitude,quint16 Altitude,quint16 Radius,quint16 Velocity,quint8 Action);
    void ReceiverDot(quint8 Function,quint8 Groups,quint16 TotalPoint,quint16 CurrentPoint,double longitude,double latitude,float Altitude,quint16 Velocity,quint16 Radius,quint16 Action);

    void ChangePoint(int PointNumber, double Latitude, double Longitude);
    void AddPoint(int PointNumber,QString Latitude,QString Longitude);
    void DecPoint(void);
    void on_EditButton_clicked();
    void on_UploadButton_clicked();
    void on_DownloadButton_clicked();
    void on_SaveButton_clicked();
    void on_ReadButton_clicked();
    void on_MoveupButton_clicked();
    void on_MovedownButton_clicked();
    void on_DeleteButton_clicked();
    void on_ClearButton_clicked();

    void SendPoint();

    void on_LoopBox_clicked();

    void RecieveWayPoint(void);

    //=============
    void on_SetHeader(int column);
    void isMissionSettingDialogOpenModify(bool flag);





    void on_InfoTable_cellChanged(int row, int column);

private:
    Ui::MissionDialog *ui;
    QTimer *RouteTimer;
    QTimer *GetRouteTimer;
    qint16 PointNum;
    bool   isGet;
    bool   isChangeOnePoint;

    bool   isMissionSettingDialogOpen;
    MissionSettingDialog *MissionSetting;

    bool isSendDotCompelect;

    WayPointCSV *waypointcsv;
    IniFile     *inifile;

};

#endif // MISSIONDIALOG_H
