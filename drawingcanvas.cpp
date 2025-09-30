#include "drawingcanvas.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <algorithm>
#include <QSet>
#include <cmath>

DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget(parent), penWidth(5), showConvexHull(false),
    iterationsJarvis(0), iterationsSlow(0)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#222222"));
    setPalette(pal);
    setAutoFillBackground(true);
}

void DrawingCanvas::clearCanvas()
{
    points.clear();
    convexHull.clear();
    showConvexHull = false;
    iterationsJarvis = 0;
    iterationsSlow = 0;
    emit pointAdded(points.size());
    update();
}

void DrawingCanvas::findConvexHull()
{
    if (points.size() < 3) {
        convexHull.clear();
        iterationsJarvis = 0;
        iterationsSlow = 0;
    } else {
        QVector<QPoint> slowHull = slowConvexHullAlgorithm(points);
        convexHull = convexHullAlgorithm(points);
    }
    showConvexHull = true;
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (showConvexHull) {
        QPen textPen(QColor("#CCCCCC"));
        painter.setPen(textPen);
        painter.setFont(QFont("Arial", 10, QFont::Bold));

        QString jarvisText = QString("General Iterations: %L1").arg(iterationsJarvis);
        QString slowText = QString("Slow Hull Convex Iterations: %L1").arg(iterationsSlow);

        painter.drawText(20, 20, jarvisText);
        painter.drawText(20, 40, slowText);
    }

    QPen pointPen(QColor("#4FC3F7"), penWidth);
    pointPen.setCapStyle(Qt::RoundCap);
    painter.setPen(pointPen);

    QBrush pointBrush(QColor("#4FC3F7"));
    painter.setBrush(pointBrush);

    for (const QPoint &p : points) {
        painter.drawPoint(p);
    }

    if (showConvexHull && convexHull.size() > 1) {
        QPen hullPen(QColor("#AB47BC"), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(hullPen);

        for (int i = 0; i < convexHull.size(); ++i) {
            const QPoint& p1 = convexHull[i];
            const QPoint& p2 = convexHull[(i + 1) % convexHull.size()];
            painter.drawLine(p1, p2);
        }

        QPen hullVertexPen(QColor("#FFB300"), penWidth + 2);
        QBrush hullVertexBrush(QColor("#FFB300"));
        painter.setPen(hullVertexPen);
        painter.setBrush(hullVertexBrush);

        for (const QPoint &p : convexHull) {
            painter.drawEllipse(p.x() - 4, p.y() - 4, 8, 8);
        }
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        showConvexHull = false;
        convexHull.clear();
        iterationsJarvis = 0;
        iterationsSlow = 0;

        points.append(event->pos());
        emit pointAdded(points.size());
        update();
    }
}

int DrawingCanvas::orientation(const QPoint& p, const QPoint& q, const QPoint& r)
{
    qint64 val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

QVector<QPoint> DrawingCanvas::convexHullAlgorithm(QVector<QPoint> pts)
{
    long long iterCount = 0;
    int n = pts.size();
    if (n < 3) return {};

    std::sort(pts.begin(), pts.end(), [&](const QPoint& a, const QPoint& b) {
        iterCount++;
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    });

    QVector<QPoint> upperHull;
    QVector<QPoint> lowerHull;

    upperHull.append(pts[0]);
    upperHull.append(pts[1]);

    for (int i = 2; i < n; ++i) {
        upperHull.append(pts[i]);

        while (upperHull.size() > 2) {
            iterCount++;

            QPoint p1 = upperHull[upperHull.size() - 3];
            QPoint p2 = upperHull[upperHull.size() - 2];
            QPoint p3 = upperHull[upperHull.size() - 1];

            if (orientation(p1, p2, p3) == 1) {
                break;
            }

            upperHull.remove(upperHull.size() - 2);
            iterCount++;
        }
    }

    lowerHull.append(pts[n-1]);
    lowerHull.append(pts[n-2]);

    for (int i = n - 3; i >= 0; --i) {
        lowerHull.append(pts[i]);

        while (lowerHull.size() > 2) {
            iterCount++;

            QPoint p1 = lowerHull[lowerHull.size() - 3];
            QPoint p2 = lowerHull[lowerHull.size() - 2];
            QPoint p3 = lowerHull[lowerHull.size() - 1];

            if (orientation(p1, p2, p3) == 1) {
                break;
            }

            lowerHull.remove(lowerHull.size() - 2);
            iterCount++;
        }
    }

    if (lowerHull.size() > 1) {
        lowerHull.removeFirst();
    }
    if (lowerHull.size() > 0) {
        lowerHull.removeLast();
    }

    upperHull.append(lowerHull);

    iterationsJarvis = iterCount;
    return upperHull;
}


QVector<QPoint> DrawingCanvas::slowConvexHullAlgorithm(QVector<QPoint> pts)
{
    long long iterCount = 0;
    int n = pts.size();
    if (n < 3) return {};

    QSet<QPoint> hullVertices;

    for (int i = 0; i < n; i++) {
        const QPoint& p = pts[i];
        for (int j = 0; j < n; j++) {
            const QPoint& q = pts[j];

            if (p == q) continue;

            bool valid = true;

            for (int k = 0; k < n; k++) {
                const QPoint& r = pts[k];
                iterCount++;

                if (r == p || r == q) continue;

                if (orientation(p, q, r) == 2) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                hullVertices.insert(p);
                hullVertices.insert(q);
            }
        }
    }

    iterationsSlow = iterCount;

    QVector<QPoint> hullList = hullVertices.values();

    if (hullList.size() < 3) {
        return hullList;
    }

    QPoint center(0, 0);
    for(const QPoint& pt : hullList) center += pt;
    center /= hullList.size();

    std::sort(hullList.begin(), hullList.end(), [&](const QPoint& a, const QPoint& b) {
        qint64 cross_product = (a.x() - center.x()) * (b.y() - center.y()) - (b.x() - center.x()) * (a.y() - center.y());

        if (cross_product > 0) return false;
        if (cross_product < 0) return true;

        qint64 dist_a = (a.x() - center.x())*(a.x() - center.x()) + (a.y() - center.y())*(a.y() - center.y());
        qint64 dist_b = (b.x() - center.x())*(b.x() - center.x()) + (b.y() - center.y())*(b.y() - center.y());
        return dist_a < dist_b;
    });

    return hullList;
}
