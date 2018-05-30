#include "waypointcsv.h"
#include "QDir"
#include "QTextStream"
#include "QDebug"

WayPointCSV::WayPointCSV(QObject *parent) : QObject(parent)
{

}

void WayPointCSV::Save(_waypoint *WayPoint)//保存航线
{
    QDir *temp = new QDir;
    if(!temp->exists("./WayPoint"))
       temp->mkdir("./WayPoint");//如果文件夹不存在就新建

        QString Path = "./WayPoint/WP";
        Path.append(QString::number(WayPoint->group));
        Path.append(".hwps");
        QFile file(Path);
        if(WayPoint->currentpoint == 1)
        {
            if(!file.open(QIODevice::WriteOnly))
            {
                return;
            }
        }
        else
        {
            if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                return;
            }
        }

        QTextStream writefiletext(&file);

        QString data;

        data.append(QString::number(WayPoint->circle));               data.append(",");
        data.append(QString::number(WayPoint->group));                data.append(",");
        data.append(QString::number(WayPoint->totalpoint));           data.append(",");
        data.append(QString::number(WayPoint->currentpoint));         data.append(",");
        data.append(QString::number(WayPoint->longitude));            data.append(",");
        data.append(QString::number(WayPoint->latitude));             data.append(",");
        data.append(QString::number(WayPoint->altitude));             data.append(",");
        data.append(QString::number(WayPoint->velocity));             data.append(",");
        data.append(QString::number(WayPoint->radiu));                data.append(",");
        data.append(QString::number(WayPoint->action));               data.append("\n");
        writefiletext<<data;

        file.close();
}

void WayPointCSV::Read(quint8 Group, qint32 PointNumber, _waypoint *WayPoint)//读取航线
{

        QDir *temp = new QDir;
        if(!temp->exists("./WayPoint"))
        temp->mkdir("./WayPoint");//如果文件夹不存在就新建

        QString Path = "./WayPoint/WP";
        Path.append(QString::number(Group));
        Path.append(".hwps");

        QFile file(Path);
        if(!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        QTextStream * out = new QTextStream(&file);//文本流
        QStringList tempOption = out->readAll().split("\n");//每行以\n区分

         QStringList tempbar = tempOption.at(PointNumber).split(",");//一行中的单元格以，区分

         QString circle        = tempbar.at(0);
         QString group         = tempbar.at(1);
         QString totalpoint    = tempbar.at(2);
         QString currentpoint  = tempbar.at(3);
         QString longitude     = tempbar.at(4);
         QString latitude      = tempbar.at(5);
         QString altitude      = tempbar.at(6);
         QString velocity      = tempbar.at(7);
         QString radiu         = tempbar.at(8);
         QString action        = tempbar.at(9);

         WayPoint->circle       = circle.toInt();
         WayPoint->group        = group.toInt();
         WayPoint->totalpoint   = totalpoint.toInt();
         WayPoint->currentpoint = currentpoint.toInt();
         WayPoint->longitude    = longitude.toDouble();
         WayPoint->latitude     = latitude.toDouble();
         WayPoint->altitude     = altitude.toFloat();
         WayPoint->velocity     = velocity.toFloat();
         WayPoint->radiu        = radiu.toFloat();
         WayPoint->action       = action.toInt();
        file.close();

}














