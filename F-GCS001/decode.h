#ifndef DECODE_H
#define DECODE_H

#include <QObject>
#include "QThread"

class Decode:public QObject
//class Decode:public QThread
{
        Q_OBJECT
public:
    Decode();



signals:

    void GET();


    void ShowVersion(quint16,quint16,quint16,quint16,QString);
    void ShowEcho(quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8);
    void ShowRemote(quint8,quint8,
                    float,float,float,float,float,float,float,float,float,
                    float,float,float,float,float,float,float,float,float,
                    quint8,quint8,double,double,float,float,
                    quint8,quint8,double,double,float,float,
                    quint8,quint8,quint8,
                    quint8,
                    float,
                    float,float,float,float,float,float,
                    float,float,float,
                    quint8,
                    float,float,float,float,
                    float,float,float,float,float,float,float,float,float,float,
                    quint16,quint16,float,
                    float,float,float,float,float,float,float,float,
                    quint8,quint8,quint8,quint8,quint8,
                    quint8,quint8,quint8,
                    quint8,
                    quint8);
    void ShowAttitude(float,float,float);
    void ShowPID(quint8,float,float,float,float,float,float,float,float,float,
                        float,float,float,float,float,float,float,float,float);
    void ShowDot(quint8,quint8,quint16,quint16,double,double,float,quint16,quint16,quint16);
    void ShowTaskSet(quint8,float,float,float,float,float,float,float,float);
    void ShowCalibration(quint8,float,float,float);

    void ShowServoPolar(qint8,qint8,qint8,qint8,
                        qint8,qint8,qint8,qint8,
                        qint8,qint8,qint8,qint8,
                        qint8,qint8,qint8,qint8,
                        qint8,qint8,qint8,qint8);
    void ShowServoPoly(float,float,float,
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


    //发送
    void SendFrame(QByteArray);

private:
    //void run();



private slots:

    void DataThread(void);

    void GetSerialData(QByteArray rawdata);
    bool SUMcheck(QByteArray data);
    quint16 CRCcheck(QByteArray data);

    void Version(QByteArray data);
    void Echo(QByteArray data);
    void Remote(QByteArray data);
    void PID(QByteArray data);
    void ReceiveDot(QByteArray data);
    void TaskSet(QByteArray data);
    void Calibration(QByteArray data);

    void BiasParmeter(QByteArray data);

    void ServoPolar(QByteArray data);
    void ServoPoly(QByteArray data);

    //发送
    void SendBeat(void);
    void SendCMD(quint8 ID, quint8 A, quint8 B, quint8 C,
                  quint8 D, quint8 E, quint8 F, quint8 G, quint8 H);
    void SendPID(quint8 ID, float PID1_P,  float PID1_I,  float PID1_D,
                  float PID2_P,  float PID2_I,  float PID2_D,
                  float PID3_P,  float PID3_I,  float PID3_D,
                  float PID4_P,  float PID4_I,  float PID4_D,
                  float PID5_P,  float PID5_I,  float PID5_D,
                  float PID6_P,  float PID6_I,  float PID6_D);
    void SendDot(quint8 Function,quint8 Groups,
                   quint16 TotalPoint,quint16 CurrentPoint,
                   double Longitude,double Latitude,float Altitude,
                   quint16 Velocity,quint16 Radius,quint16 Action);
    void SendTaskSetting(quint8 ID, float A, float B, float C, float D, float E, float F, float G, float H);

    void SendScale(quint8 ID, float A, float B, float C, float D, float E, float F);

    void SendPWM(quint8 ID, quint16 *PWM);

    void SendServoPolar(quint8 ID,
                        quint8 PWM1,quint8 PWM2,quint8 PWM3,quint8 PWM4,quint8 PWM5,quint8 PWM6,quint8 PWM7,quint8 PWM8,quint8 PWM9,quint8 PWM10,
                        quint8 PWM11,quint8 PWM12,quint8 PWM13,quint8 PWM14,quint8 PWM15,quint8 PWM16,quint8 Freq1,quint8 Freq2,quint8 Freq3,quint8 Freq4);

    void SendServoPolynomial(quint8 ID, float Poly_A_PWM1, float Poly_B_PWM1, float Poly_C_PWM1,
                                       float Poly_A_PWM2, float Poly_B_PWM2, float Poly_C_PWM2,
                                       float Poly_A_PWM3, float Poly_B_PWM3, float Poly_C_PWM3,
                                       float Poly_A_PWM4, float Poly_B_PWM4, float Poly_C_PWM4,
                                       float Poly_A_PWM5, float Poly_B_PWM5, float Poly_C_PWM5,
                                       float Poly_A_PWM6, float Poly_B_PWM6, float Poly_C_PWM6,
                                       float Poly_A_PWM7, float Poly_B_PWM7, float Poly_C_PWM7,
                                       float Poly_A_PWM8, float Poly_B_PWM8, float Poly_C_PWM8,
                                       float Poly_A_PWM9, float Poly_B_PWM9, float Poly_C_PWM9,
                                       float Poly_A_PWM10, float Poly_B_PWM10, float Poly_C_PWM10,
                                       float Poly_A_PWM11, float Poly_B_PWM11, float Poly_C_PWM11,
                                       float Poly_A_PWM12, float Poly_B_PWM12, float Poly_C_PWM12,
                                       float Poly_A_PWM13, float Poly_B_PWM13, float Poly_C_PWM13,
                                       float Poly_A_PWM14, float Poly_B_PWM14, float Poly_C_PWM14,
                                       float Poly_A_PWM15, float Poly_B_PWM15, float Poly_C_PWM15,
                                       float Poly_A_PWM16, float Poly_B_PWM16, float Poly_C_PWM16,
                                       float Poly_A_ROTOR1, float Poly_B_ROTOR1, float Poly_C_ROTOR1,
                                       float Poly_A_ROTOR2, float Poly_B_ROTOR2, float Poly_C_ROTOR2,
                                       float Poly_A_ROTOR3, float Poly_B_ROTOR3, float Poly_C_ROTOR3,
                                       float Poly_A_ROTOR4, float Poly_B_ROTOR4, float Poly_C_ROTOR4,
                                       float Poly_A_ROTOR5, float Poly_B_ROTOR5, float Poly_C_ROTOR5);





};

























#endif // DECODE_H
