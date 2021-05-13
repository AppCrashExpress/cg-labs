
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "window.h"

Window::Window()
{
    gl_widget_ = new GLwidget;

    QVBoxLayout* v_layout    = new QVBoxLayout();
    QWidget*     v_container = new QWidget();
    v_layout->addWidget(gl_widget_);
    v_layout->addWidget(createHorizontalSliders());
    v_container->setLayout(v_layout);

    QHBoxLayout* main_layout = new QHBoxLayout();
    main_layout->addWidget(v_container);

    setLayout(main_layout);
}

QSlider* Window::createSlider(float min,
                              float max,
                              float step,
                              float tick)
{
    QSlider* slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(min, max);
    slider->setSingleStep(step);
    slider->setTickInterval(tick);
    slider->setTickPosition(QSlider::TicksBelow);
    return slider;
}

QWidget* Window::createHorizontalSliders()
{
    x_slider_ = createSlider(0, 360 * 16 * 2, 16, 36 * 16);
    y_slider_ = createSlider(0, 360 * 16 * 2, 16, 36 * 16);
    z_slider_ = createSlider(3, 30, 0.1, 9.8);

    connect(x_slider_,  &QSlider::valueChanged, gl_widget_, &GLwidget::setXRotation);
    connect(y_slider_,  &QSlider::valueChanged, gl_widget_, &GLwidget::setYRotation);
    connect(z_slider_,  &QSlider::valueChanged, gl_widget_, &GLwidget::setZoom);
    connect(gl_widget_, &GLwidget::xRotationChanged, x_slider_, &QSlider::setValue);
    connect(gl_widget_, &GLwidget::yRotationChanged, y_slider_, &QSlider::setValue);
    connect(gl_widget_, &GLwidget::zoomChanged,      z_slider_, &QSlider::setValue);

    x_slider_->setValue(396 * 16);
    z_slider_->setValue(4);

    QFormLayout* sliders = new QFormLayout();
    sliders->addRow("Pitch", x_slider_);
    sliders->addRow("Roll",  y_slider_);
    sliders->addRow("Zoom",  z_slider_);

    QWidget* container = new QWidget;
    container->setLayout(sliders);
    return container;
}
