#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString path;
    QSerialPort *port;
    QSerialPortInfo *portinfo;
    QList<QSerialPortInfo> ports;
    void savedata(QByteArray data);
    void getMsg(QByteArray datablock);
    int blocknum=0;
    int totalblocknum=0;
    QString filename;
    int byteToInt2(QByteArray b,int length);
private slots:
    void onSavePathClicked();
    void Read_Data();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
