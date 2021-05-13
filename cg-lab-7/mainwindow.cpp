#include "mainwindow.h"

#include "canvas.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new Canvas());
}

MainWindow::~MainWindow()
{
}

