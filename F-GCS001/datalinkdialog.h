#ifndef DATALINKDIALOG_H
#define DATALINKDIALOG_H

#include <QDialog>
#include "qextserialenumerator.h"
#include "qextserialport.h"
#include "inifile.h"

namespace Ui {
class DataLinkDialog;
}

class DataLinkDialog : public QDialog
{
    Q_OBJECT

public:
     enum WindowsId{ ID = 1 };

    explicit DataLinkDialog(QWidget *parent = 0);
    ~DataLinkDialog();

private slots:
    void hasComDiscovered(const QextPortInfo &info);
    void hasComRemoved(const QextPortInfo &info);

    void on_FlushButton_clicked();
    void on_ConnectButton_clicked();
    void UpdateWindow(bool flag);

signals:
    void isWindowClose(char);
    void SerialPortInfo(QString PortName,BaudRateType Baudrate,bool CMD);


private:
    Ui::DataLinkDialog *ui;
    QextSerialEnumerator *SerialPort_Monitor;




};

#endif // DATALINKDIALOG_H
