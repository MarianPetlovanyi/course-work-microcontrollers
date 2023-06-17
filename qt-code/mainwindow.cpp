#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <myserialport.h>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QDir>
#include <QDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QFormLayout *formLayout = new QFormLayout(centralWidget);

    m_comPortComboBox = new QComboBox(this);
    m_comPortComboBox->setMaximumWidth(100);
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : serialPortInfos)
    {
        m_comPortComboBox->addItem(info.portName());
    }



    serialButton = new QPushButton(this);
    serialButton->setText("Set port");
    serialButton->setEnabled(true);
    serialButton->raise();

    connect(serialButton, SIGNAL(clicked(bool)), this, SLOT(createSerial()));

    formLayout->addRow(serialButton, m_comPortComboBox);






    firstParLabel = new QLabel("First label((X by default)", this);
    firstParInput = new QLineEdit(this);
    firstParInput->setMaximumWidth(100);
    formLayout->addRow(firstParLabel, firstParInput);

    secondParLabel = new QLabel("Second label((Y by default)", this);
    secondParInput = new QLineEdit(this);
    secondParInput->setMaximumWidth(100);
    formLayout->addRow(secondParLabel, secondParInput);


    // X min input box
    xMinLabel = new QLabel("X min:", this);
    xMinInput = new QLineEdit(this);
    xMinInput->setMaximumWidth(100);
    formLayout->addRow(xMinLabel, xMinInput);

    // X max input box
    xMaxLabel = new QLabel("X max:", this);
    xMaxInput = new QLineEdit(this);
    xMaxInput->setMaximumWidth(100);
    formLayout->addRow(xMaxLabel, xMaxInput);

    // Y min input box
    yMinLabel = new QLabel("Y min:", this);
    yMinInput = new QLineEdit(this);
    yMinInput->setMaximumWidth(100);
    formLayout->addRow(yMinLabel, yMinInput);

    // Y max input box
    yMaxLabel = new QLabel("Y max:", this);
    yMaxInput = new QLineEdit(this);
    yMaxInput->setMaximumWidth(100);
    formLayout->addRow(yMaxLabel, yMaxInput);




    confirmButton = new QPushButton(this);
    confirmButton->setText("Confirm configuration");
    //confirmButton->setMaximumWidth(290);
    connect(confirmButton, SIGNAL(clicked(bool)), this, SLOT(createSubWindow()));

    defaultButton = new QPushButton(this);
    defaultButton->setMaximumWidth(100);
    defaultButton->setText("Default config");
    connect(defaultButton, SIGNAL(clicked(bool)), this, SLOT(setDefaultConfig()));


    formLayout->addRow(confirmButton, defaultButton);



}

void MainWindow::createSubWindow(){
    myPlotDialog = new MyPlotDialog(this);
    if(!serialPort){
        createSerial();
    }
    if(xMinInput->text().toInt()>xMaxInput->text().toInt() ||
            yMinInput->text().toInt()>yMaxInput->text().toInt()){
        this->setDefaultConfig();
    }
    myPlotDialog->setParameters(firstParInput->text(), secondParInput->text(),xMinInput->text().toInt(),
                                    xMaxInput->text().toInt(), yMinInput->text().toInt(), yMaxInput->text().toInt(), this->serialPort);



    myPlotDialog->setWindowTitle(firstParInput->text()+"/"+secondParInput->text());
    myPlotDialog->drawPlane();
    myPlotDialog->exec();

//    subWindow = new QDialog(this);
//    subWindow->setWindowTitle(firstParInput->text()+"/"+secondParInput->text());
//    subWindow->exec();
}

void MainWindow::setDefaultConfig(){
    firstParInput->setText("X");
    secondParInput->setText("Y");
    xMinInput->setText("-10");
    xMaxInput->setText("10");
    yMinInput->setText("-10");
    yMaxInput->setText("10");

}

void MainWindow::createSerial(){
    serialPort = new MySerialPort(this);
    serialPort->serial->setPortName(m_comPortComboBox->currentText());

    qDebug()<<serialPort->serial->portName();

    serialPort->openSerialPort();

}

void MainWindow::readSerialPort(){
    serialPort->readData();
}


void MainWindow::writeSerialPort()
{
    QByteArray data;
    data.append(0xB0);
    serialPort->writeData(data);
}


MainWindow::~MainWindow()
{
    delete ui;
}

