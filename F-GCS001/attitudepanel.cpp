#include "attitudepanel.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include "QThread"
#include "QDebug"
#include "QPicture"

AttitudePanel::AttitudePanel(QWidget *parent): QWidget(parent)
{
    m_PitchValue = 0;
    m_RollValue  = 0;
    m_YawValue   = 0;
    m_CourseValue   = 0;

    m_GPSAltitudeValue = 0;
    m_PressureAltitudeValue = 0;

    m_GPSSpeedValue = 0;
    m_AirSpeedValue = 0;

    m_CtrlModeValue       = "自动";
    m_AirplaneModeValue   = "固定翼";
    m_CurrentStatusValue  = "起飞";
    m_LockStatusValue     = "未解锁";
    m_TurningStatusValue  = "直飞";

    m_GPSStatusValue = 0;//GPS无效
    m_GPSStarValue = 0;//卫星数0

    m_Protect = 0;

    m_Pitch_Roll_MinValue = -90;
    m_Pitch_Roll_MaxValue = +90;
    m_Yaw_MinValue        = -180;
    m_Yaw_MaxValue        = 360;
    m_CourseMinValue      = 0;
    m_CourseMaxValue      = 360;

    m_BatteryMinValue     = 21.2;//6s
    m_BatteryMaxValue     = 25.2;
    m_BatteryValue        = 25.8;

    m_GroundColor =   Qt::lightGray;
    m_ForeColor = Qt::darkGray;
    m_CentreLineColor =Qt::white;

    m_LedColor = Qt::green;

    setMinimumSize(QSize(10, 10));
    resize(100, 100);

    //QThread *PanelThread = new QThread;
    //this->moveToThread(PanelThread);
    //PanelThread->start();



}

void AttitudePanel::mousePressEvent(QEvent *event)
{
    qDebug() << "FFF";
    m_LedColor = Qt::blue;
}





void AttitudePanel::setValue(double Pitch,double Roll,double Yaw)
{
    if (Pitch < m_Pitch_Roll_MinValue)
        Q_EMIT errorSignal(OutOfRange);
    else if (Pitch > m_Pitch_Roll_MaxValue)
        Q_EMIT errorSignal(OutOfRange);
    else
        m_PitchValue = Pitch;

    if (Roll < m_Pitch_Roll_MinValue)
        Q_EMIT errorSignal(OutOfRange);
    else if (Roll > m_Pitch_Roll_MaxValue)
        Q_EMIT errorSignal(OutOfRange);
    else
        m_RollValue = Roll;

    if (Yaw < m_Yaw_MinValue)
        Q_EMIT errorSignal(OutOfRange);
    else if (Yaw > m_Yaw_MaxValue)
        Q_EMIT errorSignal(OutOfRange);
    else
        m_YawValue = Yaw;
    if(m_YawValue < 0)
    {
        m_YawValue += 360;
    }

    update();
}

void AttitudePanel::setCourseValue(double Course)
{
    if (Course < m_CourseMinValue)
        Q_EMIT errorSignal(OutOfRange);
    else if (Course > m_CourseMaxValue)
        Q_EMIT errorSignal(OutOfRange);
    else
        m_CourseValue = Course;
    update();
}






void AttitudePanel::setPlaneMode(QString Mode)
{
    //if((Mode == "固定翼")||(Mode == "多旋翼"))
     m_AirplaneModeValue = Mode;
   // else
    // m_AirplaneModeValue = "未分类";
    update();
}

void AttitudePanel::setCtrlMode(QString Mode)
{
    //if((Mode == "手动")||(Mode == "自动"))
     m_CtrlModeValue = Mode;
    //else
    // m_CtrlModeValue = "未知";
    update();
}
void AttitudePanel::setLockMode(QString Mode)
{
    if((Mode == "未解锁")||(Mode == "已解锁"))
     m_LockStatusValue = Mode;
    else
     m_LockStatusValue = "不定态";
    update();
}

