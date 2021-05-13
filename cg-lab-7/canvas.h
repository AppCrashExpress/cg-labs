#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <vector>

#include "points.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    QSize sizeHint()        const override;
    QSize minimumSizeHint() const override;

    void  mousePressEvent      (QMouseEvent *event) override;
    void  mouseDoubleClickEvent(QMouseEvent *event) override;
    void  mouseMoveEvent       (QMouseEvent *event) override;
    void  mouseReleaseEvent    (QMouseEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

private:
    using PointArray = std::vector<Square>;

    PointArray squares_;
    PointArray::iterator selected_square_;
};

#endif // CANVAS_H
