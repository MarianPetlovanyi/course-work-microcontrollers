#include "myplotdialog.h"
#include "myserialport.h"
#include "xyplanewidget.h"
#include <cmath>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QApplication>
#include<QGraphicsSimpleTextItem>
#include <QGraphicsProxyWidget>
#include <QThread>
#include <QLabel>
#include <QProcess>
#include <stack>

#include <QtSql>


std::vector<double> linspace(double start, double end, int numPoints) {
    std::vector<double> result;
    double step = (end - start) / (numPoints - 1);

    for (int i = 0; i < numPoints; ++i) {
        double value = start + step * i;
        result.push_back(value);
    }

    return result;
}


QString getRealTime(){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd_MM_yyyy_hh_mm_ss");
    return formattedTime;
}


MyPlotDialog::MyPlotDialog(QWidget *parent)
    : QDialog(parent)
{
    this->resize(1000,1000);
}
void MyPlotDialog::setParameters(QString firstLabel, QString secondLabel, int xmin,
                                 int xmax, int ymin, int ymax, MySerialPort* serial)
{
    this->firstLabel=firstLabel;
    this->secondLabel=secondLabel;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;
    this->serial=serial;
    connect(this->serial, &MySerialPort::fieldChanged, this, &MyPlotDialog::updateField);

}
void MyPlotDialog::updateField(QByteArray newFieldValue) {
        fieldA = newFieldValue.trimmed(); // Update fieldA in class A with the new field value
        QString dataString(fieldA);

        int x = dataString.mid(dataString.indexOf('=') + 1, dataString.indexOf(',') - dataString.indexOf('=') - 1).trimmed().toInt();
        int y = dataString.mid(dataString.lastIndexOf('=') + 1, dataString.lastIndexOf('\r') - dataString.lastIndexOf('=') - 1).trimmed().toInt();
        QPointF scenePos = ellipse->mapToScene(500, 500);
        float trueX = (scenePos.x()*(std::abs(this->xmax-this->xmin)))/(1000)+xmin;
        float trueY = ((1000-scenePos.y())*(std::abs(this->ymax-this->ymin)))/(1000)+ymin;


        //qDebug() << "Mapped scene position (x, y):" << scenePos.x() << scenePos.y();

        if(fieldA=="A"){
            QGraphicsEllipseItem* newEllipse = scene->addEllipse(scenePos.x(), scenePos.y(), 5, 5);
            newEllipse->setBrush(Qt::black);
            qDebug()<<"Push xy"<<trueX<<" "<<trueY;
            if(trueX>xmin && trueX<xmax && trueY>ymin && trueY<ymax){
                xystack.push(std::make_pair(trueX,trueY));

            }
            ellipseStack.push(newEllipse);
            ellipse->setPos(ellipse->x(), ellipse->y());
        }else if(fieldA=="B"){
            if(!ellipseStack.empty()){
                QGraphicsEllipseItem* temp = ellipseStack.top();
                xystack.pop();
                ellipseStack.pop();
                scene->removeItem(temp);
            }
        }else if(fieldA=="C"){
            QString time = getRealTime();
            lastTime = time;
            qDebug()<<"Last time "<< lastTime;
            QString createTableQuery = QString("CREATE TABLE IF NOT EXISTS points_table_%1 ("
                                               "id INT PRIMARY KEY AUTO_INCREMENT,"
                                               "x float,"
                                               "y float)").arg(time);

            sendQuery(createTableQuery);
            std::stack<std::pair<float,float>> temp = xystack;
            while(!temp.empty()){
                float tempX = temp.top().first;
                float tempY = temp.top().second;
                temp.pop();
                QString insertXYQuery = QString("INSERT INTO points_table_%1 (x,y) VALUES (%2, %3);").arg(time, QString::number(tempX), QString::number(tempY));
                sendQuery(insertXYQuery);
            }



        }else if(fieldA=="D"){
            while(!ellipseStack.empty()){
                QGraphicsEllipseItem* temp = ellipseStack.top();
                ellipseStack.pop();
                scene->removeItem(temp);
            }
            while(!xystack.empty()){
                xystack.pop();
            }
            ellipse->setPos(0,0);
        }
        else if(fieldA=="*"){
            QString dropAllTablesQuery = QString("SET FOREIGN_KEY_CHECKS = 0; "
                                                 "SET GROUP_CONCAT_MAX_LEN=32768; "
                                                 "SET @tables = NULL; "
                                                 "SELECT GROUP_CONCAT('`', table_name, '`') INTO @tables   FROM information_schema.tables   WHERE table_schema = (SELECT DATABASE()); "
                                                 "SELECT IFNULL(@tables,'dummy') INTO @tables;"
                                                 "SET @tables = CONCAT('DROP TABLE IF EXISTS ', @tables); "
                                                 "PREPARE stmt FROM @tables; EXECUTE stmt; "
                                                 "DEALLOCATE PREPARE stmt; "
                                                 "SET FOREIGN_KEY_CHECKS = 1;");
            sendQuery(dropAllTablesQuery);

        }
        else if(fieldA=="#"){
            QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
            pdb = &db;
            pdb->setHostName("192.168.33.50");
            pdb->setPort(3306);
            pdb->setDatabaseName("example_db");
            pdb->setUserName("vagrant");
            pdb->setPassword("vagrant");
            if (!pdb->open()) {
                    qDebug() << "Failed to connect to the database.";
                    return;
                }

                // Execute a select query
                QSqlQuery query;
                query.prepare(QString("SELECT * FROM regression_table_%1_%2").arg(lastTime, QString::number(lastK)));

                if (!query.exec()) {
                    qDebug() << "Failed to execute query.";
                    return;
                }

                // Store the values of the first row in a std::vector<int>
                std::vector<float> resultVector;

                if (query.first()) {
                    QSqlRecord record = query.record();
                    int columnCount = record.count();

                    for (int i = 1; i < columnCount; ++i) {
                        float value = query.value(i).toFloat();
                        resultVector.push_back(value);
                        qDebug()<<value;
                    }
                }

                int size = 10000;
                std::vector<double> lineX = linspace(xmin, xmax, size);
                std::vector<double> liney;
                liney.resize(size);

                for(int i =0; i<lineX.size(); i++){
                    for(int j=0; j<resultVector.size();j++){
                        liney[i]+=resultVector[j]*pow(lineX[i], j);
                    }


                }

                for(int i=0; i<lineX.size();i++){
                  liney[i] = 1000-(1000*(liney[i]-ymin))/(abs(ymax-ymin));
                }
                for(int i=0; i<lineX.size();i++){
                    lineX[i] = (1000*(lineX[i]-xmin))/(abs(xmax-xmin));
                }
                int red = QRandomGenerator::global()->bounded(256);
                    int green = QRandomGenerator::global()->bounded(256);
                    int blue = QRandomGenerator::global()->bounded(256);
                    QColor randomColor(red, green, blue);
                for(int i=0;i<lineX.size();i++){
                    QGraphicsEllipseItem* newEllipse = scene->addEllipse(lineX[i], liney[i], 4, 4);
                    QPen pen(Qt::NoPen);
                    newEllipse->setPen(pen);
                    newEllipse->setBrush(randomColor);
                    ellipseStack.push(newEllipse);
                }

                pdb->close();



        }else if(fieldA=="0" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 0;
            runPythonScript(table,0);
        }
        else if(fieldA=="1" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 1;
            runPythonScript(table,1);
        }
        else if(fieldA=="2" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 2;
            runPythonScript(table,2);
        }
        else if(fieldA=="3" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 3;
            runPythonScript(table,3);
        }
        else if(fieldA=="4" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 4;
            runPythonScript(table,4);
        }
        else if(fieldA=="5" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 5;
            runPythonScript(table,5);
        }
        else if(fieldA=="6" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 6;
            runPythonScript(table,6);
        }
        else if(fieldA=="7" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 7;
            runPythonScript(table,7);
        }
        else if(fieldA=="8" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 8;
            runPythonScript(table,8);
        }
        else if(fieldA=="9" && lastTime!=""){
            QString table = QString("points_table_%1").arg(lastTime);
            lastK = 9;
            runPythonScript(table,9);
        }


        else{

            if(abs(ellipse->x()+(x-512)/40)>1 || ellipse->y()+(y-512)/40>1){
                ellipse->setPos(ellipse->x()+(x-512)/40, ellipse->y()+(y-512)/40);
            }
            QPointF textPos = ellipse->mapToScene(500, 500);
            textLabel->move(textPos.x(), textPos.y());
            QString textLabelData = QString::number(trueX)+", "+QString::number(trueY)+" ";

            textLabel->setText(textLabelData);
        }











    }
