#ifndef POLYGON_H
#define POLYGON_H

#include <QMatrix4x4>
#include <QVector3D>
#include <vector>
#include "light.h"

class Polygon
{
public:
    Polygon(QVector3D i, QVector3D j, QVector3D k, QVector3D color = QVector3D(1.0f, 1.0f, 1.0f));
    Polygon(u_int32_t i, u_int32_t j, u_int32_t k, const std::vector<float>& vertices);
    Polygon(QVector3D indecies, const std::vector<float>& vertices);

    Polygon transform(const QMatrix4x4& tranform_matrix) const;
    Polygon transform(const QMatrix4x4& model_matr, const QMatrix4x4& view_matr, const QMatrix4x4& proj_matr) const;
    Polygon transformAndColor(const QMatrix4x4& model_matr, const QMatrix4x4& view_matr,  const QMatrix4x4& proj_matr,
                              const Light& light, float ambience_strength, int shininess,
                              const QVector3D& cam_pos) const;

    bool               isVisible(QVector3D view_direction) const;
    std::vector<float> getVerticesFloat() const;

private:
    QVector3D calcLight(const QVector3D& point, const QVector3D& point_color,
                        const Light& light, float ambience_strength, int shininess,
                        const QMatrix4x4& model_matr, const QVector3D& cam_pos) const;

    QVector3D i_;
    QVector3D j_;
    QVector3D k_;

    QVector3D i_color_;
    QVector3D j_color_;
    QVector3D k_color_;

    QVector3D normal_;
};

#endif // POLYGON_H
