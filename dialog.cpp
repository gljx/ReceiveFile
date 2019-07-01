#include "dialog.h"
#include "ui_dialog.h"
#include"QFileDialog"
#include"QFileInfo"
#include"QDebug"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    portinfo = new QSerialPortInfo();
    ports = portinfo->availablePorts();
    port=new QSerialPort;
    port->setPort(ports.at(1));
    port->setPortName(ports.at(1).systemLocation());
    ui->setupUi(this);
    connect(ui->SavePathBtn,SIGNAL(clicked()),this,SLOT(onSavePathClicked()));
    QObject::connect(port,&QSerialPort::readyRead,this,&Dialog::Read_Data);
}

Dialog::~Dialog()
{
    port->close();
    delete ui;
}

void Dialog::Read_Data(){
        qDebug()<<"read data"<<endl;
        QByteArray data = port->readAll();
        QString temp=path+"/"+"data.txt";
        foreach (char bytes,data) {
            qDebug()<<bytes<<endl;
        }
        QFile file(temp);
        file.open(QIODevice::Append);
        if(file.isOpen())
        {
            file.write(data);
            file.close();
            qDebug()<<"file download"<<endl;
        }
}

void Dialog::onSavePathClicked(){

     path=QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                       QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
     qDebug()<<path<<endl;
     if(port->open(QIODevice::ReadWrite)){
         qDebug()<<"open port receive"<<endl;
     }else{
         qDebug()<<"cannon open port receive"<<endl;
     }

}
