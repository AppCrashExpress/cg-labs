#ifndef POLYGON_H
#define POLYGON_H

#include <QMatrix4x4>
#include <QVector3D>
#include <vector>

class Polygon
{
public:
    Polygon(QVector3D i, QVector3D j, QVector3D k);
    Polygon(u_int32_t i, u_int32_t j, u_int32_t k, const std::vector<float>& vertices);
    Polygon(QVector3D indecies, const std::vector<float>& vertices);

    Polygon transform(const QMatrix4x4& model_matr, const QMatrix4x4& view_matr, const QMatrix4x4& proj_matr) const;

    bool               isVisible(QVector3D view_norm) const;
    std::vector<float> getVertices() const;

private:
    QVector3D i_;
    QVector3D j_;
    QVector3D k_;
};

#endif // POLYGON_H
