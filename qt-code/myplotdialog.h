#ifndef MYPLOTDIALOG_H
#define MYPLOTDIALOG_H

#include <QDialog>
#include <stack>
class QLabel;
class MySerialPort;
class QGraphicsScene;
class QGraphicsEllipseItem;
class QGraphicsView;
class QSqlDatabase;
class MyPlotDialog : public QDialog
{
    Q_OBJECT



public:
    explicit MyPlotDialog(QWidget *parent = nullptr);
    ~MyPlotDialog();
    void setParameters(QString firstLabel, QString secondLabel, int xmin,
                       int xmax, int ymin, int ymax, MySerialPort* serial);
    // Declare any additional member functions or signals/slots as needed
    void drawPlane();
    void updateField(QByteArray newFieldValue);
    void retrieveEllipsePositions();
    void connectMySQL();
    void sendQuery(QString);
    void runPythonScript(QString, int);
    QByteArray fieldA;
private:
    QSqlDatabase* pdb;
    QString firstLabel, secondLabel;
    int xmin, xmax,ymin, ymax;
    QGraphicsScene *scene;
    MySerialPort* serial;
    QGraphicsEllipseItem* point;
    QGraphicsView* view;
    QGraphicsEllipseItem* ellipse;
    QLabel* textLabel;
    std::stack<QGraphicsEllipseItem*> ellipseStack;
    std::stack<std::pair<float, float>> xystack;
    QString lastTime ="";
    int lastK=0;
};


#endif // MYPLOTDIALOG_H
