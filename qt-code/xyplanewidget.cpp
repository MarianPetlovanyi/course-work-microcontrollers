#include "xyplanewidget.h"
#include <QDebug>
XYPlaneWidget::XYPlaneWidget(QString firstLabel, QString secondLabel, int xmin, int xmax, int ymin, int ymax, QWidget *parent)
    : QWidget(parent), firstLabel(firstLabel), secondLabel(secondLabel), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
{
}

void XYPlaneWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int width = this->width();
    int height = this->height();
    float zeroYAxis = (1000 * (abs(((float)xmin) / ((float)xmax)))) / (1 + abs((((float)xmin) / ((float)xmax))));

    if (xmin < 0 && xmax > 0) {
        painter.drawLine(zeroYAxis, 0, zeroYAxis, height);
        painter.drawText(zeroYAxis, height - 10, QString::number(ymin));
        painter.drawText(zeroYAxis, 20, QString::number(ymax));

    }
    else if (xmin <= 0 && xmax <= 0) {
        painter.drawText(width - 20, height - 10, QString::number(ymin));
        painter.drawText(width - 20, 20, QString::number(ymax));

    }
    else if (xmin > 0 && xmax > 0) {
        painter.drawText(5, height - 10, QString::number(ymin));
        painter.drawText(5, 20, QString::number(ymax));

    }
    float zeroXAxis = (1000 * (abs(((float)ymin) / ((float)ymax)))) / (1 + abs((((float)ymin) / ((float)ymax))));

    if (ymin < 0 && ymax > 0) {
        painter.drawLine(0, height - zeroXAxis, width, height - zeroXAxis);
        painter.drawText(0, height - zeroXAxis - 5, QString::number(xmin));

        painter.drawText(width - 20, height - zeroXAxis - 5, QString::number(xmax));

    }
    else if (ymin <= 0 && ymax <= 0) {
        painter.drawText(5, 20, QString::number(xmin));
        painter.drawText(width - 20, 20, QString::number(xmax));

    }
    else if (xmin > 0 && xmax > 0) {
        painter.drawText(5, height - 10, QString::number(xmin));
        painter.drawText(width - 20, height - 10, QString::number(xmax));

    }
    if (xmin < 0 && xmax > 0 && ymin < 0 && ymax > 0){
        painter.drawText(width-9*firstLabel.size(), height-zeroXAxis+10, firstLabel);
        painter.drawText(zeroYAxis, 40, secondLabel);
        painter.drawText(zeroYAxis + 5, height-zeroXAxis - 5, QString::number(0));
    }else if (xmin > 0 && xmax > 0 && ymin < 0 && ymax > 0){
        painter.drawText(width-9*firstLabel.size(), height-zeroXAxis+10, firstLabel);

    }else if (xmin > 0 && xmax > 0 && ymin > 0 && ymax > 0){
        painter.drawText(width-9*firstLabel.size(), height-30, firstLabel);
        painter.drawText(0, 40, secondLabel);

    }else if (xmin < 0 && xmax > 0 && ymin > 0 && ymax > 0){
        painter.drawText(zeroYAxis, 40, secondLabel);
    }else if (xmin < 0 && xmax < 0 && ymin > 0 && ymax > 0){
        painter.drawText(0, height-30, firstLabel);
        painter.drawText(width-9*firstLabel.size(), 40, secondLabel);
    }else if (xmin < 0 && xmax < 0 && ymin < 0 && ymax > 0){
        painter.drawText(width-9*firstLabel.size(), height-zeroXAxis+10, firstLabel);
    }else if (xmin < 0 && xmax < 0 && ymin < 0 && ymax < 0){
        painter.drawText(0, 30, firstLabel);
        painter.drawText(width-9*secondLabel.size(), height-20, secondLabel);
    }else if (xmin < 0 && xmax > 0 && ymin < 0 && ymax < 0){
    painter.drawText(zeroYAxis, 40, secondLabel);
    }else if (xmin > 0 && xmax > 0 && ymin < 0 && ymax < 0){
        painter.drawText(width-9*firstLabel.size(), 30, firstLabel);
        painter.drawText(0, height-20, secondLabel);
    }



    painter.setBrush(Qt::black); // Set the brush color to black


}
