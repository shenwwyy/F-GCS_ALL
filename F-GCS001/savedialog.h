#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include "QDateTime"
#include "QTimer"
#include "QStringList"
#include "QMessageBox"

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 7 };

    explicit SaveDialog(QWidget *parent = 0);
    ~SaveDialog();



signals:
    void isWindowClose(char);
    void SendSaveDir(QString);
    void SendFreq(int);
    void CleanTrail(bool);


    void ShowSignals(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,
                     qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16);
    void ShowStatus(float,float,float,float,float,float);
    void ShowGPS1(quint8,quint8,double,double,float,float,float);
    void ShowAttitude(qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,qint16,float,float,float);
    void ShowRouteInfo(quint8,quint8,quint8,quint8,quint8,quint16,float,float,float,float,float,float,qint16,qint16,qint16,qint16,float,float,float);



private slots:
    void on_checkBox_clicked(bool checked);

    void on_SelectButton_clicked();

    void on_ReplaySelectButton_clicked();

    void on_StartReplayButton_clicked();


    void ReplayData(void);

    void on_Pause_clicked();

    void on_PathSelectButton_clicked();

    void on_CleanTrialButton_clicked();

private:
    Ui::SaveDialog *ui;
    QString FileDir;
    QString DefaultFileDir;

    QString Syear,Smonth,Sday;

    QTimer *ReplayTimer;
    quint64 ReplayCount;
        qint32 Frequency;

    QStringList ReplayList;
    QStringList ReplayItem;


};

#endif // SAVEDIALOG_H
