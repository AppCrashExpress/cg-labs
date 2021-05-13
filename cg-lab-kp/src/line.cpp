#include "line.h"

Line::Line()
{

}

Line::Line(const std::vector<QVector3D> &ctrl_points,
           double approx)
{
    constructLine(ctrl_points, approx);
}

Line::~Line()
{

}

void Line::constructLine(const std::vector<QVector3D> &ctrl_points,
                         double approx)
{
    data_.reserve(1.0f / approx);

    for (double t = 3.0f / 10; t <= 7.0f/10; t += approx)
    {
        data_.push_back(calculatePoint(ctrl_points, t));
    }

    indecies_.resize(data_.size());
    for (size_t i = 0; i < indecies_.size(); ++i) {
        indecies_[i] = i;
    }
}

const std::vector<QVector3D>& Line::getPoints() const
{
    return data_;
}

const std::vector<uint32_t>& Line::getIndecies() const
{
    return indecies_;
}

QVector3D Line::calculatePoint(const std::vector<QVector3D> &ctrl_points,
                               double param)
{
    constexpr uint32_t degree = 3;
    size_t knot_count = ((ctrl_points.size() - 1) + degree + 1) + 1;
    // That is index of last knot + 1

    std::vector< std::vector<double> > basis (
                    knot_count,
                    std::vector<double>(degree + 1, -1)
                );

    std::vector<double> knots(knot_count);
    for (size_t i = 0; i < knot_count; ++i)
    {
        knots[i] = 1.0f * i / (knot_count - 1);
    }

    for (size_t i = 0; i < ctrl_points.size(); ++i)
    {
        calculateBasis(basis, i, degree, param, knots);
    }

    QVector3D point;
    for (size_t i = 0; i < ctrl_points.size(); ++i)
    {
        point += basis[i][degree] * ctrl_points[i];
    }

    return point;
}

void Line::calculateBasis(std::vector< std::vector<double> > &basis,
                           size_t i, size_t k, double t,
                           const std::vector<double> &knots)
{
    if (basis[i][k] != -1) return;

    if (k == 0)
    {
        basis[i][k] = (t >= knots[i] && t < knots[i + 1]);
    }
    else
    {
        calculateBasis(basis, i, k - 1, t, knots);
        calculateBasis(basis, i + 1, k - 1, t, knots);

        basis[i][k] = (double) (t - knots[i]) / (knots[i + k] - knots[i]) * basis[i][k - 1]
                + (double) (knots[i + k + 1] - t) / (knots[i + k + 1] - knots[i + 1]) * basis[i + 1][k - 1];
    }
}
