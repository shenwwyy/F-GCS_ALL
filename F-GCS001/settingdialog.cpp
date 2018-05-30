#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "QDebug"
#include <QOpenGLWidget>
#include "calibration.h"
#include <QtGui>
#include <QtCore>
#include <QtOpenGL>
#include "GL/gl.h"
#include "glwidget.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    //初始化 《PID》
    ui->Fixwing_Outsde_Write_Button->setEnabled(false);
    ui->Fixwing_Inside_Write_Button->setEnabled(false);
    ui->Rotor_Outsde_Write_Button->setEnabled(false);
    ui->Rotor_Inside_Write_Button->setEnabled(false);

    ui->Fixwing_Outside_Save_Button->setEnabled(false);
    ui->Fixwing_Inside_Save_Button->setEnabled(false);
    ui->Rotor_Outside_Save_Button->setEnabled(false);
    ui->Rotor_Inside_Save_Button->setEnabled(false);



    //初始化 《传感器校准》
    ui->SenserBox->addItem("主陀螺",0x01);
    ui->SenserBox->addItem("主加表",0x02);
    ui->SenserBox->addItem("主罗盘",0x03);
    ui->SenserBox->addItem("备用陀螺",0x04);
    ui->SenserBox->addItem("备用加表",0x05);
    ui->SenserBox->addItem("备用罗盘",0x06);
    ui->SenserBox->addItem("主气压计",0x07);
    RawDataCount = 1;

    Calib = new Calibration();

    ui->Move_checkBox->setChecked(false);



    //初始化《输出设置》
    ui->Polar_PWM1->addItem("正向PWM",1);
    ui->Polar_PWM1->addItem("反向PWM",-1);
    ui->Polar_PWM1->addItem("数字输出",2);

    ui->Polar_PWM2->addItem("正向PWM",1);
    ui->Polar_PWM2->addItem("反向PWM",-1);
    ui->Polar_PWM2->addItem("数字输出",2);

    ui->Polar_PWM3->addItem("正向PWM",1);
    ui->Polar_PWM3->addItem("反向PWM",-1);
    ui->Polar_PWM3->addItem("数字输出",2);

    ui->Polar_PWM4->addItem("正向PWM",1);
    ui->Polar_PWM4->addItem("反向PWM",-1);
    ui->Polar_PWM4->addItem("数字输出",2);

    ui->Polar_PWM5->addItem("正向PWM",1);
    ui->Polar_PWM5->addItem("反向PWM",-1);
    ui->Polar_PWM5->addItem("数字输出",2);

    ui->Polar_PWM6->addItem("正向PWM",1);
    ui->Polar_PWM6->addItem("反向PWM",-1);
    ui->Polar_PWM6->addItem("数字输出",2);

    ui->Polar_PWM7->addItem("正向PWM",1);
    ui->Polar_PWM7->addItem("反向PWM",-1);
    ui->Polar_PWM7->addItem("数字输出",2);

    ui->Polar_PWM8->addItem("正向PWM",1);
    ui->Polar_PWM8->addItem("反向PWM",-1);
    ui->Polar_PWM8->addItem("数字输出",2);

    ui->Polar_PWM9->addItem("正向PWM",1);
    ui->Polar_PWM9->addItem("反向PWM",-1);
    ui->Polar_PWM9->addItem("数字输出",2);

    ui->Polar_PWM10->addItem("正向PWM",1);
    ui->Polar_PWM10->addItem("反向PWM",-1);
    ui->Polar_PWM10->addItem("数字输出",2);

    ui->Polar_PWM11->addItem("正向PWM",1);
    ui->Polar_PWM11->addItem("反向PWM",-1);
    ui->Polar_PWM11->addItem("数字输出",2);

    ui->Polar_PWM12->addItem("正向PWM",1);
    ui->Polar_PWM12->addItem("反向PWM",-1);
    ui->Polar_PWM12->addItem("数字输出",2);

    ui->Polar_PWM13->addItem("正向PWM",1);
    ui->Polar_PWM13->addItem("反向PWM",-1);
    ui->Polar_PWM13->addItem("数字输出",2);

    ui->Polar_PWM14->addItem("正向PWM",1);
    ui->Polar_PWM14->addItem("反向PWM",-1);
    ui->Polar_PWM14->addItem("数字输出",2);

    ui->Polar_PWM15->addItem("正向PWM",1);
    ui->Polar_PWM15->addItem("反向PWM",-1);
    ui->Polar_PWM15->addItem("数字输出",2);

    ui->Polar_PWM16->addItem("正向PWM",1);
    ui->Polar_PWM16->addItem("反向PWM",-1);
    ui->Polar_PWM16->addItem("数字输出",2);

    ui->GroupFrequency1->addItem("50Hz",5);
    ui->GroupFrequency1->addItem("100Hz",10);
    ui->GroupFrequency1->addItem("200Hz",20);
    ui->GroupFrequency1->addItem("400Hz",40);

    ui->GroupFrequency2->addItem("50Hz",5);
    ui->GroupFrequency2->addItem("100Hz",10);
    ui->GroupFrequency2->addItem("200Hz",20);
    ui->GroupFrequency2->addItem("400Hz",40);

    ui->GroupFrequency3->addItem("50Hz",5);
    ui->GroupFrequency3->addItem("100Hz",10);
    ui->GroupFrequency3->addItem("200Hz",20);
    ui->GroupFrequency3->addItem("400Hz",40);

    ui->GroupFrequency4->addItem("50Hz",5);
    ui->GroupFrequency4->addItem("100Hz",10);
    ui->GroupFrequency4->addItem("200Hz",20);
    ui->GroupFrequency4->addItem("400Hz",40);

    //初始化 《输出矫正》
    ui->Polar_ChangeButton->setEnabled(false);
    ui->Polar_SaveButton->setEnabled(false);

    //初始化 《多项式》
    ui->Polynomial_ChangeButton->setEnabled(false);
    ui->Polynomial_SaveButton->setEnabled(false);
}

