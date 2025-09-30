#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QColor>
#include <QVector>
#include <QPoint>

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);
    void clearCanvas();
    void findConvexHull();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPoint> points;
    QVector<QPoint> convexHull;
    int penWidth;
    bool showConvexHull;

    long long iterationsJarvis;
    long long iterationsSlow;

    QVector<QPoint> convexHullAlgorithm(QVector<QPoint> pts);
    QVector<QPoint> slowConvexHullAlgorithm(QVector<QPoint> pts);

    int orientation(const QPoint& p, const QPoint& q, const QPoint& r);

signals:
    void pointAdded(int count);
};

#endif
