#include "savedialog.h"
#include "ui_savedialog.h"
#include "QFileDialog"
#include "QDebug"

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowStaysOnTopHint);

    ui->SaveFreq->setCurrentIndex(3);

    QDateTime time = QDateTime::currentDateTime();
    DefaultFileDir = qApp->applicationDirPath();
    DefaultFileDir.append("/");
    DefaultFileDir.append("Data");
    DefaultFileDir.append("/");
    DefaultFileDir.append(time.toString("yyyyMMdd-hhmmss"));
    DefaultFileDir.append(".csv");

    FileDir        = "";


    if(ui->checkBox->isChecked())
    {
        ui->SelectButton->setEnabled(false);
        ui->DirEdit->setEnabled(false);
        ui->DirEdit->setText(DefaultFileDir);
        FileDir = DefaultFileDir;
    }
    else
    {
        ui->SelectButton->setEnabled(true);
        ui->DirEdit->setEnabled(true);
        FileDir = ui->DirEdit->text();
    }


    //初始化定时器
    ReplayTimer = new QTimer;

}

SaveDialog::~SaveDialog()
{
    emit isWindowClose(ID);

    if(ReplayTimer->isActive())
    {
        ReplayTimer->stop();
    }
    delete ReplayTimer;
    delete ui;
}

void SaveDialog::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->SelectButton->setEnabled(false);
        ui->DirEdit->setEnabled(false);
    }
    else
    {
        ui->SelectButton->setEnabled(true);
        ui->DirEdit->setEnabled(true);
    }
}

void SaveDialog::on_SelectButton_clicked()
{

    QDir *temp = new QDir;
    if(!temp->exists("./Data"))
    temp->mkdir("./Data");//如果文件夹不存在就新建

    QString FileName;

    QDateTime time = QDateTime::currentDateTime();
    FileName = qApp->applicationDirPath();
    FileName.append("/");
    FileName.append("Data");
    FileName.append("/");
    FileName.append(time.toString("yyyyMMdd-hhmmss"));
    QString sFileDir = QFileDialog::getSaveFileName(this, tr("选择文件"),FileName,
                                                       tr("Files (*.csv)"));
    FileDir = sFileDir;
    ui->DirEdit->setText(sFileDir);
}

void SaveDialog::on_ReplaySelectButton_clicked()
{
    QString sFileDir = QFileDialog::getOpenFileName(this, tr("选择文件"),"",
                                                       tr("Files (*.csv)"));
    ui->ReplayEdit->setText(sFileDir);
}

void SaveDialog::on_StartReplayButton_clicked()
{
    if(!ReplayTimer->isActive())
    {
        if((ui->Pause->text() != "继续")&&(!ui->ReplayEdit->text().isEmpty()))
        {
            switch(ui->ReplayFreq->currentIndex())
            {
                case 0: Frequency = 1000;break;//1hz
                case 1: Frequency = 500;break;
                case 2: Frequency = 200;break;
                case 3: Frequency = 100;break;
                case 4: Frequency = 50;break;
                case 5: Frequency = 20;break;
                case 6: Frequency = 10;break;
                case 7: Frequency = 5;break;
                case 8: Frequency = 2;break;
               default: Frequency = 100;break;
            }
             ui->StartReplayButton->setText(tr("结束回放"));
             ui->ReplayEdit->setEnabled(false);
             ui->ReplaySelectButton->setEnabled(false);
             ui->Pause->setEnabled(true);
             ui->ReplayFreq->setEnabled(false);
             ReplayCount = 0;


             QString FileName = ui->ReplayEdit->text();
             if(!FileName.isNull())//不用每次都打开一个文件，只要把数据放到内存就行
             {
                  QFile file(FileName);
                  if(!file.open(QIODevice::ReadOnly))
                  {
                      return;//可能有bug
                  }
                  QTextStream * out = new QTextStream(&file);//文本流
                  ReplayList = out->readAll().split("\n");//每行以\n区分
                  file.close();
             }//数据准备就绪

             connect(ReplayTimer,SIGNAL(timeout()),
                     this,SLOT(ReplayData()));
             ReplayTimer->start(Frequency);
       }
       else
       {
            ui->StartReplayButton->setText(tr("开始回放"));
            ui->Pause->setText(tr("暂停"));
            ui->ReplayEdit->setEnabled(true);
            ui->ReplaySelectButton->setEnabled(true);
            ui->Pause->setEnabled(false);
            ui->ReplayFreq->setEnabled(true);
            ReplayCount = 0;
            ReplayTimer->stop();
       }
    }
    else
    {
        ui->StartReplayButton->setText(tr("开始回放"));
        ui->Pause->setText(tr("暂停"));
        ui->ReplayEdit->setEnabled(true);
        ui->ReplaySelectButton->setEnabled(true);
        ui->Pause->setEnabled(false);
        ui->ReplayFreq->setEnabled(true);
        ReplayCount = 0;
        ReplayTimer->stop();
    }
}

