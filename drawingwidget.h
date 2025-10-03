#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QColor>
#include <QVector>
#include <QPoint>
#include <QPen>

struct LineSegment {
    QPoint startPoint;
    QPoint endPoint;
    QColor color;
    int width;
};

class DrawingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingWidget(QWidget *parent = nullptr);

    void setLineColor(const QColor &color);
    void setLineWidth(int width);
    void clearDrawing();

    QColor getLineColor() const { return currentColor; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<LineSegment> lines;
    QPoint startPoint;
    QPoint endPoint;
    bool drawing;

    QColor currentColor;
    int currentWidth;

    const Qt::PenStyle fixedStyle = Qt::SolidLine;
};

#endif
