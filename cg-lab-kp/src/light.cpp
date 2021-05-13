#include "light.h"

Light::Light(QVector3D pos, QVector3D color, LightOptions opts)
{
    pos_ = pos;
    color_ = color;
    options_ = opts;
}

void Light::setPos(QVector3D pos)
{
    pos_ = pos;
}

void Light::setColor(QVector3D color)
{
    color_ = color;
}

void Light::setOptions(LightOptions opts)
{
    options_ = opts;
}

void Light::addOptions(LightOptions opts)
{
    options_ = options_ | opts;
}

void Light::removeOptions(LightOptions opts)
{
    options_ = options_ & ~(opts);
}


QVector3D Light::getPos() const
{
    return pos_;
}

QVector3D Light::getColor() const
{
    return color_;
}

LightOptions Light::getOptions() const
{
    return options_;
}