void SaveDialog::ReplayData(void)
{
     ReplayCount++;

     if(ReplayCount == (ReplayList.count()-1))//到达结束点
     {
         ui->StartReplayButton->setText(tr("开始回放"));
         ui->Pause->setText(tr("暂停"));
         ui->ReplayEdit->setEnabled(true);
         ui->ReplaySelectButton->setEnabled(true);
         ui->Pause->setEnabled(false);
         ui->ReplayFreq->setEnabled(true);
         ReplayCount = 0;
         ReplayTimer->stop();

         QMessageBox *message = new QMessageBox;
         message->setWindowFlags(Qt::WindowStaysOnTopHint);
         message->setWindowTitle(tr("回放提示"));
         message->setWindowIcon(QIcon(":/image/resource/focus.png"));
         message->setText(tr("     本次数据回放结束          "));
         message->show();
         return;
     }

     ReplayItem = ReplayList.at(ReplayCount).split(",");//一行中的单元格以,区分

     QString Roll,Pitch,Yaw,gyro_X,gyro_Y,gyro_Z,Ail,Ele,Thr,Rud,AUX1,AUX2,AUX3,AUX4,PWM1,PWM2,PWM3,PWM4,PWM5,PWM6,PWM7,PWM8,PWM9,PWM10,PWM11,PWM12;
     QString alt_U,alt_P,Votage1,Votage2,Current1,Current2,GPS_Status,GPS_Star,Latitude,Longitude,Altitude,GPS_Speed,GPS_Course;
     QString CtrlMode,PlaneMode,CurrentStatus,LockStatus,TurningStatus,CurrentPoint,AirSpeed;
     QString WaitToFilght,DiffDistance,DiffAngle,DiffHight,DiffSpeed,TarRoll,TarPitch,TarYaw,TarHight,UseSpeed,wzSpeed,BoardHight;

     Roll   = ReplayItem.at(1);
     gyro_X = ReplayItem.at(2);
     Pitch  = ReplayItem.at(3);
     gyro_Y = ReplayItem.at(4);
     Yaw    = ReplayItem.at(5);
     gyro_Z = ReplayItem.at(6);
     Ail    = ReplayItem.at(7);
     Ele    = ReplayItem.at(8);
     Thr    = ReplayItem.at(9);
     Rud    = ReplayItem.at(10);
     AUX1   = ReplayItem.at(11);
     AUX2   = ReplayItem.at(12);
     AUX3   = ReplayItem.at(13);
     AUX4   = ReplayItem.at(14);
     PWM1   = ReplayItem.at(15);
     PWM2   = ReplayItem.at(16);
     PWM3   = ReplayItem.at(17);
     PWM4   = ReplayItem.at(18);
     PWM5   = ReplayItem.at(19);
     PWM6   = ReplayItem.at(20);
     PWM7   = ReplayItem.at(21);
     PWM8   = ReplayItem.at(22);
     PWM9   = ReplayItem.at(23);
     PWM10  = ReplayItem.at(24);
     PWM11  = ReplayItem.at(25);
     PWM12  = ReplayItem.at(26);

     alt_U      = ReplayItem.at(27);
     alt_P      = ReplayItem.at(28);
     Votage1    = ReplayItem.at(29);
     Votage2    = ReplayItem.at(30);
     Current1   = ReplayItem.at(31);
     Current2   = ReplayItem.at(32);
     GPS_Status = ReplayItem.at(33);
     GPS_Star   = ReplayItem.at(34);
     Latitude   = ReplayItem.at(35);
     Longitude  = ReplayItem.at(36);
     Altitude   = ReplayItem.at(37);
     GPS_Speed  = ReplayItem.at(38);
     GPS_Course = ReplayItem.at(39);

     CtrlMode      = ReplayItem.at(40);
     PlaneMode     = ReplayItem.at(41);
     CurrentStatus = ReplayItem.at(42);
     LockStatus    = ReplayItem.at(43);
     TurningStatus = ReplayItem.at(44);
     CurrentPoint  = ReplayItem.at(45);
     AirSpeed      = ReplayItem.at(46);

     WaitToFilght  = ReplayItem.at(47);
     DiffDistance  = ReplayItem.at(48);
     DiffAngle     = ReplayItem.at(49);
     DiffHight     = ReplayItem.at(50);
     DiffSpeed     = ReplayItem.at(51);
     TarRoll       = ReplayItem.at(52);
     TarPitch      = ReplayItem.at(53);
     TarYaw        = ReplayItem.at(54);
     TarHight      = ReplayItem.at(55);
     UseSpeed      = ReplayItem.at(56);
     wzSpeed       = ReplayItem.at(57);
     BoardHight    = ReplayItem.at(58);


     emit ShowSignals(Ail.toFloat(),Ele.toFloat(),Thr.toFloat(),Rud.toFloat(),AUX1.toFloat(),AUX2.toFloat(),AUX3.toFloat(),AUX4.toFloat(),0,0,
                      PWM1.toFloat(),PWM2.toFloat(),PWM3.toFloat(),PWM4.toFloat(),PWM5.toFloat(),PWM6.toFloat(),PWM7.toFloat(),PWM8.toFloat(),PWM9.toFloat(),PWM10.toFloat(),PWM11.toFloat(),PWM12.toFloat());
     //qDebug() << "emit ShowSignals" << Ail.toFloat()<<Ele.toFloat()<<Thr.toFloat()<<Rud.toFloat()<<AUX1.toFloat()<<AUX2.toFloat()<<AUX3.toFloat()<<AUX4.toFloat()<<PWM1.toFloat()<<PWM2.toFloat();
     emit ShowStatus(alt_U.toFloat(),alt_P.toFloat(),Votage1.toFloat(),Votage2.toFloat(),Current1.toFloat(),Current2.toFloat());
     emit ShowGPS1(GPS_Status.toInt(),GPS_Star.toInt(),Latitude.toDouble(),Longitude.toDouble(),Altitude.toFloat(),GPS_Speed.toFloat(),GPS_Course.toFloat());
     emit ShowAttitude(0,0,0,gyro_X.toInt(),gyro_Y.toInt(),gyro_Z.toInt(),0,0,0,Roll.toFloat(),Pitch.toFloat(),Yaw.toFloat());
     emit ShowRouteInfo(CtrlMode.toInt(),PlaneMode.toInt(),CurrentStatus.toInt(),LockStatus.toInt(),TurningStatus.toInt(),CurrentPoint.toInt(),
                        AirSpeed.toFloat(),WaitToFilght.toFloat(),DiffDistance.toFloat(),DiffAngle.toFloat(),DiffHight.toFloat(),DiffSpeed.toFloat(),
                        TarRoll.toInt(),TarPitch.toInt(),TarYaw.toInt(),TarHight.toInt(),UseSpeed.toFloat(),wzSpeed.toFloat(),BoardHight.toFloat());
}

void SaveDialog::on_Pause_clicked()
{
    if(ReplayTimer->isActive())
    {
        ui->Pause->setText(tr("继续"));
        ReplayTimer->stop();
    }
    else
    {
        ui->Pause->setText(tr("暂停"));
        ReplayTimer->start(Frequency);
    }
}

void SaveDialog::on_PathSelectButton_clicked()
{
    int RecodeFreq;
    switch(ui->SaveFreq->currentIndex())
    {
        case 0: RecodeFreq = 1000;break;//1hz
        case 1: RecodeFreq = 500;break;//2
        case 2: RecodeFreq = 200;break;//5
        case 3: RecodeFreq = 100;break;//10
        case 4: RecodeFreq = 50;break;//20
        case 5: RecodeFreq = 20;break;//50
        case 6: RecodeFreq = 10;break;//100
       default: RecodeFreq = 100;break;
    }

        emit SendFreq(RecodeFreq);
        emit SendSaveDir(FileDir);
}

void SaveDialog::on_CleanTrialButton_clicked()
{
    emit CleanTrail(true);
}
