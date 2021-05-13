#include "canvas.h"

#include <QBrush>
#include <QMouseEvent>

#include "lines.h"

static constexpr int kPointSize = 10;

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    squares_.push_back(Square(100, 30,  kPointSize));
    squares_.push_back(Square(200, 50,  kPointSize));
    squares_.push_back(Square(300, 100, kPointSize));
    squares_.push_back(Square(400, 10,  kPointSize));
    selected_square_ = squares_.end();
}

QSize Canvas::sizeHint() const
{
    return QSize(600, 600);
}

QSize Canvas::minimumSizeHint() const
{
    return QSize(100, 100);
}

void Canvas::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);
    painter.setBrush(Qt::red);

    for (const auto &s : squares_)
    {
        s.draw(painter);
    }

    Lagrangian line(squares_, 1);
    line.draw(painter);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    for (auto it = squares_.begin(), end = squares_.end();
         it != end;
         ++it)
    {
        if (it->clicked(event->x(), event->y()))
        {
            selected_square_ = it;
            event->accept();
        }
    }

    if (selected_square_ == squares_.end())
    {
        event->ignore();
        return;
    }

    if (event->button() == Qt::RightButton) {
        squares_.erase(selected_square_);
        selected_square_ = squares_.end();
        update();
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    squares_.push_back(Square(event->x(), event->y(), kPointSize));
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (selected_square_ != squares_.end())
    {
        selected_square_->move(event->x(), event->y());
    }
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    selected_square_ = squares_.end();
}
