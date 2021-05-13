#include "lines.h"

#include <algorithm>

Lagrangian::Lagrangian()
{

}

Lagrangian::Lagrangian(std::vector<Square> points, int approx)
{
    recalc(points, approx);
}

void Lagrangian::recalc(std::vector<Square> points, int approx)
{
    graph_points_.clear();

    const size_t count = points.size();
    if (count < 2) return;

    std::sort(points.begin(), points.end(), [](Square a, Square b) {
        return a.center().x() < b.center().x();
    });

    for (size_t i = 0; i < count - 1; ++i)
    {
        if (points[i].center().x() == points[i + 1].center().x())
            return;
    }

    int max_length = points[count - 1].center().x();
    int min_length = points[0].center().x();
    graph_points_.reserve(max_length - min_length + 1);

    for (int i = min_length; i <= max_length; i += approx)
    {
        double i_y = 0;

        for (const auto& p_i : points)
        {
            int p_i_x = p_i.center().x();
            double poly = p_i.center().y();

            for (const auto& p_j : points)
            {
                int p_j_x = p_j.center().x();
                if (p_i_x == p_j_x)
                    continue;

                //      Force float
                poly *= 1.0f * (i - p_j_x) / (p_i_x - p_j_x);
            }

            i_y += poly;
        }

        // Pixels are too small to bother with rounding
        // Floor polynomial result
        graph_points_.push_back(QPoint(i, i_y));
    }
}

void Lagrangian::draw(QPainter& painter) const
{
    const int count = graph_points_.size();
    for (int i = 0; i < count - 1; ++i) {
        painter.drawLine(graph_points_[i], graph_points_[i + 1]);
    }
}

