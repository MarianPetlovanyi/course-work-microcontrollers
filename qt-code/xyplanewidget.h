#ifndef XYPLANEWIDGET_H
#define XYPLANEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class XYPlaneWidget : public QWidget
{
    Q_OBJECT

public:
    XYPlaneWidget(QString firstLabel, QString secondLabel, int xmin, int xmax, int ymin, int ymax, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString firstLabel;
    QString secondLabel;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

#endif // XYPLANEWIDGET_H
