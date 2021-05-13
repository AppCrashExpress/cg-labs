#include "polygon.h"

#include <QVector3D>
#include <cmath>

Polygon::Polygon(QVector3D i, QVector3D j, QVector3D k, QVector3D color)
{
    i_ = i;
    j_ = j;
    k_ = k;

    i_color_ = color;
    j_color_ = color;
    k_color_ = color;

    QVector3D side_a = k_ - i_;
    QVector3D side_b = k_ - j_;
    normal_ = QVector3D::crossProduct(side_a, side_b).normalized();
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

Polygon Polygon::transform(const QMatrix4x4& tranform_matrix) const
{
    return Polygon(tranform_matrix * i_, tranform_matrix * j_, tranform_matrix * k_);
}

Polygon Polygon::transform(const QMatrix4x4& model_matr, const QMatrix4x4& view_matr, const QMatrix4x4& proj_matr) const
{
    QMatrix4x4 tranform_matrix = proj_matr * view_matr * model_matr;
    return transform(tranform_matrix);
}

Polygon Polygon::transformAndColor(const QMatrix4x4& model_matr, const QMatrix4x4& view_matr,  const QMatrix4x4& proj_matr,
                                   const Light& light, float ambience_strength, int shininess,
                                   const QVector3D& cam_pos) const
{
    QMatrix4x4 transform_matrix = proj_matr * view_matr * model_matr;
    Polygon transformed_poly = transform(transform_matrix);

    transformed_poly.i_color_ = calcLight(i_, i_color_, light, ambience_strength, shininess, model_matr, cam_pos);
    transformed_poly.j_color_ = calcLight(j_, j_color_, light, ambience_strength, shininess, model_matr, cam_pos);
    transformed_poly.k_color_ = calcLight(k_, k_color_, light, ambience_strength, shininess, model_matr, cam_pos);

    return transformed_poly;
}

bool Polygon::isVisible(QVector3D view_direction) const
{
    return QVector3D::dotProduct(normal_, view_direction) < 0;
}

std::vector<float> Polygon::getVerticesFloat() const
{
    std::vector<float> result;
    result.reserve(18);

    result.push_back(i_.x());
    result.push_back(i_.y());
    result.push_back(i_.z());
    result.push_back(i_color_.x());
    result.push_back(i_color_.y());
    result.push_back(i_color_.z());

    result.push_back(j_.x());
    result.push_back(j_.y());
    result.push_back(j_.z());
    result.push_back(j_color_.x());
    result.push_back(j_color_.y());
    result.push_back(j_color_.z());

    result.push_back(k_.x());
    result.push_back(k_.y());
    result.push_back(k_.z());
    result.push_back(k_color_.x());
    result.push_back(k_color_.y());
    result.push_back(k_color_.z());

    return result;
}

QVector3D Polygon::calcLight(const QVector3D& point, const QVector3D& point_color,
                             const Light& light, float ambience_strength, int shininess,
                             const QMatrix4x4& model_matr, const QVector3D& cam_pos) const
{
    const QVector3D light_pos   = light.getPos();
    const QVector3D light_color = light.getColor();
    const QVector3D real_point = model_matr * point;
    const QVector3D real_norm  = (model_matr * normal_).normalized();
    const QVector3D light_dir  = (real_point - light_pos).normalized();
    const float     distance   = sqrt( (point.x() - light_pos.x()) * (point.x() - light_pos.x()) +
                                       (point.x() - light_pos.y()) * (point.x() - light_pos.y()) +
                                       (point.x() - light_pos.z()) * (point.x() - light_pos.z())
                                 );

    // Bitwise "and" returns either 1 or 0
    // Then, multiplication either sets value to 0 or does nothing
    ambience_strength *= light.getOptions() & L_AMBIENT;

    float diff = 0;
    if (light.getOptions() & LightOptions::L_DIFFUSE) {
        diff = QVector3D::dotProduct(real_norm, light_dir);
        diff = diff > 0 ? diff : 0;
        diff /= distance;
    }

    float spec = 0;
    if (light.getOptions() & LightOptions::L_SPECULAR) {
        // Math stackexchange, 13261
        QVector3D light_reflect = light_dir - 2 * QVector3D::dotProduct(real_norm, light_dir) * real_norm;
        spec = QVector3D::dotProduct((cam_pos - real_point).normalized(), light_reflect);
        spec = spec > 0 ? spec : 0;
        spec = pow(spec, shininess);
        spec /= distance;
    }

    return (ambience_strength + diff + spec) * light_color * point_color;
}
