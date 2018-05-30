#ifndef AIRPLANEDIALOG_H
#define AIRPLANEDIALOG_H

#include "QTimer"
#include <QDialog>
#include "inifile.h"

namespace Ui {
class AirPlaneDialog;
}

class AirPlaneDialog : public QDialog
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 2 };

    explicit AirPlaneDialog(QWidget *parent = 0);
    ~AirPlaneDialog();

signals:
    void isWindowClose(char);
    void SendGetVersion(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);

    void SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);
    void SendTaskSetting(quint8,float,float,float,float,float,float,float,float);

private slots:

    void on_TimeOut();

//=======显示
    void ShowTaskSetting(quint8 ID,float A,float B,float C,float D,float E,float F,float G,float H);

    void on_WriteFlightSettingButton_clicked();

    void on_ReadFlightSettingButton_clicked();

    void on_SaveFlightSettingButton_clicked();

private:
    Ui::AirPlaneDialog *ui;
    QTimer *Timer;

    int ServoType;
    int ServoCMD;

    quint8 TaskSettingID;

    float TaskSettingA,TaskSettingB,TaskSettingC,TaskSettingD,TaskSettingE,TaskSettingF,TaskSettingG,TaskSettingH;

    IniFile *inifile;

};

#endif // AIRPLANEDIALOG_H