void AttitudePanel::setTurningStatus(QString Mode)
{
    //if((Mode == "左转")||(Mode == "右转")||(Mode == "直线"))
     m_TurningStatusValue = Mode;
  //  else
   //  m_TurningStatusValue = "未知";
    update();
}

void AttitudePanel::setCurrentStatusMode(QString Mode)
{
    //if((Mode == "起飞")||(Mode == "过渡")||(Mode == "巡航")||(Mode == "降落"))
     m_CurrentStatusValue = Mode;
   // else
   //  m_CurrentStatusValue = "未知";
    update();
}

void AttitudePanel::setGPSStatus(int Status)
{
     m_GPSStatusValue = Status;
     update();
}

void AttitudePanel::setGPSStarNumber(int Number)
{
    if(Number>=0)
     m_GPSStarValue = Number;
    else
     m_GPSStarValue = -1;
    update();
}

void AttitudePanel::setGPSAltitude(double Altitude)
{
    m_GPSAltitudeValue = Altitude;
    update();
}
void AttitudePanel::setPressureAltitude(double Altitude)
{
    m_PressureAltitudeValue = Altitude;
    update();
}

void AttitudePanel::setGPSSpeed(double speed)
{
    m_GPSSpeedValue = speed;
    update();
}
void AttitudePanel::setAirSpeed(double speed)
{
    m_AirSpeedValue = speed;
    update();
}

void AttitudePanel::setBattery(double Battery)
{
    m_BatteryValue = Battery;
    update();
}

void AttitudePanel::setLed(QColor LED)
{
    m_LedColor = LED;
    update();
}


void AttitudePanel::setProtect(quint8 ProtectType, QString ProtectString)
{

    if(ProtectType != 0)
    {
        m_Protect       = ProtectType;
        m_ProtectString = ProtectString;
    }
    update();
}






void AttitudePanel::setPitchRollMinValue(int value)
{
    if (value > m_Pitch_Roll_MinValue)
        Q_EMIT errorSignal(MinValueError);
    else
        m_Pitch_Roll_MinValue = value;
    update();
}

void AttitudePanel::setPitchRollMaxValue(int value)
{
    if(value < m_Pitch_Roll_MinValue)
        Q_EMIT errorSignal(MaxValueError);
    else
        m_Pitch_Roll_MinValue = value;
    update();
}


void AttitudePanel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
    painter.setPen(Qt::NoPen);

    drawBackground(&painter);						/* 画背景 */
    drawForeground(&painter);						/* 画前景 */
    drawAwindow(&painter);                          /* 画姿态 */
    //drawScale(&painter);							/* 画刻度线 */
    drawCenterLine(&painter);                       /* 画中心线 */
    drawText(&painter);                             /* 画文字 */
    //drawGPS(&painter);
    drawHight(&painter);
    drawSpeed(&painter);
    drawBattery(&painter);
    drawLed(&painter);
    drawProtect(&painter);
}

void AttitudePanel::drawProtect(QPainter *painter)
{
    if(m_Protect != 0)
    {
        painter->save();
        painter->setPen(Qt::red);
        painter->drawText(-24,-40,m_ProtectString);
        painter->restore();
    }
    else
    {

    }
}

void AttitudePanel::drawLed(QPainter *painter)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(m_LedColor);
    painter->drawRoundedRect(-97, -97, 10, 10,5,5);
    painter->restore();
}



void AttitudePanel::drawBackground(QPainter *painter)
{
    m_GroundColor = Qt::lightGray;
    painter->save();
    painter->setBrush(m_GroundColor);
    painter->drawRect(-100, -100, 200, 200);
    painter->restore();
}
void AttitudePanel::drawForeground(QPainter *painter)
{
    static const QPointF points[8] = {
          QPointF(-75.0,-100.0),
          QPointF(-100.0,-75.0),
          QPointF(-100.0, 75.0),
          QPointF(-75.0, 100.0),
          QPointF( 75.0, 100.0),
          QPointF( 100.0, 75.0),
          QPointF( 100.0,-75.0),
          QPointF( 75.0,-100.0)
      };
    if(m_Protect != 0)
    {
        m_ForeColor = 0xff4500;
    }
    else
    {
        m_ForeColor = Qt::darkGray;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,8);
    painter->restore();
}

