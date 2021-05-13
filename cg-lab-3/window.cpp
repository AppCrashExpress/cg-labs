
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>

#include <QWindow>

#include "window.h"

Window::Window()
{
    gl_widget_ = new GLwidget;

    QSpinBox* spinbox = createSpinbox(3, 50);
    connect(spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            gl_widget_, &GLwidget::setSideCount);
    connect(gl_widget_, &GLwidget::sideCountChanged, spinbox, &QSpinBox::setValue);

    light_menu_ = new LightMenu;
    connect(light_menu_, &LightMenu::lightChanged, gl_widget_, &GLwidget::setLight);

    menu_caller_ = new QPushButton("Light options");
    connect(menu_caller_, &QPushButton::pressed, this, &Window::triggerMenu);

    QGridLayout* grid_layout = new QGridLayout();
    grid_layout->addWidget(new QLabel("Side count:"), 0, 0, Qt::AlignLeft);
    grid_layout->addWidget(spinbox, 0, 1);
    grid_layout->addWidget(menu_caller_, 0, 3, Qt::AlignRight);
    grid_layout->addWidget(createHorizontalSliders(), 1, 0, 1, 4);

    QWidget* grid_container = new QWidget();
    grid_container->setLayout(grid_layout);

    QVBoxLayout* v_layout    = new QVBoxLayout();
    v_layout->addWidget(gl_widget_);
    v_layout->addWidget(grid_container);

    setLayout(v_layout);
}

void Window::triggerMenu()
{
    light_menu_->show();
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

QSpinBox* Window::createSpinbox(int     min,
                                int     max,
                                int     step) {
    QSpinBox* spinbox = new QSpinBox(this);
    spinbox->setMinimum(min);
    spinbox->setMaximum(max);
    spinbox->setSingleStep(step);
    return spinbox;
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
    x_slider_->setParent(container);
    y_slider_->setParent(container);
    z_slider_->setParent(container);
    container->setLayout(sliders);
    return container;
}