SettingDialog::~SettingDialog()
{
    emit isWindowClose(ID);

    quint8 Senser;
    Senser = ui->SenserBox->currentData().toInt();
    Senser = Senser<<4;
    //qDebug() << Senser;
    emit SendCMD(0x02,0,0,0,0,0,Senser,0,0);

    delete Calib;
    delete ui;
}

void SettingDialog::ShowPID(quint8 ID,
                             float PID1_P, float PID1_I, float PID1_D,
                             float PID2_P, float PID2_I, float PID2_D,
                             float PID3_P, float PID3_I, float PID3_D,
                             float PID4_P, float PID4_I, float PID4_D,
                             float PID5_P, float PID5_I, float PID5_D,
                             float PID6_P, float PID6_I, float PID6_D)
{
      switch(ID)
      {
      case 0x31:
      {
           ui->Rotor_Inside_Gyro_Roll_P_Box->setValue(PID1_P);
           ui->Rotor_Inside_Gyro_Roll_I_Box->setValue(PID1_I);
           ui->Rotor_Inside_Gyro_Roll_D_Box->setValue(PID1_D);
           ui->Rotor_Inside_Gyro_Pitch_P_Box->setValue(PID2_P);
           ui->Rotor_Inside_Gyro_Pitch_I_Box->setValue(PID2_I);
           ui->Rotor_Inside_Gyro_Pitch_D_Box->setValue(PID2_D);
           ui->Rotor_Inside_Gyro_Yaw_P_Box->setValue(PID3_P);
           ui->Rotor_Inside_Gyro_Yaw_I_Box->setValue(PID3_I);
           ui->Rotor_Inside_Gyro_Yaw_D_Box->setValue(PID3_D);
           ui->Rotor_Inside_Angle_Roll_P_Box->setValue(PID4_P);
           ui->Rotor_Inside_Angle_Roll_I_Box->setValue(PID4_I);
           ui->Rotor_Inside_Angle_Roll_D_Box->setValue(PID4_D);
           ui->Rotor_Inside_Angle_Pitch_P_Box->setValue(PID5_P);
           ui->Rotor_Inside_Angle_Pitch_I_Box->setValue(PID5_I);
           ui->Rotor_Inside_Angle_Pitch_D_Box->setValue(PID5_D);
           ui->Rotor_Inside_Angle_Yaw_P_Box->setValue(PID6_P);
           ui->Rotor_Inside_Angle_Yaw_I_Box->setValue(PID6_I);
           ui->Rotor_Inside_Angle_Yaw_D_Box->setValue(PID6_D);
           ui->Rotor_Inside_Write_Button->setEnabled(true);
           ui->Rotor_Inside_Save_Button->setEnabled(true);
      }break;
      case 0x33:
      {
           ui->Rotor_Outside_Speed_X_P_Box->setValue(PID1_P);
           ui->Rotor_Outside_Speed_X_I_Box->setValue(PID1_I);
           ui->Rotor_Outside_Speed_X_D_Box->setValue(PID1_D);
           ui->Rotor_Outside_Speed_Y_P_Box->setValue(PID2_P);
           ui->Rotor_Outside_Speed_Y_I_Box->setValue(PID2_I);
           ui->Rotor_Outside_Speed_Y_D_Box->setValue(PID2_D);
           ui->Rotor_Outside_Speed_Z_P_Box->setValue(PID3_P);
           ui->Rotor_Outside_Speed_Z_I_Box->setValue(PID3_I);
           ui->Rotor_Outside_Speed_Z_D_Box->setValue(PID3_D);
           ui->Rotor_Outside_Position_X_P_Box->setValue(PID4_P);
           ui->Rotor_Outside_Position_X_I_Box->setValue(PID4_I);
           ui->Rotor_Outside_Position_X_D_Box->setValue(PID4_D);
           ui->Rotor_Outside_Position_Y_P_Box->setValue(PID5_P);
           ui->Rotor_Outside_Position_Y_I_Box->setValue(PID5_I);
           ui->Rotor_Outside_Position_Y_D_Box->setValue(PID5_D);
           ui->Rotor_Outside_Position_Z_P_Box->setValue(PID6_P);
           ui->Rotor_Outside_Position_Z_I_Box->setValue(PID6_I);
           ui->Rotor_Outside_Position_Z_D_Box->setValue(PID6_D);
           ui->Rotor_Outsde_Write_Button->setEnabled(true);
           ui->Rotor_Outside_Save_Button->setEnabled(true);
      }break;
      case 0x32:
      {
           ui->Fixwing_Inside_Gyro_Roll_P_Box->setValue(PID1_P);
           ui->Fixwing_Inside_Gyro_Roll_I_Box->setValue(PID1_I);
           ui->Fixwing_Inside_Gyro_Roll_D_Box->setValue(PID1_D);
           ui->Fixwing_Inside_Gyro_Pitch_P_Box->setValue(PID2_P);
           ui->Fixwing_Inside_Gyro_Pitch_I_Box->setValue(PID2_I);
           ui->Fixwing_Inside_Gyro_Pitch_D_Box->setValue(PID2_D);
           ui->Fixwing_Inside_Gyro_Yaw_P_Box->setValue(PID3_P);
           ui->Fixwing_Inside_Gyro_Yaw_I_Box->setValue(PID3_I);
           ui->Fixwing_Inside_Gyro_Yaw_D_Box->setValue(PID3_D);
           ui->Fixwing_Inside_Angle_Roll_P_Box->setValue(PID4_P);
           ui->Fixwing_Inside_Angle_Roll_I_Box->setValue(PID4_I);
           ui->Fixwing_Inside_Angle_Roll_D_Box->setValue(PID4_D);
           ui->Fixwing_Inside_Angle_Pitch_P_Box->setValue(PID5_P);
           ui->Fixwing_Inside_Angle_Pitch_I_Box->setValue(PID5_I);
           ui->Fixwing_Inside_Angle_Pitch_D_Box->setValue(PID5_D);
           ui->Fixwing_Inside_Angle_Yaw_P_Box->setValue(PID6_P);
           ui->Fixwing_Inside_Angle_Yaw_I_Box->setValue(PID6_I);
           ui->Fixwing_Inside_Angle_Yaw_D_Box->setValue(PID6_D);
           ui->Fixwing_Inside_Write_Button->setEnabled(true);
           ui->Fixwing_Inside_Save_Button->setEnabled(true);
      }break;
      case 0x34:
      {
          ui->Fixwing_Outside_FlightSpeed_P_Box->setValue(PID1_P);
          ui->Fixwing_Outside_FlightSpeed_I_Box->setValue(PID1_I);
          ui->Fixwing_Outside_FlightSpeed_D_Box->setValue(PID1_D);
          ui->Fixwing_Outside_FlightHight_P_Box->setValue(PID2_P);
          ui->Fixwing_Outside_FlightHight_I_Box->setValue(PID2_I);
          ui->Fixwing_Outside_FlightHight_D_Box->setValue(PID2_D);
          ui->Fixwing_Outside_Distance_P_Box->setValue(PID3_P);
          ui->Fixwing_Outside_Distance_I_Box->setValue(PID3_I);
          ui->Fixwing_Outside_Distance_D_Box->setValue(PID3_D);
          ui->Fixwing_Outside_Angle_P_Box->setValue(PID4_P);
          ui->Fixwing_Outside_Angle_I_Box->setValue(PID4_I);
          ui->Fixwing_Outside_Angle_D_Box->setValue(PID4_D);
          ui->doubleSpinBox_66->setValue(PID5_P);
          ui->doubleSpinBox_67->setValue(PID5_I);
          ui->doubleSpinBox_68->setValue(PID5_D);
          ui->doubleSpinBox_69->setValue(PID6_P);
          ui->doubleSpinBox_70->setValue(PID6_I);
          ui->doubleSpinBox_71->setValue(PID6_D);
          ui->Fixwing_Outsde_Write_Button->setEnabled(true);
          ui->Fixwing_Outside_Save_Button->setEnabled(true);
      }break;

      }

}




