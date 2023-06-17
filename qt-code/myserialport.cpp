#include "myserialport.h"
#include <QSerialPort>
#include <QDebug>
#include <QSerialPortInfo>
#include "mainwindow.h"
MySerialPort::MySerialPort(QObject *parent) : QObject(parent)
{
    serial=new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead, this, &MySerialPort::readData);
    //connect(serial, &QSerialPort::error, this, &MySerialPort::handleError);

        field = QByteArray();
}
void MySerialPort::openSerialPort()
{

    serial->setBaudRate(QSerialPort::Baud9600);
    if(serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"opened";
    }
    else
    {
        qDebug()<<QSerialPort::PermissionError;
    }

}
void MySerialPort::closeSerialPort()
{
    if(serial->isOpen())
    {
        serial->close();
    }
}
void MySerialPort::writeData(const QByteArray &data)
{
    serial->write(data);

}
void MySerialPort::readData()
{
    if(serial){
    this->setField(serial->readAll());
    //MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
    //qDebug()<<"msp"<<" "<<field;
}
}
void MySerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        qDebug()<<"no error";
    }
}

void MySerialPort::setField(QByteArray newValue) {
        field = newValue;
        emit fieldChanged(field);
}
