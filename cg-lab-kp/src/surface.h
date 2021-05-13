#ifndef BSURFACE_H
#define BSURFACE_H

#include <QVector3D>

#include <vector>

class BSurface
{
public:
    BSurface();
    BSurface(const std::vector< std::vector<QVector3D> >& ctrl_points,
             double approx = 0.01);

    BSurface(const std::vector< std::vector<QVector3D> >& ctrl_points,
             double long_approx, double lat_approx);

    void constructSurface(const std::vector< std::vector<QVector3D> >& ctrl_points,
                          double long_approx, double lat_approx);

    const std::vector< std::vector<QVector3D> >& getPoints() const;

private:
    size_t calcKnotCount(size_t ctrl_point_count, uint32_t degree);
    void   calculateBasis(std::vector< std::vector<double> >& basis,
                          size_t i, size_t k, double t,
                          const std::vector<double>& knots);

    std::vector< std::vector<QVector3D> > points;
};

#endif // BSURFACE_H
