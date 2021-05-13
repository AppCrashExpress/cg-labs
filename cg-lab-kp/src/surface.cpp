#include "surface.h"

BSurface::BSurface()
{

}

BSurface::BSurface(const std::vector< std::vector<QVector3D> >& ctrl_points,
                   double approx)
{
    constructSurface(ctrl_points, approx, approx);
}

BSurface::BSurface(const std::vector< std::vector<QVector3D> >& ctrl_points,
                   double long_approx, double lat_approx)
{
    constructSurface(ctrl_points, long_approx, lat_approx);
}

void BSurface::constructSurface(const std::vector< std::vector<QVector3D> >& ctrl_points,
                                double long_approx, double lat_approx)
{
    constexpr uint32_t u_degree = 3;
    constexpr uint32_t v_degree = 3;
    const size_t u_knot_count = calcKnotCount(ctrl_points[0].size() - 1, u_degree);
    const size_t v_knot_count = calcKnotCount(ctrl_points.size() - 1, v_degree);

    // Assuming that 'u' is responsible for creating circle
    // And 'v' is connecting 'u' lines
    // Then u_knots must be uniform and v_knots should be open uniform
    std::vector<double> u_knots;
    std::vector<double> v_knots;

    u_knots.resize(u_knot_count);
    for (size_t i = 0; i < u_knot_count; ++i)
    {
        u_knots[i] = (double) i / (u_knot_count - 1);
    }

    v_knots.resize(v_knot_count);
    for (size_t i = 0; i <= v_degree; ++i)
    {
        v_knots[i] = 0;
        v_knots[v_knot_count - 1 - i] = 1;
    }

    size_t knot_max = ctrl_points.size() - v_degree;
    for (size_t i = 1; i < knot_max; ++i)
    {
        v_knots[v_degree + i] = (double) i / knot_max;
    }

    double u_max = u_knots[ctrl_points[0].size()];
    double v_max = v_knots[ctrl_points.size()];
    points.reserve(1 / (v_max - v_knots[v_degree]));

    for (double v_t = v_knots[v_degree]; v_t < v_max; v_t += long_approx)
    {
        std::vector< std::vector<double> > v_basis (
                        v_knot_count,
                        std::vector<double>(v_degree + 1, -1)
                    );

        for (size_t i = 0; i < ctrl_points.size(); ++i)
        {
            calculateBasis(v_basis, i, v_degree, v_t, v_knots);
        }

        std::vector<QVector3D> line;
        line.reserve(1 / (u_max - u_knots[u_degree]));

        for (double u_t = u_knots[u_degree]; u_t < u_max; u_t += lat_approx)
        {

            std::vector< std::vector<double> > u_basis (
                            u_knot_count,
                            std::vector<double>(u_degree + 1, -1)
                        );

            for (size_t i = 0; i < ctrl_points[0].size(); ++i)
            {
                calculateBasis(u_basis, i, u_degree, u_t, u_knots);
            }

            QVector3D point;
            for (size_t j = 0; j < ctrl_points.size(); ++j)
            {
                for (size_t i = 0; i < ctrl_points[0].size(); ++i)
                {
                    point += u_basis[i][u_degree] * v_basis[j][v_degree] * ctrl_points[j][i];
                }
            }

            line.push_back(point);
        }

        points.push_back(line);
    }

}

const std::vector< std::vector<QVector3D> >& BSurface::getPoints() const
{
    return points;
}

size_t BSurface::calcKnotCount(size_t max_ctrl_point, uint32_t degree)
{
    return (max_ctrl_point + degree + 1) + 1;
}

void BSurface::calculateBasis(std::vector< std::vector<double> > &basis,
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

        basis[i][k] = 0;

        if (basis[i][k - 1] != 0)
        {
            basis[i][k] += (double) (t - knots[i]) / (knots[i + k] - knots[i]) * basis[i][k - 1];

        }

        if (basis[i + 1][k - 1] != 0)
        {
            basis[i][k] += (double) (knots[i + k + 1] - t) / (knots[i + k + 1] - knots[i + 1]) * basis[i + 1][k - 1];
        }
    }
}
