#include "datasave.h"
#include "QDebug"
#include "QApplication"
#include "QFile"
#include "QDir"

DataSave::DataSave()
{
    QDir *temp = new QDir;
    if(!temp->exists("./Data"))
    temp->mkdir("./Data");//如果文件夹不存在就新建

    QDateTime time = QDateTime::currentDateTime();
    FileDir = qApp->applicationDirPath();
    FileDir.append("./");
    FileDir.append("Data");
    FileDir.append("/");
    FileDir.append(time.toString("yyyyMMdd-hhmmss"));
    FileDir.append(".csv");

    FirstLine = "帧计数,传感器,";
    FirstLine.append("加速度1X,加速度1Y,加速度1Z,角速率1X,角速率1Y,角速率1Z,滚转1,俯仰1,真航向1,");
    FirstLine.append("加速度2X,加速度2Y,加速度2Z,角速率2X,角速率2Y,角速率2Z,滚转2,俯仰2,真航向2,");
    FirstLine.append("GPS1状态,GPS1星数,GPS1纬度,GPS1经度,GPS1海拔,GPS1航迹角,");
    FirstLine.append("GPS2状态,GPS2星数,GPS2纬度,GPS2经度,GPS2海拔,GPS2航迹角,");
    FirstLine.append("UTC时,UTC分,UTC秒,");
    FirstLine.append("速度,空速,");
    FirstLine.append("北向速度1,东向速度1,地向速度1,北向速度2,东向速度2,地向速度2,");
    FirstLine.append("旋翼电压,固定翼电压,飞控电压,");
    FirstLine.append("遥控,遥控副翼,遥控俯仰,遥控油门,遥控方向,");
    FirstLine.append("旋翼油门,电机1,电机2,电机3,电机4,固定翼油门1,固定翼油门2,副翼舵,升降舵,方向舵,");
    FirstLine.append("目标航点,待飞距,侧偏距,偏航角,高度差,速度差,");
    FirstLine.append("目标滚转,目标俯仰,目标偏航,目标高度,相对高度,");
    FirstLine.append("控制模式,飞机类型,锁定状态,转弯状态,当前状态,关车模式,命令1,命令2");
    FirstLine.append("\n");

    QFile csvFile(FileDir);
    if (csvFile.open(QIODevice::ReadWrite))
    {
       QTextStream writefiletext(&csvFile);
       writefiletext<<FirstLine;
       csvFile.close();
    }

}

DataSave::~DataSave()
{

}


void DataSave::setFileDir(QString Dir)
{
    FileDir = Dir;
    //qDebug() << FileDir;
}


