#include "mapsettingdialog.h"
#include "ui_mapsettingdialog.h"
#include "QDebug"

MapSettingDialog::MapSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapSettingDialog)
{
    ui->setupUi(this);

    this->setWhatsThis(tr("地图使用选择窗口"));

    ui->comboBox->addItems(mapcontrol::Helper::MapTypes());
    ui->comboBox->setWhatsThis(tr("请选择需要使用的地图类型"));
    IniFile *ini = new IniFile();
    QString Type = ini->ReadIni("FGCS.ini","Map","Type");
    delete ini;
    ui->comboBox->setCurrentText(Type);

    ui->ConfirmButton->setWhatsThis(tr("点击确认选择使用当前地图"));

}

MapSettingDialog::~MapSettingDialog()
{
    emit isWindowClose(ID);
    delete ui;
}

void MapSettingDialog::on_ConfirmButton_clicked()
{
   IniFile *ini = new IniFile();
   ini->WriteIni("FGCS.ini","Map","Type",ui->comboBox->currentText());
   delete ini;
   emit SetMapType(ui->comboBox->currentText());
}