void SettingDialog::on_Rotor_Inside_Read_Button_clicked()
{
    emit SendCMD(2,0x01,0,0,0,0,0,0,0);
}

void SettingDialog::on_Fixwing_Inside_Read_Button_clicked()
{
    emit SendCMD(2,0x04,0,0,0,0,0,0,0);
}

void SettingDialog::on_Rotor_Outsde_Read_Button_clicked()
{
     emit SendCMD(2,0x02,0,0,0,0,0,0,0);
}

void SettingDialog::on_Fixwing_Outsde_Read_Button_clicked()
{
     emit SendCMD(2,0x08,0,0,0,0,0,0,0);
}

void SettingDialog::on_Rotor_Inside_Write_Button_clicked()
{
  emit SendPID(1,ui->Rotor_Inside_Gyro_Roll_P_Box->value(),
                ui->Rotor_Inside_Gyro_Roll_I_Box->value(),
                ui->Rotor_Inside_Gyro_Roll_D_Box->value(),
                ui->Rotor_Inside_Gyro_Pitch_P_Box->value(),
                ui->Rotor_Inside_Gyro_Pitch_I_Box->value(),
                ui->Rotor_Inside_Gyro_Pitch_D_Box->value(),
                ui->Rotor_Inside_Gyro_Yaw_P_Box->value(),
                ui->Rotor_Inside_Gyro_Yaw_I_Box->value(),
                ui->Rotor_Inside_Gyro_Yaw_D_Box->value(),
                ui->Rotor_Inside_Angle_Roll_P_Box->value(),
                ui->Rotor_Inside_Angle_Roll_I_Box->value(),
                ui->Rotor_Inside_Angle_Roll_D_Box->value(),
                ui->Rotor_Inside_Angle_Pitch_P_Box->value(),
                ui->Rotor_Inside_Angle_Pitch_I_Box->value(),
                ui->Rotor_Inside_Angle_Pitch_D_Box->value(),
                ui->Rotor_Inside_Angle_Yaw_P_Box->value(),
                ui->Rotor_Inside_Angle_Yaw_I_Box->value(),
                ui->Rotor_Inside_Angle_Yaw_D_Box->value());
}