void AttitudePanel::drawAwindow(QPainter *painter)
{
    bool Get = 0;

    static QPointF Possiblepoints[4] = {//可能点
          QPointF(0.0,0.0),
          QPointF(0.0,0.0),
          QPointF(0.0,0.0),
          QPointF(0.0,0.0)
      };
    static QPointF Selectpoints[2] = {//选择点
          QPointF(0.0,0.0),
          QPointF(0.0,0.0)
      };
    static QPointF Usepoints[5] = {//使用点
           QPointF(0.0,0.0),
           QPointF(-70.0, 70.0),
           QPointF(-70.0,-70.0),
           QPointF( 70.0,-70.0),
           QPointF(0.0,0.0)
      };

    float y,x;
    //计算第一点y=-70
    if((-m_RollValue == 90)||(-m_RollValue == -90))
    {
        Possiblepoints[0].setX(7/9.0 * -m_PitchValue);
        Possiblepoints[0].setY(-70);

        Possiblepoints[1].setX(7/9.0 * -m_PitchValue);
        Possiblepoints[1].setY(70);
    }
    else
    {
        {
            y = -70;
            x = (y + 7/9.0 *-m_PitchValue)/tan(-m_RollValue * M_PI /180.0);
            Possiblepoints[0].setX(x);
            Possiblepoints[0].setY(y);
        }
        //计算第二点y=70
        {
            y = 70;
            x = (y + 7/9.0 * -m_PitchValue)/tan(-m_RollValue * M_PI /180.0);
            Possiblepoints[1].setX(x);
            Possiblepoints[1].setY(y);
        }
    }
    //计算第三点x=-70
    {
        x = -70;
        y = x*tan(-m_RollValue * M_PI /180.0) - 7/9.0 * -m_PitchValue;
        Possiblepoints[2].setX(x);
        Possiblepoints[2].setY(y);
    }
    //计算第四点x=70
    {
        x = 70;
        y = x*tan(-m_RollValue * M_PI /180.0) - 7/9.0 * -m_PitchValue;
        Possiblepoints[3].setX(x);
        Possiblepoints[3].setY(y);
    }
    //比较四点中x,y值范围正确的，去掉x值不在 -70<x<70,-70<y<70,之间的点
    if((Possiblepoints[0].x() <70)&&(Possiblepoints[0].x() > -70))
    {
        Selectpoints[0] = Possiblepoints[0];
        Get = 1;
    }

    if((Possiblepoints[1].x() <70)&&(Possiblepoints[1].x() > -70))
    {
       if(Get == 0)//没有取得第一点
       {
            Selectpoints[0] = Possiblepoints[1];
            Get = 1;
       }
       else if(Get == 1)//没有取得第二点
       {
           Selectpoints[1] = Possiblepoints[1];
           Get = 2;
       }
    }

    if((Possiblepoints[2].y() <70)&&(Possiblepoints[2].y() > -70))
    {
       if(Get == 0)//没有取得第一点
       {
            Selectpoints[0] = Possiblepoints[2];
            Get = 1;
       }
       else if(Get == 1)//没有取得第二点
       {
           Selectpoints[1] = Possiblepoints[2];
           Get = 2;
       }
    }

    if((Possiblepoints[3].y() <70)&&(Possiblepoints[3].y() > -70))
    {
       if(Get == 0)//没有取得第一点
       {
            Selectpoints[0] = Possiblepoints[3];
            Get = 1;
       }
       else if(Get == 1)//没有取得第二点
       {
           Selectpoints[1] = Possiblepoints[3];
           Get = 2;
       }
    }

    m_GroundColor = Qt::darkYellow;
    painter->save();
    painter->setBrush(m_GroundColor);
    painter->drawRect(-70.0, -70.0, 140, 140);

   if(((Selectpoints[0].x() + Selectpoints[1].x()) == 0)||
      ((Selectpoints[0].y() + Selectpoints[1].y()) == 0))//两个点不在相邻的线上
   {
      //四点
       if(-m_RollValue < -45)
       {
           Usepoints[0] = Selectpoints[0];

           Usepoints[1].setX(-70);
           Usepoints[1].setY(-70);

           Usepoints[2].setX(-70);
           Usepoints[2].setY( 70);

           Usepoints[3] = Selectpoints[1];
           m_GroundColor = Qt::blue;
           painter->setBrush(m_GroundColor);
           painter->drawPolygon(Usepoints,4);
       }
       else if((-m_RollValue > -45)&&(-m_RollValue < 45))
       {
           Usepoints[0] = Selectpoints[0];

           Usepoints[1].setX(-70);
           Usepoints[1].setY(-70);

           Usepoints[2].setX( 70);
           Usepoints[2].setY(-70);

           Usepoints[3] = Selectpoints[1];
           m_GroundColor = Qt::blue;
           painter->setBrush(m_GroundColor);
           painter->drawPolygon(Usepoints,4);
       }
       else if(-m_RollValue > 45)
       {
           Usepoints[0] = Selectpoints[0];

           Usepoints[1].setX( 70);
           Usepoints[1].setY(-70);

           Usepoints[2].setX( 70);
           Usepoints[2].setY( 70);

           Usepoints[3] = Selectpoints[1];
           m_GroundColor = Qt::blue;
           painter->setBrush(m_GroundColor);
           painter->drawPolygon(Usepoints,4);
       }
       else
       {
           if(m_RollValue == 45)
           {
               Usepoints[0].setX(-70);
               Usepoints[0].setY( 70);

               Usepoints[1].setX(-70);
               Usepoints[1].setY(-70);

               Usepoints[2].setX( 70);
               Usepoints[2].setY(-70);

               m_GroundColor = Qt::blue;
               painter->setBrush(m_GroundColor);
               painter->drawPolygon(Usepoints,3);
           }
           else
           {
               Usepoints[0].setX(-70);
               Usepoints[0].setY(-70);

               Usepoints[1].setX( 70);
               Usepoints[1].setY(-70);

               Usepoints[2].setX( 70);
               Usepoints[2].setY( 70);

               m_GroundColor = Qt::blue;
               painter->setBrush(m_GroundColor);
               painter->drawPolygon(Usepoints,3);
           }

       }
   }
   else//两个点在相邻的线上
   {
       if((Selectpoints[0].y() == 70)||(Selectpoints[1].y() == 70))
       {
           //5点
          Usepoints[0] = Selectpoints[0];
          if(-m_RollValue <0)
          {
              Usepoints[1].setX(-70);
              Usepoints[1].setY( 70);

              Usepoints[2].setX(-70);
              Usepoints[2].setY(-70);

              Usepoints[3].setX( 70);
              Usepoints[3].setY(-70);
          }
          else
          {
              Usepoints[1].setX( 70);
              Usepoints[1].setY( 70);

              Usepoints[2].setX( 70);
              Usepoints[2].setY(-70);

              Usepoints[3].setX(-70);
              Usepoints[3].setY(-70);
          }

          Usepoints[4] = Selectpoints[1];

          m_GroundColor = Qt::blue;
          painter->setBrush(m_GroundColor);
          painter->drawPolygon(Usepoints,5);
       }
       else if((Selectpoints[0].y() == -70)||(Selectpoints[1].y() == -70))
       {
           //3点
           Usepoints[0] = Selectpoints[0];
           if(-m_RollValue <0)
           {
               Usepoints[1].setX(-70);
               Usepoints[1].setY(-70);
           }
           else
           {
               Usepoints[1].setX( 70);
               Usepoints[1].setY(-70);
           }
           Usepoints[2] = Selectpoints[1];

           m_GroundColor = Qt::blue;
           painter->setBrush(m_GroundColor);
           painter->drawPolygon(Usepoints,3);
       }
   }
    painter->restore();
}

