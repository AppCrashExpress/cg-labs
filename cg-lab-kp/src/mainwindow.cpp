#include "mainwindow.h"

#include <QOpenGLWidget>

#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new Window());
}

MainWindow::~MainWindow()
{
}

