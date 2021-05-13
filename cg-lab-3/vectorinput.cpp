#include <QGridLayout>
#include <QVBoxLayout>

#include "vectorinput.h"

VectorInput::VectorInput(QWidget *parent) : QWidget(parent)
{
    group_ = new QGroupBox();

    x_ = new QDoubleSpinBox(group_);
    y_ = new QDoubleSpinBox(group_);
    z_ = new QDoubleSpinBox(group_);

    connect(x_,   static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &VectorInput::changeX);
    connect(y_,   static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &VectorInput::changeY);
    connect(z_,   static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &VectorInput::changeZ);

    x_label_ = new QLabel(group_);
    y_label_ = new QLabel(group_);
    z_label_ = new QLabel(group_);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(x_label_, 0, 0, 1, 1);
    layout->addWidget(y_label_, 0, 1, 1, 1);
    layout->addWidget(z_label_, 0, 2, 1, 1);
    layout->addWidget(x_, 1, 0, 1, 1);
    layout->addWidget(y_, 1, 1, 1, 1);
    layout->addWidget(z_, 1, 2, 1, 1);
    group_->setLayout(layout);

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(group_);
    setLayout(main_layout);
}

void VectorInput::setLimits(double min, double max, double step)
{
    x_->setMinimum(min);
    y_->setMinimum(min);
    z_->setMinimum(min);

    x_->setMaximum(max);
    y_->setMaximum(max);
    z_->setMaximum(max);

    x_->setSingleStep(step);
    y_->setSingleStep(step);
    z_->setSingleStep(step);
}

void VectorInput::setGroupName(const QString& name)
{
    group_->setTitle(name);
}

void VectorInput::setLabels(const QString& x_name,
                            const QString& y_name,
                            const QString& z_name)
{
    x_label_->setText(x_name);
    y_label_->setText(y_name);
    z_label_->setText(z_name);
}

void VectorInput::changeVector(const QVector3D& new_vector)
{
    if (value_ != new_vector) {
        value_ = new_vector;
        x_->setValue(new_vector.x());
        y_->setValue(new_vector.y());
        z_->setValue(new_vector.z());
        emit vectorChanged(value_);
    }
}

void VectorInput::changeX(double val)
{
    value_.setX(val);
    emit vectorChanged(value_);
}

void VectorInput::changeY(double val)
{
    value_.setY(val);
    emit vectorChanged(value_);
}

void VectorInput::changeZ(double val)
{
    value_.setZ(val);
    emit vectorChanged(value_);
}