void MyPlotDialog::drawPlane()
{
    XYPlaneWidget* xyPlaneWidget = new XYPlaneWidget(this->firstLabel, this->secondLabel,
                                                     this->xmin, this->xmax, this->ymin, this->ymax, this);
    xyPlaneWidget->setGeometry(0, 0, 1000, 1000);

    scene = new QGraphicsScene(QRectF(0, 0, 1000, 1000));
    scene->setBackgroundBrush(Qt::transparent);
    // Create a QGraphicsProxyWidget to add the xyPlaneWidget as a background
    QGraphicsProxyWidget* proxyWidget = new QGraphicsProxyWidget();
        proxyWidget->setWidget(xyPlaneWidget);
        scene->addItem(proxyWidget);
        proxyWidget->setZValue(-1);

    QGraphicsView* view = new QGraphicsView(scene, this);

    ellipse = scene->addEllipse(500, 500, 5, 5);
    ellipse->setBrush(Qt::red);
    textLabel = new QLabel(this);
    textLabel->setText("0,0");
    textLabel->move(500, 500);
    textLabel->setFixedWidth(100);
    textLabel->show();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setGeometry(0,0,1000,1000);
    view->setStyleSheet("background: transparent; border-radius: 20px");
    view->setScene(scene);
    view->show();





}