void AttitudePanel::drawScale(QPainter *painter)
{
    painter->save();
    int steps = 20;
    m_ForeColor = Qt::black;
    painter->setPen(m_ForeColor);
    QPen pen = painter->pen();
    for (int i = 0; i <= steps; i++)
    {
        if (i % 5 == 0)
        {
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(-70, 50-i*5, -62, 50-i*5);
            painter->drawLine( 70, 50-i*5,  62, 50-i*5);
        }
        else
        {
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawLine(-70, 50-i*5, -66, 50-i*5);
            painter->drawLine( 70, 50-i*5,  66, 50-i*5);
        }
    }
    painter->restore();
}

void AttitudePanel::drawCenterLine(QPainter *painter)
{
    static const QPointF points[7] = {
          QPointF(-60.0,  0.0),
          QPointF(-20.0,  0.0),
          QPointF(-10.0, 10.0),
          QPointF(  0.0,-20.0),
          QPointF( 10.0, 10.0),
          QPointF( 20.0,  0.0),
          QPointF( 60.0,  0.0)
      };

    painter->save();
    painter->setPen(m_CentreLineColor);
    painter->drawPolyline(points,7);
    painter->restore();
}

void AttitudePanel::drawText(QPainter *painter)
{
    painter->save();
    painter->setPen(m_CentreLineColor);
    painter->drawText(-60, 55,"Pitch");
    painter->drawText(-60, 65,QString::number(m_PitchValue,'f',1));
    painter->drawText( 35, 55,"Roll");
    painter->drawText( 35, 65,QString::number(m_RollValue,'f',1));
    painter->drawText(-10, 55,"Yaw");
    painter->drawText(-10, 65,QString::number(m_YawValue,'f',1));



    painter->drawText(-68,-60,"GPS:");
    if(m_GPSStatusValue == 0)
        painter->drawText(-45,-60,tr("无效"));
    else
        painter->drawText(-45,-60,tr("正常"));
    painter->drawText(-68,-48,"NSV:");
    painter->drawText(-45,-48,QString::number(m_GPSStarValue,'f',0));

    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-20,-88,40, 16);

    painter->drawText(-12,-76,QString::number(m_CourseValue,'f',1));

    //飞机模式
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-70, 72,40, 16);
    painter->drawText(-68, 84,m_AirplaneModeValue);
    //锁定状态
    if(m_LockStatusValue == "已解锁")
    m_GroundColor = Qt::darkRed;
    else if(m_LockStatusValue == "未解锁")
    m_GroundColor = Qt::darkGreen;
    else
    m_GroundColor = Qt::darkYellow;

    painter->setBrush(m_GroundColor);
    painter->drawRect(-20, 72,40, 16);
    painter->drawText(-18, 84,m_LockStatusValue);
    //控制模式
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect( 30, 72,40, 16);
    painter->drawText( 38, 84,m_CtrlModeValue);

    //飞行状态
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-70,-88,40, 16);
    painter->drawText(-62,-76,m_CurrentStatusValue);

    //转弯状态
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect( 30,-88,40, 16);
    painter->drawText( 38,-76,m_TurningStatusValue);
    painter->restore();


}