void SettingDialog::on_Fixwing_Inside_Write_Button_clicked()
{
    emit SendPID(2,ui->Fixwing_Inside_Gyro_Roll_P_Box->value(),
                  ui->Fixwing_Inside_Gyro_Roll_I_Box->value(),
                  ui->Fixwing_Inside_Gyro_Roll_D_Box->value(),
                  ui->Fixwing_Inside_Gyro_Pitch_P_Box->value(),
                  ui->Fixwing_Inside_Gyro_Pitch_I_Box->value(),
                  ui->Fixwing_Inside_Gyro_Pitch_D_Box->value(),
                  ui->Fixwing_Inside_Gyro_Yaw_P_Box->value(),
                  ui->Fixwing_Inside_Gyro_Yaw_I_Box->value(),
                  ui->Fixwing_Inside_Gyro_Yaw_D_Box->value(),
                  ui->Fixwing_Inside_Angle_Roll_P_Box->value(),
                  ui->Fixwing_Inside_Angle_Roll_I_Box->value(),
                  ui->Fixwing_Inside_Angle_Roll_D_Box->value(),
                  ui->Fixwing_Inside_Angle_Pitch_P_Box->value(),
                  ui->Fixwing_Inside_Angle_Pitch_I_Box->value(),
                  ui->Fixwing_Inside_Angle_Pitch_D_Box->value(),
                  ui->Fixwing_Inside_Angle_Yaw_P_Box->value(),
                  ui->Fixwing_Inside_Angle_Yaw_I_Box->value(),
                  ui->Fixwing_Inside_Angle_Yaw_D_Box->value());
}

void SettingDialog::on_Rotor_Outsde_Write_Button_clicked()
{
    emit SendPID(3,ui->Rotor_Outside_Speed_X_P_Box->value(),
                  ui->Rotor_Outside_Speed_X_I_Box->value(),
                  ui->Rotor_Outside_Speed_X_D_Box->value(),
                  ui->Rotor_Outside_Speed_Y_P_Box->value(),
                  ui->Rotor_Outside_Speed_Y_I_Box->value(),
                  ui->Rotor_Outside_Speed_Y_D_Box->value(),
                  ui->Rotor_Outside_Speed_Z_P_Box->value(),
                  ui->Rotor_Outside_Speed_Z_I_Box->value(),
                  ui->Rotor_Outside_Speed_Z_D_Box->value(),
                  ui->Rotor_Outside_Position_X_P_Box->value(),
                  ui->Rotor_Outside_Position_X_I_Box->value(),
                  ui->Rotor_Outside_Position_X_D_Box->value(),
                  ui->Rotor_Outside_Position_Y_P_Box->value(),
                  ui->Rotor_Outside_Position_Y_I_Box->value(),
                  ui->Rotor_Outside_Position_Y_D_Box->value(),
                  ui->Rotor_Outside_Position_Z_P_Box->value(),
                  ui->Rotor_Outside_Position_Z_I_Box->value(),
                  ui->Rotor_Outside_Position_Z_D_Box->value());
        //qDebug() << ui->Rotor_Outside_Position_Z_D_Box->value()*1000;
}

