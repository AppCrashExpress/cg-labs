#ifndef SQUARE_H
#define SQUARE_H

#include <QPoint>
#include <QPainter>
#include <QBrush>

class Square
{
public:
    Square(int x, int y, int size);

    void move (int x, int y);
    void draw (QPainter& painter) const;
    void erase(QPainter& painter) const;

    bool clicked(int x, int y) const;
    QPoint center() const;

private:
    QPoint center_;
    int    size_;
};

#endif // SQUARE_H
