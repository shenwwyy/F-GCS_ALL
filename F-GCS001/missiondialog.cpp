#include "missiondialog.h"
#include "ui_missiondialog.h"
#include "QDebug"
#include "decode.h"

MissionDialog::MissionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MissionDialog)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowStaysOnTopHint);

    isMissionSettingDialogOpen = false;

    ui->MovedownButton->setHidden(true);
    ui->MoveupButton->setHidden(true);



    ui->InfoTable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->InfoTable->setColumnWidth(0,50);
    ui->InfoTable->setColumnWidth(1,150);
    ui->InfoTable->setColumnWidth(2,150);
    ui->InfoTable->setColumnWidth(3,60);
    ui->InfoTable->setColumnWidth(4,60);
    ui->InfoTable->setColumnWidth(5,60);
    ui->InfoTable->setColumnWidth(6,50);
    ui->InfoTable->verticalHeader()->setVisible(false);
    ui->InfoTable->setSelectionMode(QAbstractItemView::SingleSelection);

    waypointcsv = new WayPointCSV();
    inifile     = new IniFile();

    RouteTimer = new QTimer;
    GetRouteTimer = new QTimer;
    isChangeOnePoint = false;
    PointNum = 0;
    isGet = false;
    connect(RouteTimer,SIGNAL(timeout()),this,SLOT(SendPoint()));


    /*if(ui->LoopBox->isChecked())
    {
        quint16 number = ui->InfoTable->rowCount() - 1;
        emit LoopChecked(number,true);
    }*/

    QString wploop = inifile->ReadIni("FGCS.ini","Map","WPLoop");
    ui->LoopBox->setChecked(wploop.toInt());

    connect(ui->InfoTable->horizontalHeader(),SIGNAL(sectionDoubleClicked(int)),
            this,SLOT(on_SetHeader(int)));
}

void MissionDialog::isMissionSettingDialogOpenModify(bool flag)
{
    if(flag == true)
    {
        MissionSetting->close();
        delete MissionSetting;
       isMissionSettingDialogOpen = false;
    }
    else
       isMissionSettingDialogOpen = true;

    //qDebug() << isMissionSettingDialogOpen;
}

void MissionDialog::on_SetHeader(int column)
{
    if(isMissionSettingDialogOpen == false)
    {
        MissionSetting = new MissionSettingDialog(0,column);
        isMissionSettingDialogOpen = true;

        //connect(MissionSetting,SIGNAL(isWindowsClose(bool)),
        //        this,SLOT(isMissionSettingDialogOpenModify(bool)));

        MissionSetting->show();//只是show的话关闭父窗口后子窗口也会关闭
    }

}

MissionDialog::~MissionDialog()
{
    emit isWindowClose(ID);

  /* if(MissionSetting != 0)
   {
       MissionSetting->close();
       delete MissionSetting;
   }*/
       // this->deleteLater();
        delete RouteTimer;
        delete GetRouteTimer;
        delete ui;

}


