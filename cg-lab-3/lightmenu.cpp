#include <QVBoxLayout>

#include "lightmenu.h"
#include "vectorinput.h"

LightMenu::LightMenu(QWidget *parent) : QWidget(parent)
{
    light_ = Light(QVector3D(0.0f, 1.0f, 0.0f));

    VectorInput* pos_input = new VectorInput;
    pos_input->setLimits(-20.0, 20.0);
    pos_input->setGroupName("Light Position");
    pos_input->setLabels("X", "Y", "Z");
    connect(pos_input, &VectorInput::vectorChanged, this, &LightMenu::setPos);

    VectorInput* color_input = new VectorInput;
    color_input->setLimits(0.0, 1.0, 0.1);
    color_input->setGroupName("Light Color");
    color_input->setLabels("Red", "Green", "Blue");
    connect(color_input, &VectorInput::vectorChanged, this, &LightMenu::setColor);

    pos_input->changeVector(QVector3D(0.0f, 1.0f, 0.0f));
    color_input->changeVector(QVector3D(1.0f, 1.0f, 1.0f));

    ambt_check_ = new QCheckBox("Ambient light", this);
    diff_check_ = new QCheckBox("Diffuse light", this);
    spec_check_ = new QCheckBox("Specular light", this);

    ambt_check_->setCheckState(Qt::Checked);
    diff_check_->setCheckState(Qt::Checked);
    spec_check_->setCheckState(Qt::Checked);

    connect(ambt_check_, &QCheckBox::stateChanged, this, &LightMenu::setAmbient);
    connect(diff_check_, &QCheckBox::stateChanged, this, &LightMenu::setDiffuse);
    connect(spec_check_, &QCheckBox::stateChanged, this, &LightMenu::setSpecular);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(pos_input);
    layout->addWidget(color_input);
    layout->addWidget(ambt_check_);
    layout->addWidget(diff_check_);
    layout->addWidget(spec_check_);
    setLayout(layout);
}

void LightMenu::setPos(const QVector3D& pos)
{
    if (light_.getPos() != pos) {
        light_.setPos(pos);
        emit lightChanged(light_);
    }
}

void LightMenu::setColor(const QVector3D& color)
{
    if (light_.getColor() != color) {
        light_.setColor(color);
        emit lightChanged(light_);
    }
}

void LightMenu::setAmbient(int state)
{
    state ? light_.addOptions(L_AMBIENT) :
            light_.removeOptions(L_AMBIENT);
    emit lightChanged(light_);
}

void LightMenu::setDiffuse(int state)
{
    state ? light_.addOptions(L_DIFFUSE) :
            light_.removeOptions(L_DIFFUSE);
    emit lightChanged(light_);
}

void LightMenu::setSpecular(int state)
{
    state ? light_.addOptions(L_SPECULAR) :
            light_.removeOptions(L_SPECULAR);
    emit lightChanged(light_);
}