void AttitudePanel::drawGPS(QPainter *painter)
{
    static const QPointF points[4] = {
          QPointF(-75.0,-90.0),
          QPointF(-90.0,-75.0),
          QPointF(-90.0, 75.0),
          QPointF(-75.0, 90.0)
      };
    QColor GPScolor = Qt::black;
    painter->save();
    painter->setBrush(GPScolor);
    painter->drawPolygon(points,8);
    painter->restore();
}

void AttitudePanel::drawHight(QPainter *painter)
{
    static const QPointF points[4] = {
          QPointF(-80.0,-50.0),
          QPointF(-50.0,-30.0),
          QPointF(-50.0, 30.0),
          QPointF(-80.0, 50.0)
      };

    if(m_Protect != 0)
    {
        m_GroundColor = 0xff4500;
    }
    else
    {
        m_GroundColor = Qt::darkCyan;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,4);
    painter->restore();

    painter->save();
    painter->setPen(m_CentreLineColor);
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-88,-20,35, 16);
    painter->drawText(-86,-8,QString::number(m_GPSAltitudeValue,'f',1));

    painter->drawText(-88,-25,tr("海拔"));

    painter->drawRect(-88,4,35, 16);
    painter->drawText(-86,17,QString::number(m_PressureAltitudeValue,'f',1));

    painter->drawText(-88,33,tr("气压"));

    painter->restore();
}

