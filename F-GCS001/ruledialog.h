#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include <QDialog>
#include "urlfactory.h"
#include "opmapwidget.h"
#include "qmessagebox.h"

namespace Ui {
class RuleDialog;
}

class RuleDialog : public QDialog
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 5 };

    explicit RuleDialog(QWidget *parent = 0);
    ~RuleDialog();




signals:
    void isWindowClose(char);
    void GetPoint(quint8);


private slots:
    double GetDiatance(internals::PointLatLng P1,internals::PointLatLng P2);
    double GetHeading(internals::PointLatLng P1,internals::PointLatLng P2);

    void RulePoint(internals::PointLatLng LatLng);
    void ShowMousePos(internals::PointLatLng LatLng);

    void on_SelectFirstPointButton_clicked();

    void on_SelectSecondPointButton_clicked();

    void on_ConformButton_clicked();

private:
    Ui::RuleDialog *ui;

    core::UrlFactory *setUrlFactory;

    internals::PointLatLng Point1;
    internals::PointLatLng Point2;

    quint8 CurrentPoint;

    QMessageBox *Box1;

};

#endif // RULEDIALOG_H
