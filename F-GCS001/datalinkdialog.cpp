#include "datalinkdialog.h"
#include "ui_datalinkdialog.h"
#include "QSlider"
#include "QDir"
#include "QSettings"
#include "QDebug"
#include "QComboBox"

#define Q_WS_WIN

DataLinkDialog::DataLinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataLinkDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    SerialPort_Monitor = new QextSerialEnumerator();
    SerialPort_Monitor->setUpNotifications();

    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
    ui->PortNameBox->addItem(info.portName);
    //make sure user can input their own port name!
    ui->PortNameBox->setEditable(true);

    ui->BaudRateBox->addItem("110", BAUD110);
    ui->BaudRateBox->addItem("300", BAUD300);
    ui->BaudRateBox->addItem("600", BAUD600);
    ui->BaudRateBox->addItem("1200", BAUD1200);
    ui->BaudRateBox->addItem("2400", BAUD2400);
    ui->BaudRateBox->addItem("4800", BAUD4800);
    ui->BaudRateBox->addItem("9600", BAUD9600);
    ui->BaudRateBox->addItem("19200", BAUD19200);
    ui->BaudRateBox->addItem("38400", BAUD38400);
    ui->BaudRateBox->addItem("57600", BAUD57600);
    ui->BaudRateBox->addItem("115200", BAUD115200);

#ifdef Q_WS_WIN
    ui->BaudRateBox->addItem("14400", BAUD14400);
    ui->BaudRateBox->addItem("56000", BAUD56000);
    ui->BaudRateBox->addItem("128000", BAUD128000);
    ui->BaudRateBox->addItem("256000", BAUD256000);
#endif
    ui->BaudRateBox->setEditable(true);
    //ui->BaudRateBox->setCurrentIndex(10);

    connect(SerialPort_Monitor,SIGNAL(deviceDiscovered(const QextPortInfo&)),this,SLOT(hasComDiscovered(const QextPortInfo&)));
    connect(SerialPort_Monitor,SIGNAL(deviceRemoved(const QextPortInfo&)),this,SLOT(hasComRemoved(const QextPortInfo&)));

    IniFile *ini = new IniFile();
    QString Name = ini->ReadIni("FGCS.ini","Serial","Name");
    QString Baud = ini->ReadIni("FGCS.ini","Serial","Baud");
    delete ini;

    ui->PortNameBox->setCurrentText(Name);
    ui->BaudRateBox->setCurrentIndex(ui->BaudRateBox->findText(Baud));



   // qDebug() << ui->BaudRateBox->currentIndex();

}
DataLinkDialog::~DataLinkDialog()
{
    emit isWindowClose(ID);
    delete SerialPort_Monitor;
    delete ui;
}


/************************************************
  *类型：槽函数
  *对应信号：有新的串口设备被发现
  *功能：在串口下拉列表中加入改串口设备
  *************************************************/
void DataLinkDialog::hasComDiscovered(const QextPortInfo &info)
{
    ui->PortNameBox->addItem(info.portName);
    ui->PortNameBox->setCurrentIndex(ui->PortNameBox->count()-1);
}

/************************************************
  *类型：槽函数
  *对应信号：有串口设备断开连接
  *功能：如果断开的设备是当前打开的设备，就关闭改串口及定时器
        提示用户
        如果不是当前设备，就在串口下拉列表中删除该设备
  *************************************************/
void DataLinkDialog::hasComRemoved(const QextPortInfo &info)
{
    if(info.portName==ui->PortNameBox->currentText())
    {
        emit SerialPortInfo(ui->PortNameBox->currentText(),
                            (BaudRateType)ui->BaudRateBox->itemData(ui->BaudRateBox->currentIndex()).toInt(),
                            false);
    }
    for(int i=0;i<ui->PortNameBox->count();i++)
    {
        if(ui->PortNameBox->itemText(i)==info.portName)
            ui->PortNameBox->removeItem(i);
    }
}



void DataLinkDialog::on_FlushButton_clicked()
{
    ui->PortNameBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
    ui->PortNameBox->addItem(info.portName);
}

void DataLinkDialog::on_ConnectButton_clicked()
{
    IniFile *ini = new IniFile();
    ini->WriteIni("FGCS.ini","Serial","Name",ui->PortNameBox->currentText());
    ini->WriteIni("FGCS.ini","Serial","Baud",ui->BaudRateBox->currentText());
    delete ini;

    emit SerialPortInfo(ui->PortNameBox->currentText(),
                        (BaudRateType)ui->BaudRateBox->itemData(ui->BaudRateBox->currentIndex()).toInt(),
                        true);
}

void DataLinkDialog::UpdateWindow(bool flag)
{
   if(flag == true)
   {
        ui->PortNameBox->setEnabled(false);
        ui->BaudRateBox->setEnabled(false);
        ui->FlushButton->setEnabled(false);
        ui->ConnectButton->setText(tr("断开"));
   }
   else
   {
       ui->PortNameBox->setEnabled(true);
       ui->BaudRateBox->setEnabled(true);
       ui->FlushButton->setEnabled(true);
       ui->ConnectButton->setText(tr("连接"));
   }
}