void MissionDialog::ChangePoint(int PointNumber, double Latitude, double Longitude)
{   //读取文件修改当前文件的特定值
    ui->InfoTable->setItem(PointNumber,0,new QTableWidgetItem(QString::number(PointNumber+1)));
    ui->InfoTable->setItem(PointNumber,1,new QTableWidgetItem(QString::number(Longitude,'f',8)));
    ui->InfoTable->setItem(PointNumber,2,new QTableWidgetItem(QString::number(Latitude,'f',8)));

    ui->InfoTable->setItem(PointNumber,3,new QTableWidgetItem(inifile->ReadIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Altitude")));
    ui->InfoTable->setItem(PointNumber,4,new QTableWidgetItem(inifile->ReadIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Velocity")));
    ui->InfoTable->setItem(PointNumber,5,new QTableWidgetItem(inifile->ReadIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Radius")));
    ui->InfoTable->setItem(PointNumber,6,new QTableWidgetItem(inifile->ReadIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Action")));

    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Longitude",QString::number(Longitude));
    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Latitude",QString::number(Latitude));

    ui->InfoTable->setVerticalHeaderItem(PointNumber,new QTableWidgetItem(""));
}

void MissionDialog::AddPoint(int PointNumber, QString Latitude, QString Longitude)
{
    ui->InfoTable->setRowCount(ui->InfoTable->rowCount()+1);
    int rowcount = ui->InfoTable->rowCount();

    ui->InfoTable->setItem(rowcount-1,0,new QTableWidgetItem(QString::number(PointNumber+1)));
    ui->InfoTable->setItem(rowcount-1,1,new QTableWidgetItem(Longitude));
    ui->InfoTable->setItem(rowcount-1,2,new QTableWidgetItem(Latitude));
    ui->InfoTable->setItem(rowcount-1,3,new QTableWidgetItem(QString::number(200)));
    ui->InfoTable->setItem(rowcount-1,4,new QTableWidgetItem(QString::number(20)));
    ui->InfoTable->setItem(rowcount-1,5,new QTableWidgetItem(QString::number(100)));
    ui->InfoTable->setItem(rowcount-1,6,new QTableWidgetItem(QString::number(0)));

    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Longitude",Longitude);
    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Latitude",Latitude);
    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Altitude",QString::number(200));
    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Velocity",QString::number(20));
    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Radius",QString::number(100));
    inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(PointNumber+1),"Action",QString::number(0));

    ui->InfoTable->setVerticalHeaderItem(rowcount-1,new QTableWidgetItem(""));
}

void MissionDialog::DecPoint(void)
{
    if(ui->InfoTable->rowCount() != 0)
    {
      ui->InfoTable->setRowCount(ui->InfoTable->rowCount()-1);
    }
}

void MissionDialog::on_EditButton_clicked()
{

}

void MissionDialog::on_UploadButton_clicked()
{
    if(RouteTimer->isActive())
    {
        ui->UploadButton->setText(tr("上传航线"));
        isGet = false;
        PointNum = 0;
        RouteTimer->stop();
        ui->progressBar->setValue(0);
    }
    else if(ui->InfoTable->rowCount() >0)
    {
        ui->UploadButton->setText(tr("取消上传"));
        isGet = false;
        PointNum = 0;
        RouteTimer->start(100);
    }
    else
    {

    }
}

void MissionDialog::on_DownloadButton_clicked()//下载航线
{
    if(ui->DownloadButton->text() == "下载航线")
    {
        ui->DownloadButton->setText(tr("取消下载"));
        ui->InfoTable->setRowCount(0);
        ui->progressBar->setValue(2);
        quint8 Groups = ui->RouteBox->currentIndex();
        Groups = (Groups<<1) | 0x01;
        emit DeleteAll();
        emit SendCMD(4,Groups,0,0,0,0,0,0,0);

    }
    else if(ui->DownloadButton->text() == "取消下载")
    {
        ui->DownloadButton->setText(tr("下载航线"));
        ui->InfoTable->setRowCount(0);
        ui->progressBar->setValue(0);
        emit DeleteAll();
        emit SendCMD(4,0,0,0,0,0,0,0,0);
    }
}

void MissionDialog::ReceiverDot(quint8 Function, quint8 Groups, quint16 TotalPoint, quint16 CurrentPoint, double longitude, double latitude, float Altitude, quint16 Velocity, quint16 Radius, quint16 Action)
{
    if((TotalPoint == 0)&&(CurrentPoint == 0))//如果总航点和当前航点都是0，那么说明没有上传航向
    {
        ui->DownloadButton->setText(tr("下载航线"));
        ui->progressBar->setValue(0);
        emit SendCMD(4,0,0,0,0,0,0,0,0);//发送停止命令
        return;
    }
    ui->LoopBox->setChecked(Function);
    ui->RouteBox->setCurrentIndex(Groups);
    ui->InfoTable->setRowCount(TotalPoint);

    ui->InfoTable->setItem(CurrentPoint-1,0,new QTableWidgetItem(QString::number(CurrentPoint)));
    ui->InfoTable->setItem(CurrentPoint-1,1,new QTableWidgetItem(QString::number(longitude,'f',8)));
    ui->InfoTable->setItem(CurrentPoint-1,2,new QTableWidgetItem(QString::number(latitude,'f',8)));
    ui->InfoTable->setItem(CurrentPoint-1,3,new QTableWidgetItem(QString::number(Altitude)));
    ui->InfoTable->setItem(CurrentPoint-1,4,new QTableWidgetItem(QString::number(Velocity)));
    ui->InfoTable->setItem(CurrentPoint-1,5,new QTableWidgetItem(QString::number(Radius)));
    ui->InfoTable->setItem(CurrentPoint-1,6,new QTableWidgetItem(QString::number(Action)));

    ui->InfoTable->setVerticalHeaderItem(CurrentPoint-1,new QTableWidgetItem(""));

    if(TotalPoint != CurrentPoint)//下载未完成
    {
        int Value = CurrentPoint * 100 / TotalPoint;
        ui->progressBar->setValue(Value);

        quint8 Groups = ui->RouteBox->currentIndex();
        Groups = (Groups<<1) | 0x01;
       emit SendCMD(4,Groups,0,0,0,0,0,0,0);
    }
    else if(TotalPoint == CurrentPoint)//下载完成
    {
        ui->DownloadButton->setText(tr("下载航线"));
        ui->progressBar->setValue(0);
        emit SendCMD(4,0,0,0,0,0,0,0,0);
    }

    emit ShowDownLoadWayPoint(CurrentPoint-1,latitude,longitude);

   /* qDebug() << "Recieve"
             << Groups
             << TotalPoint
             << CurrentPoint
             << longitude
             << latitude
             << Altitude
             << Velocity
             << Radius
             << Action;
             */
}

void MissionDialog::on_SaveButton_clicked()//保存航线
{
    QDir *temp = new QDir;
    if(!temp->exists("./WayPoint"))
       temp->mkdir("./WayPoint");//如果文件夹不存在就新建

    if(ui->InfoTable->rowCount() > 0)
    {
        QDateTime time = QDateTime::currentDateTime();
        QString Path = "./WayPoint/WP";
        Path.append(time.toString("yyyyMMddHHmmss"));

        QString FileName = QFileDialog::getSaveFileName(this,tr("保存航线"),Path,"hwp(*.hwp)");
        if(!FileName.isNull())
        {
            QFile file(FileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                return;
            }
            QTextStream writefiletext(&file);

            QString data = "组别,总航点,航点号,经度,纬度,高度,速度,半径,动作";
            data.append("\n");
            writefiletext<<data;
            data.clear();

            for(int i = 0;i < ui->InfoTable->rowCount();i++)
            {
                data.append(QString::number(ui->RouteBox->currentIndex()));data.append(",");
                data.append(QString::number(ui->InfoTable->rowCount()));   data.append(",");
                data.append(ui->InfoTable->item(i,0)->text());             data.append(",");
                data.append(ui->InfoTable->item(i,1)->text());             data.append(",");
                data.append(ui->InfoTable->item(i,2)->text());             data.append(",");
                data.append(ui->InfoTable->item(i,3)->text());             data.append(",");
                data.append(ui->InfoTable->item(i,4)->text());             data.append(",");
                data.append(ui->InfoTable->item(i,5)->text());             data.append(",");
                data.append(ui->InfoTable->item(i,6)->text());             data.append("\n");
                writefiletext<<data;
                data.clear();
            }
            file.close();
        }
    }

}

void MissionDialog::on_ReadButton_clicked()//读取航线
{

        QDir *temp = new QDir;
        if(!temp->exists("./WayPoint"))
        temp->mkdir("./WayPoint");//如果文件夹不存在就新建

        QString FileName = QFileDialog::getOpenFileName(this,tr("保存航线"),"./WayPoint/","hwp(*.hwp)");
        if(!FileName.isNull())
        {
            QFile file(FileName);
            if(!file.open(QIODevice::ReadOnly))
            {
                return;
            }
            emit DeleteAll();//清空所有航点

            QTextStream * out = new QTextStream(&file);//文本流
            QStringList tempOption = out->readAll().split("\n");//每行以\n区分
            for(int i = 1 ; i < tempOption.count()-1; i++)
            {
                 QStringList tempbar = tempOption.at(i).split(",");//一行中的单元格以，区分

                 qDebug() << tempbar.at(2);
                 QString Group        = tempbar.at(0);
                 QString TatolPoint   = tempbar.at(1);
                 QString CurrentPoint = tempbar.at(2);
                 QString longitude    = tempbar.at(3);
                 QString latitude     = tempbar.at(4);
                 QString Altitude     = tempbar.at(5);
                 QString Velocity     = tempbar.at(6);
                 QString Radius       = tempbar.at(7);
                 QString Action       = tempbar.at(8);

                 ui->RouteBox->setCurrentIndex(Group.toInt());
                 ui->InfoTable->setRowCount(TatolPoint.toInt());
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,0,new QTableWidgetItem(CurrentPoint));
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,1,new QTableWidgetItem(longitude));
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,2,new QTableWidgetItem(latitude));
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,3,new QTableWidgetItem(Altitude));
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,4,new QTableWidgetItem(Velocity));
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,5,new QTableWidgetItem(Radius));
                 ui->InfoTable->setItem(CurrentPoint.toInt()-1,6,new QTableWidgetItem(Action));

                 ui->InfoTable->setVerticalHeaderItem(CurrentPoint.toInt()-1,new QTableWidgetItem(""));

                 emit ShowDownLoadWayPoint(CurrentPoint.toInt()-1,latitude.toDouble(),longitude.toDouble());

            }
            file.close();
        }
}

void MissionDialog::on_MoveupButton_clicked()
{
    QList<QTableWidgetSelectionRange>ranges=ui->InfoTable->selectedRanges();
        int count=ranges.count();//0~ui->InfoTable->count

        for(int i=0;i<count;i++)
        {
           int topRow=ranges.at(i).topRow();
           int bottomRow=ranges.at(i).bottomRow();
           for(int j=topRow;j<=bottomRow;j++)
           {
               if(j>0)
               {//有bug
                      ui->InfoTable->insertRow(j-1);

                      QString str = ui->InfoTable->item(j+1,0)->text();
                      ui->InfoTable->setItem(j-1,0,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j+1,1)->text();
                      ui->InfoTable->setItem(j-1,1,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j+1,2)->text();
                      ui->InfoTable->setItem(j-1,2,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j+1,3)->text();
                      ui->InfoTable->setItem(j-1,3,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j+1,4)->text();
                      ui->InfoTable->setItem(j-1,4,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j+1,5)->text();
                      ui->InfoTable->setItem(j-1,5,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j+1,6)->text();
                      ui->InfoTable->setItem(j-1,6,new QTableWidgetItem(str));

                      ui->InfoTable->removeRow(j+1);
               }
           }
        }

        for(int i=0;i<ui->InfoTable->rowCount();i++)
        {
            ui->InfoTable->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        }

}

void MissionDialog::on_MovedownButton_clicked()
{
    QList<QTableWidgetSelectionRange>ranges=ui->InfoTable->selectedRanges();
        int count=ranges.count();//0~ui->InfoTable->count

        for(int i=0;i<count;i++)
        {
           int topRow=ranges.at(i).topRow();
           int bottomRow=ranges.at(i).bottomRow();
           for(int j=topRow;j<=bottomRow;j++)
           {
               if(j<(ui->InfoTable->rowCount()-1))
               {
                      ui->InfoTable->insertRow(j+2);
                      QString str = ui->InfoTable->item(j,0)->text();
                      ui->InfoTable->setItem(j+2,0,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j,1)->text();
                      ui->InfoTable->setItem(j+2,1,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j,2)->text();
                      ui->InfoTable->setItem(j+2,2,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j,3)->text();
                      ui->InfoTable->setItem(j+2,3,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j,4)->text();
                      ui->InfoTable->setItem(j+2,4,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j,5)->text();
                      ui->InfoTable->setItem(j+2,5,new QTableWidgetItem(str));
                      str = ui->InfoTable->item(j,6)->text();
                      ui->InfoTable->setItem(j+2,6,new QTableWidgetItem(str));

                      ui->InfoTable->removeRow(j);
               }
           }
        }

        for(int i=0;i<ui->InfoTable->rowCount();i++)
        {
            ui->InfoTable->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        }
}

void MissionDialog::on_DeleteButton_clicked()
{
    QList<QTableWidgetSelectionRange>ranges=ui->InfoTable->selectedRanges();
        int count=ranges.count();//0~ui->InfoTable->count

        for(int i=0;i<count;i++)
        {
           int topRow=ranges.at(i).topRow();
           int bottomRow=ranges.at(i).bottomRow();
           for(int j=topRow;j<=bottomRow;j++)
           {
              qDebug() <<"selectRow"<<j+1;
              ui->InfoTable->removeRow(j);
              emit DeletePoint(j);
           }
        }

        for(int i=0;i<ui->InfoTable->rowCount();i++)
        {
            ui->InfoTable->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        }

}

void MissionDialog::on_ClearButton_clicked()
{
    ui->InfoTable->setRowCount(0);
    emit DeleteAll();
}


void MissionDialog::GetWayPoint(mapcontrol::WayPointItem *Item)
{
     int PointNnm = Item->Number();
     QString Latitude,Longitude;
     Latitude = QString::number(Item->Coord().Lat(),'f',8);
     Longitude = QString::number(Item->Coord().Lng(),'f',8);

     AddPoint(PointNnm,Latitude,Longitude);
}

void MissionDialog::SendPoint()
{
    quint8 Function= 0;
    quint8 Groups = 0;
    quint16 TotalPoint = 0,CurrentPoint = 0;
    double Latitude = 0,Longitude = 0;
    quint16 Altitude = 0,Radius = 0,Velocity = 0;
    quint8 Action = 0;

    if(isSendDotCompelect)
    {
        PointNum ++;
        isSendDotCompelect = 0;
    }
    if(PointNum == ui->InfoTable->rowCount())
    {
        ui->progressBar->setValue(0);
        ui->UploadButton->setText(tr("上传航点"));
        RouteTimer->stop();
        PointNum = 0;
        return;
    }

    if(ui->InfoTable->rowCount() != 0)
    {
        Function     = ui->LoopBox->checkState()/2;
        Groups       = ui->RouteBox->currentIndex();
        TotalPoint   = ui->InfoTable->rowCount();
        CurrentPoint = ui->InfoTable->item(PointNum,0)->text().toInt();
        Latitude     = ui->InfoTable->item(PointNum,1)->text().toDouble();
        Longitude    = ui->InfoTable->item(PointNum,2)->text().toDouble();
        Altitude     = ui->InfoTable->item(PointNum,3)->text().toInt();
        Radius       = ui->InfoTable->item(PointNum,4)->text().toInt();
        Velocity     = ui->InfoTable->item(PointNum,5)->text().toInt();
        Action       = ui->InfoTable->item(PointNum,6)->text().toInt();
    }

    int Value = (PointNum+1) * 100 / ui->InfoTable->rowCount();
    ui->progressBar->setValue(Value);

    /*inifile->WriteIni(QString::number(Groups),QString::number(CurrentPoint),"Longitude",QString::number(Longitude));
    inifile->WriteIni(QString::number(Groups),QString::number(CurrentPoint),"Latitude",QString::number(Latitude));
    inifile->WriteIni(QString::number(Groups),QString::number(CurrentPoint),"Altitude",QString::number(Altitude));
    inifile->WriteIni(QString::number(Groups),QString::number(CurrentPoint),"Velocity",QString::number(Velocity));
    inifile->WriteIni(QString::number(Groups),QString::number(CurrentPoint),"Radius",QString::number(Radius));
    inifile->WriteIni(QString::number(Groups),QString::number(CurrentPoint),"Action",QString::number(Action));
*/

    emit SendDot(Function,Groups,TotalPoint,CurrentPoint,Latitude,Longitude,Altitude,Radius,Velocity,Action);


}

void MissionDialog::on_LoopBox_clicked()
{
    bool Flag = ui->LoopBox->isChecked();
    emit SetWPLoop(Flag);
    inifile->WriteIni("FGCS.ini","Map","WPLoop",QString::number(Flag));
    //qDebug() << ui->LoopBox->isChecked();
}


void MissionDialog::RecieveWayPoint(void)
{

}

void MissionDialog::ShowEcho(quint8 ID, quint8 A, quint8 B, quint8 C, quint8 D, quint8 E, quint8 F, quint8 G, quint8 H)
{
    union {uint8_t B[2];uint16_t D;}src;

    quint16 SendDotID;
    if(ID == 0x04)
    {
       src.B[0] = B;
       src.B[1] = C;
       SendDotID = src.D;

       if(A == 0x07)
       {
           isSendDotCompelect = true;
           //qDebug() << "Compelect";

       }
       else
       {
           isSendDotCompelect = false;
       }

    }

}



void MissionDialog::on_InfoTable_cellChanged(int row, int column)//行列
{
    row = row;
    column = column;
    /*
    QString data = ui->InfoTable->item(row+1,column)->text();
    switch(column)
    {
        case 1:{
            inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(row+1),"Longitude",
                              data);
        }break;
        case 2:{
            inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(row+1),"Latitude",
                              data);
        }break;
        case 3:{
            inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(row+1),"Altitude",
                              data);
        }break;
        case 4:{
            inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(row+1),"Velocity",
                              data);
        }break;
        case 5:{
            inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(row+1),"Radius",
                              data);
        }break;
        case 6:{
             inifile->WriteIni(QString::number(ui->RouteBox->currentIndex()),QString::number(row+1),"Action",
                               data);
        }break;
    }*/
    //qDebug() <<ui->InfoTable->item(row,column)->text();
}