void MyPlotDialog::sendQuery(QString queryToSend){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    pdb = &db;
    pdb->setHostName("192.168.33.50");
    pdb->setPort(3306);
    pdb->setDatabaseName("example_db");
    pdb->setUserName("vagrant");
    pdb->setPassword("vagrant");
    if(pdb->open()){
        qDebug() << "Connected to MySQL server";
        QSqlQuery query;

        if (query.exec(queryToSend)) {
            qDebug() << "Table created successfully";
        } else {
            qDebug() << "Failed to create table: " << query.lastError().text();
        }


    }else {
        qDebug() << "Failed to connect to MySQL server: " << db.lastError().text();
    }
}
void MyPlotDialog::runPythonScript(QString table, int k)
{
    QString program = "/home/mpetlyovanyi/course_work/python-backend/bin/python";  // Assuming 'python' is in the system PATH

    QStringList arguments;
    arguments << "/home/mpetlyovanyi/course_work/python-backend/script.py";  // Replace with the actual path to your Python script
    arguments << table;
    arguments << QString::number(k);
    QProcess process;
    process.start(program, arguments);
    process.waitForFinished(-1);  // Wait for the process to finish

    // Optionally, you can retrieve the output of the Python script:
    QByteArray output = process.readAllStandardOutput();
    qDebug() << output;
}


void MyPlotDialog::retrieveEllipsePositions()
{
    QPointF ellipsePos = ellipse->scenePos();
    qDebug() << "Ellipse position (x, y):" << ellipsePos.x() << ellipsePos.y();
}


MyPlotDialog::~MyPlotDialog()
{
    // Clean up any allocated resources or perform necessary cleanup tasks
}
