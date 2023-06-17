#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "myplotdialog.h"
class MySerialPort;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void createSerial();
    void writeSerialPort();
    void readSerialPort();
    void setDefaultConfig();
    void createSubWindow();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    QLabel *firstParLabel;
    QLabel *secondParLabel;
    QLabel *xMinLabel;
    QLabel *xMaxLabel;
    QLabel *yMinLabel;
    QLabel *yMaxLabel;

    QLineEdit *firstParInput;
    QLineEdit *secondParInput;
    QLineEdit *xMinInput;
    QLineEdit *xMaxInput;
    QLineEdit *yMinInput;
    QLineEdit *yMaxInput;

    QPushButton* readButton;
    QPushButton* serialButton;
    QPushButton* defaultButton;

    QLabel *imageLabel;
    QPixmap *imagePixmap;

    QComboBox*  m_comPortComboBox;
    MySerialPort* serialPort = nullptr;

    QPushButton* confirmButton;
    MyPlotDialog *myPlotDialog;


    QDialog *subWindow;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
