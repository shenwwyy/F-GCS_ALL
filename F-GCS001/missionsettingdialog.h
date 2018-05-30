#ifndef MISSIONSETTINGDIALOG_H
#define MISSIONSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class MissionSettingDialog;
}

class MissionSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MissionSettingDialog(QWidget *parent = 0, int Column = 0);
    ~MissionSettingDialog();

signals:
       void isWindowsClose(bool);

public Q_SLOTS:



private slots:


private:
    Ui::MissionSettingDialog *ui;
};

#endif // MISSIONSETTINGDIALOG_H
