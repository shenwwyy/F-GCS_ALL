#ifndef MAPSETTINGDIALOG_H
#define MAPSETTINGDIALOG_H

#include <QDialog>
#include "opmapwidget.h"
#include "QDir"
#include "QSettings"
#include "inifile.h"

namespace Ui {
class MapSettingDialog;
}

class MapSettingDialog : public QDialog
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 6 };

    explicit MapSettingDialog(QWidget *parent = 0);
    ~MapSettingDialog();


signals:
    void isWindowClose(char);
    void SetMapType(QString);


private slots:
    void on_ConfirmButton_clicked();

private:
    Ui::MapSettingDialog *ui;
};

#endif // MAPSETTINGDIALOG_H