void SettingDialog::on_Fixwing_Outsde_Write_Button_clicked()
{
    emit SendPID(4,ui->Fixwing_Outside_FlightSpeed_P_Box->value(),
                  ui->Fixwing_Outside_FlightSpeed_I_Box->value(),
                  ui->Fixwing_Outside_FlightSpeed_D_Box->value(),
                  ui->Fixwing_Outside_FlightHight_P_Box->value(),
                  ui->Fixwing_Outside_FlightHight_I_Box->value(),
                  ui->Fixwing_Outside_FlightHight_D_Box->value(),
                  ui->Fixwing_Outside_Distance_P_Box->value(),
                  ui->Fixwing_Outside_Distance_I_Box->value(),
                  ui->Fixwing_Outside_Distance_D_Box->value(),
                  ui->Fixwing_Outside_Angle_P_Box->value(),
                  ui->Fixwing_Outside_Angle_I_Box->value(),
                  ui->Fixwing_Outside_Angle_D_Box->value(),
                  ui->doubleSpinBox_66->value(),
                  ui->doubleSpinBox_67->value(),
                  ui->doubleSpinBox_68->value(),
                  ui->doubleSpinBox_69->value(),
                  ui->doubleSpinBox_70->value(),
                  ui->doubleSpinBox_71->value());
}


void SettingDialog::on_StartCalibration_clicked()
{
    quint8 Senser;
    Senser = ui->SenserBox->currentData().toInt();
    //qDebug() << Senser;
    emit SendCMD(0x02,0,0,0,0,0,Senser&0x0f,0,0);
}

void SettingDialog::on_CompleteCalibration_clicked()
{
    quint8 Senser;

    float BiasX,BiasY,BiasZ,ScaleX,ScaleY,ScaleZ;

    Senser = ui->SenserBox->currentData().toInt();
    Senser = Senser<<4;
    //qDebug() << Senser;
    emit SendCMD(0x02,0,0,0,0,0,Senser,0,0);
    ui->glwidget->CalOffset(&BiasX,&BiasY,&BiasZ,&ScaleX,&ScaleY,&ScaleZ);

    ui->BiasX_lineEdit->setText(QString::number(BiasX));
    ui->BiasY_lineEdit->setText(QString::number(BiasY));
    ui->BiasZ_lineEdit->setText(QString::number(BiasZ));

    ui->ScaleX_lineEdit->setText(QString::number(ScaleX));
    ui->ScaleY_lineEdit->setText(QString::number(ScaleY));
    ui->ScaleZ_lineEdit->setText(QString::number(ScaleZ));
}

void SettingDialog::ShowCalibration(quint8 SenserType,float x,float y,float z)
{
    //qDebug() << "ok";

    if(ui->SenserBox->currentData().toInt() == SenserType)
    {
       Calib->CalcData_Input(x,y,z);

       ui->glwidget->setValue(x,y,z);
    }
}

void SettingDialog::ShowAttitude(float Roll, float Pitch, float Yaw)
{
    if(ui->Move_checkBox->isChecked())
       ui->glwidget->setRotation(Pitch,Roll,Yaw);
}

void SettingDialog::on_WriteToFCButton_clicked()
{
   emit SendScale(ui->SenserBox->currentData().toInt(),
                  ui->BiasX_lineEdit->text().toDouble(),
                  ui->BiasY_lineEdit->text().toDouble(),
                  ui->BiasZ_lineEdit->text().toDouble(),
                  ui->ScaleX_lineEdit->text().toDouble(),
                  ui->ScaleY_lineEdit->text().toDouble(),
                  ui->ScaleZ_lineEdit->text().toDouble());
}

//极性设置

