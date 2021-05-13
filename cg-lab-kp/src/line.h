#ifndef LINE_H
#define LINE_H

#include <QVector3D>

#include <vector>

class Line
{
public:
    Line();
    Line(const std::vector<QVector3D>& ctrl_points,
         double approx = 0.01);
    ~Line();

    void constructLine(const std::vector<QVector3D>& ctrl_points,
                       double approx = 0.01);

    const std::vector<QVector3D>& getPoints()   const;
    const std::vector<uint32_t>&  getIndecies() const;

private:
    QVector3D calculatePoint(const std::vector<QVector3D>& ctrl_points,
                             double param);

    void calculateBasis(std::vector< std::vector<double> >& basis,
                        size_t i, size_t k, double t,
                        const std::vector<double>& knots);

    std::vector<QVector3D> data_;
    std::vector<uint32_t>  indecies_;
};

#endif // LINE_H
