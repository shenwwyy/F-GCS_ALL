#ifndef ATTITUDEPANEL_H
#define ALTITUDEPANEL_H

#include <QWidget>

class QColor;

class Q_DECL_EXPORT AttitudePanel: public QWidget
{
    Q_OBJECT
public:
    enum ErrorCode {MaxValueError=1,MinValueError,ThresholdError,TargetError,PrecisionError,ColorError,UnitsEmpty,OutOfRange};

    explicit AttitudePanel(QWidget *parent = 0);





Q_SIGNALS:
    void errorSignal(int);

public Q_SLOTS:
    void setValue(double Pitch,double Roll,double Yaw);
    void setCourseValue(double Course);
    void setPitchRollMinValue(int value);
    void setPitchRollMaxValue(int value);
    void setPlaneMode(QString Mode);
    void setCtrlMode(QString Mode);
    void setLockMode(QString Mode);
    void setTurningStatus(QString Mode);
    void setCurrentStatusMode(QString Mode);
    void setGPSStatus(int Status);
    void setGPSStarNumber(int Number);
    void setGPSAltitude(double Altitude);
    void setPressureAltitude(double Altitude);
    void setGPSSpeed(double speed);
    void setAirSpeed(double speed);
    void setBattery(double Battery);
    void setLed(QColor LED);

    void setProtect(quint8 ProtectType,QString ProtectString);

    double ValueRoll(void){
        return m_RollValue;
    }

    double ValuePitch(void){
        return m_PitchValue;
    }

    double ValueYaw(void){
        return m_YawValue;
    }

    QString CtrlMode(void){
         return m_CtrlModeValue;
    }

    QString AirplaneMode(void){
         return m_AirplaneModeValue;
    }

    QString CurrentStatus(void){
         return m_CurrentStatusValue;
    }

    QString LockStatus(void){
         return m_LockStatusValue;
    }

    QString TurningStatus(void){
         return m_TurningStatusValue;
    }



protected:

    void mousePressEvent(QEvent *event);

    void paintEvent(QPaintEvent *);

    void drawLed(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawForeground(QPainter *painter);
    void drawAwindow(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawCenterLine(QPainter *painter);
    void drawText(QPainter *painter);
    void drawGPS(QPainter *painter);
    void drawHight(QPainter *painter);
    void drawSpeed(QPainter *painter);
    void drawBattery(QPainter *painter);
    void drawProtect(QPainter *painter);


private:

    double m_PitchValue;
    double m_RollValue;
    double m_YawValue;
    double m_CourseValue;

    double m_BatteryValue;

    double m_GPSAltitudeValue;
    double m_PressureAltitudeValue;

    double m_GPSSpeedValue;
    double m_AirSpeedValue;

    QString m_CtrlModeValue;
    QString m_AirplaneModeValue;
    QString m_CurrentStatusValue;
    QString m_LockStatusValue;
    QString m_TurningStatusValue;

    int m_GPSStatusValue;
    int m_GPSStarValue;

    float m_Z_SpeedValue;
    float m_Z_HightValue;
    //float m_X_SpeedValue;

    //保护
    QString m_ProtectString;
    quint8  m_Protect;

    double m_Pitch_Roll_MinValue;
    double m_Yaw_MinValue;
    double m_CourseMinValue;
    double m_Pitch_Roll_MaxValue;
    double m_Yaw_MaxValue;
    double m_CourseMaxValue;

    double m_BatteryMinValue;
    double m_BatteryMaxValue;

    QColor m_CentreLineColor;
    QColor m_ForeColor;
    QColor m_CornerColor;
    QColor m_GroundColor;
    QColor m_SkyColor;

    QColor m_LedColor;



};

#endif // ALTITUDEPANEL_H
