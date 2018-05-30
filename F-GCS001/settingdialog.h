#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "QtOpenGL"
#include "QWidget"
#include "QtGui/qopengl.h"
#include "QGLWidget"
#include "qopenglfunctions_3_3_core.h"
#include "calibration.h"
#include "glwidget.h"


namespace Ui {
class SettingDialog;
}


class SettingDialog : public QDialog,protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    enum WindowsId{ ID = 3 };

    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();



signals:
    void isWindowClose(char);
    void SendCMD(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);
    void SendPID(quint8,float,float,float,float,float,float,
                        float,float,float,float,float,float,
                        float,float,float,float,float,float);

    void SendScale(quint8,float,float,float,float,float,float);

    void SendServoPolar(quint8,
                        quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,
                        quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);

    void SendServoPolynomial(quint8,float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float,
                                     float,float,float);


private Q_SLOTS:

    void ShowPID(quint8 ID,
                  float PID1_P,float PID1_I,float PID1_D,
                  float PID2_P,float PID2_I,float PID2_D,
                  float PID3_P,float PID3_I,float PID3_D,
                  float PID4_P,float PID4_I,float PID4_D,
                  float PID5_P,float PID5_I,float PID5_D,
                  float PID6_P,float PID6_I,float PID6_D);

    void ShowCalibration(quint8 SenserType,float x,float y,float z);

    void ShowAttitude(float Roll,float Pitch ,float Yaw);



    void ShowServoPolar(qint8 polar1,qint8 polar2,qint8 polar3,qint8 polar4,
                        qint8 polar5,qint8 polar6,qint8 polar7,qint8 polar8,
                        qint8 polar9,qint8 polar10,qint8 polar11,qint8 polar12,
                        qint8 polar13,qint8 polar14,qint8 polar15,qint8 polar16,
                        qint8 Freq1,qint8 Freq2,qint8 Freq3,qint8 Freq4);


    void ShowServoPoly(float PWM1_A,float PWM1_B,float PWM1_C,
                       float PWM2_A,float PWM2_B,float PWM2_C,
                       float PWM3_A,float PWM3_B,float PWM3_C,
                       float PWM4_A,float PWM4_B,float PWM4_C,
                       float PWM5_A,float PWM5_B,float PWM5_C,
                       float PWM6_A,float PWM6_B,float PWM6_C,
                       float PWM7_A,float PWM7_B,float PWM7_C,
                       float PWM8_A,float PWM8_B,float PWM8_C,
                       float PWM9_A,float PWM9_B,float PWM9_C,
                       float PWM10_A,float PWM10_B,float PWM10_C,
                       float PWM11_A,float PWM11_B,float PWM11_C,
                       float PWM12_A,float PWM12_B,float PWM12_C,
                       float PWM13_A,float PWM13_B,float PWM13_C,
                       float PWM14_A,float PWM14_B,float PWM14_C,
                       float PWM15_A,float PWM15_B,float PWM15_C,
                       float PWM16_A,float PWM16_B,float PWM16_C,
                       float ROTOR1_A,float ROTOR1_B,float ROTOR1_C,
                       float ROTOR2_A,float ROTOR2_B,float ROTOR2_C,
                       float ROTOR3_A,float ROTOR3_B,float ROTOR3_C,
                       float ROTOR4_A,float ROTOR4_B,float ROTOR4_C,
                       float ROTOR5_A,float ROTOR5_B,float ROTOR5_C);


    void on_Rotor_Inside_Read_Button_clicked();

    void on_Fixwing_Inside_Read_Button_clicked();

    void on_Rotor_Outsde_Read_Button_clicked();

    void on_Fixwing_Outsde_Read_Button_clicked();

    void on_Rotor_Inside_Write_Button_clicked();

    void on_Fixwing_Inside_Write_Button_clicked();

    void on_Rotor_Outsde_Write_Button_clicked();

    void on_Fixwing_Outsde_Write_Button_clicked();



    void on_StartCalibration_clicked();

    void on_CompleteCalibration_clicked();

    void on_WriteToFCButton_clicked();

    void on_Polar_ChangeButton_clicked();

    void on_Polar_ReadButton_clicked();

    void on_Polynomial_ReadButton_clicked();

    void on_Polynomial_ChangeButton_clicked();

    void on_Polynomial_SaveButton_clicked();

    void on_Polar_SaveButton_clicked();

private:
    Ui::SettingDialog *ui;
    Calibration *Calib;
    quint64 RawDataCount;
    float RawData[3][8640000];//这个数组的大小可以校准100Hz频率数据 24小时

};

#endif // SETTINGDIALOG_H