void AttitudePanel::drawSpeed(QPainter *painter)
{
    static const QPointF points[4] = {
          QPointF( 80.0,-50.0),
          QPointF( 50.0,-30.0),
          QPointF( 50.0, 30.0),
          QPointF( 80.0, 50.0)
      };

    if(m_Protect != 0)
    {
        m_GroundColor = 0xff4500;
    }
    else
    {
        m_GroundColor = Qt::darkCyan;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,4);
    painter->restore();

    painter->save();
    painter->setPen(m_CentreLineColor);
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect( 53,-20,35, 16);
    painter->drawText( 55,-8,QString::number(m_AirSpeedValue,'f',1));

    painter->drawText( 64,-25,tr("空速"));

    painter->drawRect( 53,4,35, 16);
    painter->drawText( 55,17,QString::number(m_GPSSpeedValue,'f',1));

    painter->drawText( 64,33,tr("地速"));

    painter->restore();
}

void AttitudePanel::drawBattery(QPainter *painter)
{
    static const QPointF points[8] = {
          QPointF( 68.0,-68.0),
          QPointF( 68.0,-52.0),
          QPointF( 36.0,-52.0),
          QPointF( 36.0,-56.0),
          QPointF( 34.0,-56.0),
          QPointF( 34.0,-64.0),
          QPointF( 36.0,-64.0),
          QPointF( 36.0,-68.0)
      };




    if(m_BatteryValue < m_BatteryMinValue)
    {
       m_ForeColor = Qt::red;
    }
    else if(m_BatteryValue > m_BatteryMaxValue)
    {
       m_ForeColor = Qt::green;
    }
    else
    {
       m_ForeColor = Qt::white;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,8);
    painter->restore();

    painter->save();
    painter->setPen(m_CentreLineColor);
    painter->drawText( 37,-42,QString::number(m_BatteryValue,'f',1)+"V");
    painter->restore();

    double B;
    B = (m_BatteryValue - m_BatteryMinValue)/(m_BatteryMaxValue - m_BatteryMinValue);
    quint8 Flag = 0;
    if(B<=0.25)
        Flag = 1;
    else if(B <= 0.5)
        Flag = 2;
    else if(B<=0.75)
        Flag = 3;
    else
        Flag = 4;
    switch(Flag)
    {
        case 4:
             {
                    m_ForeColor = Qt::black;
                    painter->save();
                    painter->setBrush(m_ForeColor);
                    painter->drawRect(37,-66,6,12);
                    painter->restore();
             }
        case 3:
              {
                     m_ForeColor = Qt::black;
                     painter->save();
                     painter->setBrush(m_ForeColor);
                     painter->drawRect(45,-66,6,12);
                     painter->restore();
              }
        case 2:
              {
                     m_ForeColor = Qt::black;
                     painter->save();
                     painter->setBrush(m_ForeColor);
                     painter->drawRect(53,-66,6,12);
                     painter->restore();
              }
        case 1:
              {
                     m_ForeColor = Qt::black;
                     painter->save();
                     painter->setBrush(m_ForeColor);
                     painter->drawRect(61,-66,6,12);
                     painter->restore();
              }

    }



}





