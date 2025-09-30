#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QStyleFactory>
#include <QSpacerItem>
#include "drawingcanvas.h"

const QString BUTTON_STYLE = R"(
    QPushButton {
        background-color: #DDDDDD; /* Light Gray */
        color: #333333; /* Dark Text */
        border: 1px solid #AAAAAA; /* Light border */
        border-radius: 4px;
        padding: 8px 16px;
        font-size: 13px;
        font-weight: 500;
        /* min-width REMOVED to allow stretching to full width */
    }
    QPushButton:hover {
        background-color: #CCCCCC; /* Darker on hover */
    }
    QPushButton:pressed {
        background-color: #BBBBBB; /* Even darker when pressed */
    }
)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Computer Graphics Simulation");
    setMinimumSize(800, 600);

    const QString LIGHT_WINDOW_STYLE = R"(
        QMainWindow, QWidget {
            background-color: #F0F0F0; /* Light/White Window background */
            color: #333333; /* All text is dark */
        }
    )";
    setStyleSheet(LIGHT_WINDOW_STYLE);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    DrawingCanvas *canvas = new DrawingCanvas(this);
    mainLayout->addWidget(canvas, 1);

    QWidget *controlBar = new QWidget(this);
    controlBar->setFixedHeight(50);
    controlBar->setStyleSheet("background-color: #FFFFFF; border-top: 1px solid #CCCCCC;"); // White bar color

    QHBoxLayout *controlLayout = new QHBoxLayout(controlBar);
    controlLayout->setContentsMargins(10, 5, 10, 5);
    controlLayout->setSpacing(10);

    QPushButton *convexHullButton = new QPushButton("Fix Convex Hull");
    convexHullButton->setObjectName("convexHullButton");
    convexHullButton->setStyleSheet(BUTTON_STYLE);
    controlLayout->addWidget(convexHullButton, 1);

    QPushButton *clearButton = new QPushButton("Clear Canvas");
    clearButton->setStyleSheet(BUTTON_STYLE);
    controlLayout->addWidget(clearButton, 1);

    mainLayout->addWidget(controlBar, 0);

    QObject::connect(clearButton, &QPushButton::clicked, canvas, &DrawingCanvas::clearCanvas);
    QObject::connect(convexHullButton, &QPushButton::clicked, canvas, &DrawingCanvas::findConvexHull);
}

MainWindow::~MainWindow() {}
