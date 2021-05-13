#include "mainwindow.h"
#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new Window());
}

MainWindow::~MainWindow()
{

}

