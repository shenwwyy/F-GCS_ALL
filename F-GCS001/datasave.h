#ifndef DATASAVE_H
#define DATASAVE_H

#include "QDateTime"
#include "QTimer"
#include "QObject"
class DataSave:public QObject
{
    Q_OBJECT
public:
    DataSave();
   ~DataSave();


private slots:
    void setFileDir(QString Dir);
    void Save(QString data);

    void Record(quint8 RSSI,
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
                float RelativeHeight,
                quint8 mode1, quint8 mode2, quint8 LockStatus, quint8 TurningStatus, quint8 CurrentStatus,
                quint8 CtrlMode, quint8 AirPlane,quint8 FlightMode,
                quint8 Command1,
                quint8 Command2
               );


private:

    QString FileDir;
    QString FirstLine;



};

#endif // DATASAVE_H
