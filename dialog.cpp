#include "dialog.h"
#include "ui_dialog.h"
#include"QFileDialog"
#include"QFileInfo"
#include"QDebug"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    path="c:";
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
        QByteArray datablock=port->readAll();
        if(blocknum==0){
            qDebug()<<"LOG1:"<<blocknum<<endl;
            getMsg(datablock);
            blocknum++;
            qDebug()<<"LOG7:"<<blocknum<<endl;
        }
        if(blocknum==totalblocknum){
            qDebug()<<"LOG2:"<<blocknum<<endl;
            ui->textLog->append("传输完毕\n");
            blocknum=0;
            totalblocknum=0;
        }else{
            qDebug()<<"LOG3:"<<blocknum<<endl;
            if(blocknum!=0){
                qDebug()<<"LOG4:"<<blocknum<<endl;
               savedata(datablock);
               qDebug()<<"LOG5:"<<blocknum<<endl;
            }

        }
        qDebug()<<"LOG6:"<<blocknum<<endl;
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

void Dialog::savedata(QByteArray data){
    QString temp=path+"/"+filename;
    qDebug()<<temp<<endl;
    QFile file(temp);
    file.open(QIODevice::Append);
    if(file.isOpen())
    {
        ui->textLog->append("正在保存第"+QString("%1").arg(blocknum)+"块\n");
        file.write(data);
        file.close();
        blocknum++;
        qDebug()<<"file download"<<endl;
    }
}

void Dialog::getMsg(QByteArray datablock){
    int base=256;
    QByteArray total_byte;
    total_byte.resize(4);
    for(int i=0;i<4;i++){
        total_byte[i]=datablock[base+i];
    }
    totalblocknum = byteToInt2(total_byte,total_byte.length())+1;
    ui->textLog->append("共有"+QString("%1").arg(totalblocknum)+"个文件块\n");
    base=260;
    QByteArray filenameBytes;
    for(int i=0;i<100;i++){
        if(datablock[base+i]==0xff){
            break;
        }else{
            filenameBytes[i]=datablock[base+i];
        }
    }
    filename=filenameBytes;
    ui->textLog->append("正在接受"+filename);
}

int Dialog::byteToInt2(QByteArray b,int length) {
    int mask=0xff;
    int temp=0;
    int n=0;
    for(int i=0;i<length;i++){
       n<<=8;
       temp=b[i]&mask;
       n|=temp;
   }
return n;
}
