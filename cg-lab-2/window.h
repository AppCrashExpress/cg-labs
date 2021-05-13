#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>

#include "mainwindow.h"
#include "glwidget.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    QSlider* createSlider(float min,  float max,
                          float step, float tick);

    QWidget* createHorizontalSliders();

    QSlider* x_slider_;
    QSlider* y_slider_;
    QSlider* z_slider_;

    GLwidget* gl_widget_;
};

#endif // WINDOW_H