void SettingDialog::ShowServoPolar(qint8 polar1, qint8 polar2, qint8 polar3, qint8 polar4, qint8 polar5, qint8 polar6, qint8 polar7, qint8 polar8, qint8 polar9, qint8 polar10, qint8 polar11, qint8 polar12, qint8 polar13, qint8 polar14, qint8 polar15, qint8 polar16, qint8 Freq1, qint8 Freq2, qint8 Freq3, qint8 Freq4)
{
    ui->Polar_PWM1->setCurrentIndex(ui->Polar_PWM1->findData(polar1));
    ui->Polar_PWM2->setCurrentIndex(ui->Polar_PWM2->findData(polar2));
    ui->Polar_PWM3->setCurrentIndex(ui->Polar_PWM3->findData(polar3));
    ui->Polar_PWM4->setCurrentIndex(ui->Polar_PWM4->findData(polar4));
    ui->Polar_PWM5->setCurrentIndex(ui->Polar_PWM5->findData(polar5));
    ui->Polar_PWM6->setCurrentIndex(ui->Polar_PWM6->findData(polar6));
    ui->Polar_PWM7->setCurrentIndex(ui->Polar_PWM7->findData(polar7));
    ui->Polar_PWM8->setCurrentIndex(ui->Polar_PWM8->findData(polar8));
    ui->Polar_PWM9->setCurrentIndex(ui->Polar_PWM9->findData(polar9));
    ui->Polar_PWM10->setCurrentIndex(ui->Polar_PWM10->findData(polar10));
    ui->Polar_PWM11->setCurrentIndex(ui->Polar_PWM11->findData(polar11));
    ui->Polar_PWM12->setCurrentIndex(ui->Polar_PWM12->findData(polar12));
    ui->Polar_PWM13->setCurrentIndex(ui->Polar_PWM13->findData(polar13));
    ui->Polar_PWM14->setCurrentIndex(ui->Polar_PWM14->findData(polar14));
    ui->Polar_PWM15->setCurrentIndex(ui->Polar_PWM15->findData(polar15));
    ui->Polar_PWM16->setCurrentIndex(ui->Polar_PWM16->findData(polar16));
    ui->GroupFrequency1->setCurrentIndex(ui->GroupFrequency1->findData(Freq1));
    ui->GroupFrequency2->setCurrentIndex(ui->GroupFrequency2->findData(Freq2));
    ui->GroupFrequency3->setCurrentIndex(ui->GroupFrequency3->findData(Freq3));
    ui->GroupFrequency4->setCurrentIndex(ui->GroupFrequency4->findData(Freq4));

    ui->Polar_ChangeButton->setEnabled(true);
    ui->Polar_SaveButton->setEnabled(true);
}


void SettingDialog::on_Polar_ChangeButton_clicked()
{
    emit SendServoPolar(0x01,
                        ui->Polar_PWM1->currentData().toInt(),ui->Polar_PWM2->currentData().toInt(),
                        ui->Polar_PWM3->currentData().toInt(),ui->Polar_PWM4->currentData().toInt(),
                        ui->Polar_PWM5->currentData().toInt(),ui->Polar_PWM6->currentData().toInt(),
                        ui->Polar_PWM7->currentData().toInt(),ui->Polar_PWM8->currentData().toInt(),
                        ui->Polar_PWM9->currentData().toInt(),ui->Polar_PWM10->currentData().toInt(),
                        ui->Polar_PWM11->currentData().toInt(),ui->Polar_PWM12->currentData().toInt(),
                        ui->Polar_PWM13->currentData().toInt(),ui->Polar_PWM14->currentData().toInt(),
                        ui->Polar_PWM15->currentData().toInt(),ui->Polar_PWM16->currentData().toInt(),
                        ui->GroupFrequency1->currentData().toInt(),ui->GroupFrequency2->currentData().toInt(),
                        ui->GroupFrequency3->currentData().toInt(),ui->GroupFrequency4->currentData().toInt());
}

void SettingDialog::on_Polar_ReadButton_clicked()
{
    emit SendCMD(2,0,0x01,0,0,0,0,0,0);//查询极性
}

void SettingDialog::on_Polar_SaveButton_clicked()
{
    emit SendCMD(2,0,0x10,0,0,0,0,0,0);//保存极性设置
}


//输出矫正


