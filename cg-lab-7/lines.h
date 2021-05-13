#ifndef LAGRANGIAN_H
#define LAGRANGIAN_H

#include <QPainter>
#include <QPoint>

#include <vector>
#include "points.h"

class Lagrangian
{
public:
    Lagrangian();
    Lagrangian(std::vector<Square> points, int approx = 1);

    // Approx - approximate length of each small line in pixels
    void recalc(std::vector<Square> points, int approx = 1);

    void draw(QPainter& painter) const;

private:
    std::vector<QPoint> graph_points_;
};

#endif // LAGRANGIAN_H
