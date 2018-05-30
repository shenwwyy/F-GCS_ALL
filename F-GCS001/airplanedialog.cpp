#include "airplanedialog.h"
#include "ui_airplanedialog.h"
#include "QDebug"


AirPlaneDialog::AirPlaneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AirPlaneDialog)
{
    ui->setupUi(this);
    //setFixedSize(200,500);
    //setWindowFlags(Qt::WindowStaysOnTopHint);

    inifile = new IniFile();//初始化ini对象

    Timer = new QTimer;
    //Timer->start(200);

    ServoType = 0;
    ServoCMD  = 0;

    TaskSettingID = 0;


    //失能设置、写入按钮
    ui->WriteFlightSettingButton->setEnabled(false);
    ui->SaveFlightSettingButton->setEnabled(false);

}

AirPlaneDialog::~AirPlaneDialog()
{
    emit isWindowClose(ID);
    if(Timer->isActive())
       delete Timer;
    delete ui;
}


void AirPlaneDialog::ShowTaskSetting(quint8 ID,float A,float B,float C,float D,float E,float F,float G,float H)
{

    switch(0X0F&ID)
    {
        case 1:
        {
                ui->TopHeightBox->setValue(A);
                ui->BottomHeightBox->setValue(B);
                ui->MaxCtrlRadiusBox->setValue(C);
                ui->TakeoffHeightBox->setValue(D);
                ui->StartCircleHieghtBox->setValue(E);
                ui->FirstTimeLimitDistanceBox->setValue(F);
                ui->LandingChangeModeDistatnceBox->setValue(G);
                ui->LandingLowSpeedHeightBox->setValue(H);
        }break;
        case 2:
        {
                ui->CircleDefaultHeightBox->setValue(A);
                ui->CircleDefaultRadiuBox->setValue(B);
                ui->CircleDefaultVelocityBox->setValue(C);
                ui->CircleChangeSpeedBox->setValue(D);
        }break;
        case 3:
        {
        }break;
        case 4:
        {
        }break;
        case 5:
        {
        }break;
        case 6:
        {
        }break;
        default :
        {
        }break;
    }
    //使能设置、写入按钮
    ui->WriteFlightSettingButton->setEnabled(true);
    ui->SaveFlightSettingButton->setEnabled(true);
}


void AirPlaneDialog::on_WriteFlightSettingButton_clicked()
{
    connect(Timer,SIGNAL(timeout()),
            this,SLOT(on_TimeOut()));

    TaskSettingID = 1;
    Timer->start(100);//启动定时器，定时100ms
}

void AirPlaneDialog::on_TimeOut()
{
    switch(TaskSettingID)
    {
        case 1:
        {
                //qDebug() << TaskSettingID;
                TaskSettingA = ui->TopHeightBox->value();
                TaskSettingB = ui->BottomHeightBox->value();
                TaskSettingC = ui->MaxCtrlRadiusBox->value();
                TaskSettingD = ui->TakeoffHeightBox->value();
                TaskSettingE = ui->StartCircleHieghtBox->value();
                TaskSettingF = ui->FirstTimeLimitDistanceBox->value();
                TaskSettingG = ui->LandingChangeModeDistatnceBox->value();
                TaskSettingH = ui->LandingLowSpeedHeightBox->value();
                emit SendTaskSetting(TaskSettingID,TaskSettingA,TaskSettingB,TaskSettingC,TaskSettingD,TaskSettingE,TaskSettingF,TaskSettingG,TaskSettingH);

                TaskSettingID += 1;
        }break;
        case 2:
        {
                //qDebug() << TaskSettingID;
                TaskSettingA = ui->CircleDefaultHeightBox->value();
                TaskSettingB = ui->CircleDefaultRadiuBox->value();
                TaskSettingC = ui->CircleDefaultVelocityBox->value();
                TaskSettingD = ui->CircleChangeSpeedBox->value();
                TaskSettingE = 0;
                TaskSettingF = 0;
                TaskSettingG = 0;
                TaskSettingH = 0;
                emit SendTaskSetting(TaskSettingID,TaskSettingA,TaskSettingB,TaskSettingC,TaskSettingD,TaskSettingE,TaskSettingF,TaskSettingG,TaskSettingH);

                TaskSettingID += 1;
        }break;
        case 3:
        {

                TaskSettingID += 1;
        }break;
        case 4:
        {
                TaskSettingID += 1;
        }break;
        case 5:
        {
                TaskSettingID += 1;
        }break;
        case 6:
        {
                TaskSettingID += 1;
        }break;
        default :
        {
                Timer->stop();
                TaskSettingID = 0;
        }break;
    }


}


void AirPlaneDialog::on_ReadFlightSettingButton_clicked()
{
    emit SendCMD(0x01,0,0x01,0,0,0,0,0,0);//发送读取命令
}

void AirPlaneDialog::on_SaveFlightSettingButton_clicked()
{
    emit SendCMD(0x01,0,0x10,0,0,0,0,0,0);//发送保存命令
}
