#include "polygon.h"

#include <QVector3D>

Polygon::Polygon(QVector3D i, QVector3D j, QVector3D k)
{
    i_ = i;
    j_ = j;
    k_ = k;
}

Polygon::Polygon(u_int32_t i, u_int32_t j, u_int32_t k, const std::vector<float>& vertices)
{
    i_ = QVector3D(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    j_ = QVector3D(vertices[j * 3], vertices[j * 3 + 1], vertices[j * 3 + 2]);
    k_ = QVector3D(vertices[k * 3], vertices[k * 3 + 1], vertices[k * 3 + 2]);
}

Polygon::Polygon(QVector3D indecies, const std::vector<float>& vertices)
{
    *this = Polygon(indecies.x(), indecies.y(), indecies.z(), vertices);
}

Polygon Polygon::transform(const QMatrix4x4& model_matr, const QMatrix4x4& view_matr, const QMatrix4x4& proj_matr) const
{
    QMatrix4x4 tranform = proj_matr * view_matr * model_matr;
    return Polygon(tranform * i_, tranform * j_, tranform * k_);
}

bool Polygon::isVisible(QVector3D view_norm) const
{
    QVector3D side_a = k_ - i_;
    QVector3D side_b = k_ - j_;
    QVector3D cross = QVector3D::crossProduct(side_a, side_b);

    return QVector3D::dotProduct(cross, view_norm) < 0;
}

std::vector<float> Polygon::getVertices() const
{
    std::vector<float> result;
    result.reserve(9);

    result.push_back(i_.x());
    result.push_back(i_.y());
    result.push_back(i_.z());

    result.push_back(j_.x());
    result.push_back(j_.y());
    result.push_back(j_.z());

    result.push_back(k_.x());
    result.push_back(k_.y());
    result.push_back(k_.z());

    return result;
}