void SettingDialog::ShowServoPoly(float PWM1_A, float PWM1_B, float PWM1_C,
                                  float PWM2_A, float PWM2_B, float PWM2_C,
                                  float PWM3_A, float PWM3_B, float PWM3_C,
                                  float PWM4_A, float PWM4_B, float PWM4_C,
                                  float PWM5_A, float PWM5_B, float PWM5_C,
                                  float PWM6_A, float PWM6_B, float PWM6_C,
                                  float PWM7_A, float PWM7_B, float PWM7_C,
                                  float PWM8_A, float PWM8_B, float PWM8_C,
                                  float PWM9_A, float PWM9_B, float PWM9_C,
                                  float PWM10_A, float PWM10_B, float PWM10_C,
                                  float PWM11_A, float PWM11_B, float PWM11_C,
                                  float PWM12_A, float PWM12_B, float PWM12_C,
                                  float PWM13_A, float PWM13_B, float PWM13_C,
                                  float PWM14_A, float PWM14_B, float PWM14_C,
                                  float PWM15_A, float PWM15_B, float PWM15_C,
                                  float PWM16_A, float PWM16_B, float PWM16_C,
                                  float ROTOR1_A, float ROTOR1_B, float ROTOR1_C,
                                  float ROTOR2_A, float ROTOR2_B, float ROTOR2_C,
                                  float ROTOR3_A, float ROTOR3_B, float ROTOR3_C,
                                  float ROTOR4_A, float ROTOR4_B, float ROTOR4_C,
                                  float ROTOR5_A, float ROTOR5_B, float ROTOR5_C)
{
    ui->Polynomial_A_PWM1->setValue(PWM1_A); ui->Polynomial_B_PWM1->setValue(PWM1_B); ui->Polynomial_C_PWM1->setValue(PWM1_C);
    ui->Polynomial_A_PWM2->setValue(PWM2_A); ui->Polynomial_B_PWM2->setValue(PWM2_B); ui->Polynomial_C_PWM2->setValue(PWM2_C);
    ui->Polynomial_A_PWM3->setValue(PWM3_A); ui->Polynomial_B_PWM3->setValue(PWM3_B); ui->Polynomial_C_PWM3->setValue(PWM3_C);
    ui->Polynomial_A_PWM4->setValue(PWM4_A); ui->Polynomial_B_PWM4->setValue(PWM4_B); ui->Polynomial_C_PWM4->setValue(PWM4_C);
    ui->Polynomial_A_PWM5->setValue(PWM5_A); ui->Polynomial_B_PWM5->setValue(PWM5_B); ui->Polynomial_C_PWM5->setValue(PWM5_C);
    ui->Polynomial_A_PWM6->setValue(PWM6_A); ui->Polynomial_B_PWM6->setValue(PWM6_B); ui->Polynomial_C_PWM6->setValue(PWM6_C);
    ui->Polynomial_A_PWM7->setValue(PWM7_A); ui->Polynomial_B_PWM7->setValue(PWM7_B); ui->Polynomial_C_PWM7->setValue(PWM7_C);
    ui->Polynomial_A_PWM8->setValue(PWM8_A); ui->Polynomial_B_PWM8->setValue(PWM8_B); ui->Polynomial_C_PWM8->setValue(PWM8_C);
    ui->Polynomial_A_PWM9->setValue(PWM9_A); ui->Polynomial_B_PWM9->setValue(PWM9_B); ui->Polynomial_C_PWM9->setValue(PWM9_C);
    ui->Polynomial_A_PWM10->setValue(PWM10_A); ui->Polynomial_B_PWM10->setValue(PWM10_B); ui->Polynomial_C_PWM10->setValue(PWM10_C);
    ui->Polynomial_A_PWM11->setValue(PWM11_A); ui->Polynomial_B_PWM11->setValue(PWM11_B); ui->Polynomial_C_PWM11->setValue(PWM11_C);
    ui->Polynomial_A_PWM12->setValue(PWM12_A); ui->Polynomial_B_PWM12->setValue(PWM12_B); ui->Polynomial_C_PWM12->setValue(PWM12_C);
    ui->Polynomial_A_PWM13->setValue(PWM13_A); ui->Polynomial_B_PWM13->setValue(PWM13_B); ui->Polynomial_C_PWM13->setValue(PWM13_C);
    ui->Polynomial_A_PWM14->setValue(PWM14_A); ui->Polynomial_B_PWM14->setValue(PWM14_B); ui->Polynomial_C_PWM14->setValue(PWM14_C);
    ui->Polynomial_A_PWM15->setValue(PWM15_A); ui->Polynomial_B_PWM15->setValue(PWM15_B); ui->Polynomial_C_PWM15->setValue(PWM15_C);
    ui->Polynomial_A_PWM16->setValue(PWM16_A); ui->Polynomial_B_PWM16->setValue(PWM16_B); ui->Polynomial_C_PWM16->setValue(PWM16_C);
    ui->Polynomial_A_ROTOR1->setValue(ROTOR1_A); ui->Polynomial_B_ROTOR1->setValue(ROTOR1_B); ui->Polynomial_C_ROTOR1->setValue(ROTOR1_C);
    ui->Polynomial_A_ROTOR2->setValue(ROTOR2_A); ui->Polynomial_B_ROTOR2->setValue(ROTOR2_B); ui->Polynomial_C_ROTOR2->setValue(ROTOR2_C);
    ui->Polynomial_A_ROTOR3->setValue(ROTOR3_A); ui->Polynomial_B_ROTOR3->setValue(ROTOR3_B); ui->Polynomial_C_ROTOR3->setValue(ROTOR3_C);
    ui->Polynomial_A_ROTOR4->setValue(ROTOR4_A); ui->Polynomial_B_ROTOR4->setValue(ROTOR4_B); ui->Polynomial_C_ROTOR4->setValue(ROTOR4_C);
    ui->Polynomial_A_ROTOR5->setValue(ROTOR5_A); ui->Polynomial_B_ROTOR5->setValue(ROTOR5_B); ui->Polynomial_C_ROTOR5->setValue(ROTOR5_C);

    ui->Polynomial_ChangeButton->setEnabled(true);
    ui->Polynomial_SaveButton->setEnabled(true);
}




