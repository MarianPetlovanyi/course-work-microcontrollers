#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include<QMainWindow>
class MySerialPort : public QObject
{
    Q_OBJECT


public slots:
    void setField(QByteArray newValue);



public:
    explicit MySerialPort(QObject *parent = nullptr);
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &);
    void updateField(QByteArray newFieldValue);
    QByteArray data;

signals:
    void fieldChanged(QByteArray newFieldValue);


public slots:
    void readData();
    void handleError(QSerialPort::SerialPortError);
public:
    QSerialPort *serial;
    QByteArray field;
};

#endif // MYSERIALPORT_H