void DataSave::Record(quint8 RSSI,
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
                      float RelativeHeight, quint8 mode1, quint8 mode2, quint8 LockStatus, quint8 TurningStatus, quint8 CurrentStatus,
                      quint8 CtrlMode, quint8 AirPlane, quint8 FlightMode,
                      quint8 Command1,
                      quint8 Command2
)
{
    QString str;
    str.append(QString::number(RSSI));          str.append(",");
    str.append(QString::number(IMUSelect));     str.append(",");
    str.append(QString::number(ACC1_X));        str.append(",");
    str.append(QString::number(ACC1_Y));        str.append(",");
    str.append(QString::number(ACC1_Z));        str.append(",");
    str.append(QString::number(GYRO1_X));       str.append(",");
    str.append(QString::number(GYRO1_Y));       str.append(",");
    str.append(QString::number(GYRO1_Z));       str.append(",");
    str.append(QString::number(ROLL1));         str.append(",");
    str.append(QString::number(PITCH1));        str.append(",");
    str.append(QString::number(YAW1));          str.append(",");
    str.append(QString::number(ACC2_X));        str.append(",");
    str.append(QString::number(ACC2_Y));        str.append(",");
    str.append(QString::number(ACC2_Z));        str.append(",");
    str.append(QString::number(GYRO2_X));       str.append(",");
    str.append(QString::number(GYRO2_Y));       str.append(",");
    str.append(QString::number(GYRO2_Z));       str.append(",");
    str.append(QString::number(ROLL2));         str.append(",");
    str.append(QString::number(PITCH2));        str.append(",");
    str.append(QString::number(YAW2));          str.append(",");
    str.append(QString::number(GPS1_Status));         str.append(",");
    str.append(QString::number(GPS1_Stars));          str.append(",");
    str.append(QString::number(Latitude1,'f',8));     str.append(",");
    str.append(QString::number(Longitude1,'f',8));    str.append(",");
    str.append(QString::number(Altitude1,'f',8));           str.append(",");
    str.append(QString::number(Course1,'f',8));             str.append(",");
    str.append(QString::number(GPS2_Status));         str.append(",");
    str.append(QString::number(GPS2_Stars));          str.append(",");
    str.append(QString::number(Latitude2,'f',8));     str.append(",");
    str.append(QString::number(Longitude2,'f',8));    str.append(",");
    str.append(QString::number(Altitude2,'f',8));           str.append(",");
    str.append(QString::number(Course2,'f',8));             str.append(",");
    str.append(QString::number(Hour));                str.append(",");
    str.append(QString::number(Minute));              str.append(",");
    str.append(QString::number(Second));              str.append(",");
    str.append(QString::number(SpeedSelect));         str.append(",");
    str.append(QString::number(AirSpeed,'f',8));            str.append(",");
    str.append(QString::number(NorthSpeed1,'f',8));         str.append(",");
    str.append(QString::number(EastSpeed1,'f',8));          str.append(",");
    str.append(QString::number(DownSpeed1,'f',8));          str.append(",");
    str.append(QString::number(NorthSpeed2,'f',8));         str.append(",");
    str.append(QString::number(EastSpeed2,'f',8));          str.append(",");
    str.append(QString::number(DownSpeed2,'f',8));          str.append(",");
    str.append(QString::number(VotageMultiRotor));    str.append(",");
    str.append(QString::number(VotageFixedWing));     str.append(",");
    str.append(QString::number(VotageSystem));        str.append(",");
    str.append(QString::number(isRCValid));           str.append(",");
    str.append(QString::number(ROL));                 str.append(",");
    str.append(QString::number(PIT));                 str.append(",");
    str.append(QString::number(THR));                 str.append(",");
    str.append(QString::number(RUD));                 str.append(",");
    str.append(QString::number(RotorThrottle));       str.append(",");
    str.append(QString::number(Rotor1));              str.append(",");
    str.append(QString::number(Rotor2));              str.append(",");
    str.append(QString::number(Rotor3));              str.append(",");
    str.append(QString::number(Rotor4));              str.append(",");
    str.append(QString::number(Throttle1));           str.append(",");
    str.append(QString::number(Throttle2));           str.append(",");
    str.append(QString::number(Aileron));             str.append(",");
    str.append(QString::number(Elevator));            str.append(",");
    str.append(QString::number(Rudder));              str.append(",");
    str.append(QString::number(CurrentTargetPoint));  str.append(",");
    str.append(QString::number(WaitToFlight));        str.append(",");
    str.append(QString::number(DiffrentDistance));    str.append(",");
    str.append(QString::number(DiffrentAngle));       str.append(",");
    str.append(QString::number(DiffrentHeigh));       str.append(",");
    str.append(QString::number(DiffrentSpeed));       str.append(",");
    str.append(QString::number(TargetRoll));          str.append(",");
    str.append(QString::number(TargetPitch));         str.append(",");
    str.append(QString::number(TargetYaw));           str.append(",");
    str.append(QString::number(TargetHeigh));         str.append(",");
    str.append(QString::number(RelativeHeight));      str.append(",");
    str.append(QString::number(CtrlMode));            str.append(",");
    str.append(QString::number(AirPlane));            str.append(",");
    str.append(QString::number(LockStatus));          str.append(",");
    str.append(QString::number(TurningStatus));       str.append(",");
    str.append(QString::number(CurrentStatus));       str.append(",");
    str.append(QString::number(FlightMode));          str.append(",");
    str.append(QString::number(Command1));            str.append(",");
    str.append(QString::number(Command2));            str.append("\n");

    Save(str);
}

void DataSave::Save(QString data)
{
    QFile csvFile(FileDir);
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
       QTextStream writefiletext(&csvFile);
       writefiletext<<data;
       csvFile.close();
    }
}
















