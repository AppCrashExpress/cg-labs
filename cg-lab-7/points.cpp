#include "points.h"

Square::Square(int x, int y, int size)
    : center_(x, y), size_(size)
{

}

void Square::move(int x, int y)
{
    center_.setX(x);
    center_.setY(y);
}

void Square::draw(QPainter &painter) const
{
    painter.drawRect(center_.x() - size_ / 2, center_.y() - size_ / 2, size_, size_);
}

void Square::erase(QPainter &painter) const
{
    painter.eraseRect(center_.x() - size_ / 2, center_.y() - size_ / 2, size_, size_);
}

bool Square::clicked(int x, int y) const
{
    QPoint distance(center_.x() - x, center_.y() - y);
    if (distance.x() < 0) distance.setX(distance.x() * -1);
    if (distance.y() < 0) distance.setY(distance.y() * -1);

    return distance.x() <= size_ / 2 && distance.y() <= size_ / 2;
}

QPoint Square::center() const
{
    return center_;
}
