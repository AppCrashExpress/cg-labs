#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>

#include "mainwindow.h"
#include "glwidget.h"
#include "lightmenu.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void triggerMenu();

private:
    QSlider*  createSlider (float min, float max, float step, float tick);
    QSpinBox* createSpinbox(int   min, int   max, int   step = 1);

    QWidget* createHorizontalSliders();

    QSlider* x_slider_;
    QSlider* y_slider_;
    QSlider* z_slider_;

    GLWidget* gl_widget_;

    QPushButton* menu_caller_;
    LightMenu*   light_menu_;
};

#endif // WINDOW_H
