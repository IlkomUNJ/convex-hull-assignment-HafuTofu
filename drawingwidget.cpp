#include "drawingwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPen>

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget(parent), drawing(false), currentColor(Qt::black),
    currentWidth(2)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMinimumSize(400, 300);
}

void DrawingWidget::setLineColor(const QColor &color)
{
    currentColor = color;
}

void DrawingWidget::setLineWidth(int width)
{
    currentWidth = width;
}


void DrawingWidget::clearDrawing()
{
    lines.clear();
    update();
}

void DrawingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (const LineSegment &line : lines) {
        QPen pen(line.color, line.width, fixedStyle, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(line.startPoint, line.endPoint);
    }

    if (drawing) {
        QPen pen(currentColor, currentWidth, fixedStyle, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(startPoint, endPoint);
    }
}

void DrawingWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        endPoint = startPoint;
        drawing = true;
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && drawing) {
        endPoint = event->pos();
        update();
    }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        endPoint = event->pos();

        LineSegment line;
        line.startPoint = startPoint;
        line.endPoint = endPoint;
        line.color = currentColor;
        line.width = currentWidth;
        lines.append(line);

        drawing = false;
        update();
    }
}
