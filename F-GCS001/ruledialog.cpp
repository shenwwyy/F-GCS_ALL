#include "ruledialog.h"
#include "ui_ruledialog.h"
#include "qmessagebox.h"

RuleDialog::RuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleDialog)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    setUrlFactory = new core::UrlFactory;
    CurrentPoint = 1;
}

RuleDialog::~RuleDialog()
{
    emit isWindowClose(ID);


    delete setUrlFactory;
    delete Box1;
    delete ui;
}

double RuleDialog::GetDiatance(internals::PointLatLng P1,internals::PointLatLng P2)
{
    double EarthRadiusKm = 6378.137;

    double dLat1InRad  = P1.Lat() * (M_PI / 180);
    double dLong1InRad = P1.Lng() * (M_PI / 180);
    double dLat2InRad  = P2.Lat() * (M_PI / 180);
    double dLong2InRad = P2.Lng() * (M_PI / 180);
    double dLongitude  = dLong2InRad - dLong1InRad;
    double dLatitude   = dLat2InRad - dLat1InRad;
    double a = pow(sin(dLatitude / 2), 2) + cos(dLat1InRad) * cos(dLat2InRad) * pow(sin(dLongitude / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double dDistance   = EarthRadiusKm * c;

    return dDistance;
}

double RuleDialog::GetHeading(internals::PointLatLng P1,internals::PointLatLng P2)
{
        double temp;
        double mLat = P2.Lat() - P1.Lat();
        double mLon = (P2.Lng() - P1.Lng())* cos(((P2.Lat() + P1.Lat())/2)* 0.0174532925);
        temp = 90.0 + atan2(-mLat, mLon) * 57.2957795;
        if(temp < 0)temp += 360.0;
        return temp;
}

void RuleDialog::RulePoint(internals::PointLatLng LatLng)
{
    switch(CurrentPoint) {
    case 1:{
            Point1.SetLat(LatLng.Lat());
            Point1.SetLng(LatLng.Lng());
            ui->FirstPointLatitude->setText(QString::number(LatLng.Lat(),'f',8));
            ui->FirstPointLongitude->setText(QString::number(LatLng.Lng(),'f',8));
            CurrentPoint = 2;
             } break;
    case 2:{
            Point2.SetLat(LatLng.Lat());
            Point2.SetLng(LatLng.Lng());
            ui->SecondPointLatitude->setText(QString::number(LatLng.Lat(),'f',8));
            ui->SecondPointLongitude->setText(QString::number(LatLng.Lng(),'f',8));
             } break;
    default: LatLng = LatLng;
        break;
    }
}

void RuleDialog::ShowMousePos(internals::PointLatLng LatLng)
{
    ui->MouseLatitude->setText(QString::number(LatLng.Lat(),'f',8));
    ui->MouseLongitude->setText(QString::number(LatLng.Lng(),'f',8));
}


void RuleDialog::on_SelectFirstPointButton_clicked()
{
    CurrentPoint = 1;
    emit GetPoint(1);
}

void RuleDialog::on_SelectSecondPointButton_clicked()
{
    CurrentPoint = 2;
    emit GetPoint(2);
}

void RuleDialog::on_ConformButton_clicked()
{
         double Distance;
         Distance = GetDiatance(Point1,Point2);
         ui->TwoPointDistance->setText(QString::number(Distance,'f',6) + "km");

         double Heading;
         Heading = GetHeading(Point1,Point2);
         ui->TwoPointAngle->setText(QString::number(Heading,'f',2) + "°");

         CurrentPoint = 1;//获取结束后从新选择第一点
}
