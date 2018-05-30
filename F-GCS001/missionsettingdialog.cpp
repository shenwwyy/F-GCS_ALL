#include "missionsettingdialog.h"
#include "ui_missionsettingdialog.h"
#include "QDebug"

MissionSettingDialog::MissionSettingDialog(QWidget *parent,int Column) :
    QDialog(parent),
    ui(new Ui::MissionSettingDialog)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    qDebug() << Column;
}

MissionSettingDialog::~MissionSettingDialog()
{
    emit isWindowsClose(true);
    qDebug() << "emit";

    this->close();
    delete ui;
}