void SettingDialog::on_Polynomial_ChangeButton_clicked()
{
     emit SendServoPolynomial(0x01,ui->Polynomial_A_PWM1->value(),ui->Polynomial_B_PWM1->value(),ui->Polynomial_C_PWM1->value(),
                                   ui->Polynomial_A_PWM2->value(),ui->Polynomial_B_PWM2->value(),ui->Polynomial_C_PWM2->value(),
                                   ui->Polynomial_A_PWM3->value(),ui->Polynomial_B_PWM3->value(),ui->Polynomial_C_PWM3->value(),
                                   ui->Polynomial_A_PWM4->value(),ui->Polynomial_B_PWM4->value(),ui->Polynomial_C_PWM4->value(),
                                   ui->Polynomial_A_PWM5->value(),ui->Polynomial_B_PWM5->value(),ui->Polynomial_C_PWM5->value(),
                                   ui->Polynomial_A_PWM6->value(),ui->Polynomial_B_PWM6->value(),ui->Polynomial_C_PWM6->value(),
                                   ui->Polynomial_A_PWM7->value(),ui->Polynomial_B_PWM7->value(),ui->Polynomial_C_PWM7->value(),
                                   ui->Polynomial_A_PWM8->value(),ui->Polynomial_B_PWM8->value(),ui->Polynomial_C_PWM8->value(),
                                   ui->Polynomial_A_PWM9->value(),ui->Polynomial_B_PWM9->value(),ui->Polynomial_C_PWM9->value(),
                                   ui->Polynomial_A_PWM10->value(),ui->Polynomial_B_PWM10->value(),ui->Polynomial_C_PWM10->value(),
                                   ui->Polynomial_A_PWM11->value(),ui->Polynomial_B_PWM11->value(),ui->Polynomial_C_PWM11->value(),
                                   ui->Polynomial_A_PWM12->value(),ui->Polynomial_B_PWM12->value(),ui->Polynomial_C_PWM12->value(),
                                   ui->Polynomial_A_PWM13->value(),ui->Polynomial_B_PWM13->value(),ui->Polynomial_C_PWM13->value(),
                                   ui->Polynomial_A_PWM14->value(),ui->Polynomial_B_PWM14->value(),ui->Polynomial_C_PWM14->value(),
                                   ui->Polynomial_A_PWM15->value(),ui->Polynomial_B_PWM15->value(),ui->Polynomial_C_PWM15->value(),
                                   ui->Polynomial_A_PWM16->value(),ui->Polynomial_B_PWM16->value(),ui->Polynomial_C_PWM16->value(),
                                   ui->Polynomial_A_ROTOR1->value(),ui->Polynomial_B_ROTOR1->value(),ui->Polynomial_C_ROTOR1->value(),
                                   ui->Polynomial_A_ROTOR2->value(),ui->Polynomial_B_ROTOR2->value(),ui->Polynomial_C_ROTOR2->value(),
                                   ui->Polynomial_A_ROTOR3->value(),ui->Polynomial_B_ROTOR3->value(),ui->Polynomial_C_ROTOR3->value(),
                                   ui->Polynomial_A_ROTOR4->value(),ui->Polynomial_B_ROTOR4->value(),ui->Polynomial_C_ROTOR4->value(),
                                   ui->Polynomial_A_ROTOR5->value(),ui->Polynomial_B_ROTOR5->value(),ui->Polynomial_C_ROTOR5->value());
}

void SettingDialog::on_Polynomial_ReadButton_clicked()
{
     emit SendCMD(2,0,0x02,0,0,0,0,0,0);//查询输出矫正系数
}

void SettingDialog::on_Polynomial_SaveButton_clicked()
{
     emit SendCMD(2,0,0x20,0,0,0,0,0,0);//保存输出矫正系数
}


