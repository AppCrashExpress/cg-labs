#include <math.h>

#include "horseshoe.h"

HorseshoeMesh::HorseshoeMesh()
{

}

void HorseshoeMesh::createHorseshoe(unsigned int side_count, QVector3D color)
{
    side_count_ = side_count;
    polygons_.clear();

    constexpr float radius = 0.2;
    std::vector<QVector3D> ellipses[5];

    ellipses[2] = translateAndRotateZ(
                    createEllipse(side_count, radius),
                    QVector3D(0.0, 0.5, 0.0),
                    90
                );


    ellipses[1] = translateAndRotateZ(
                    createEllipse(side_count, radius),
                    QVector3D(-0.5, 0.25, 0.0),
                    180 - 45
                );


    ellipses[3] = translateAndRotateZ(
                    createEllipse(side_count, radius),
                    QVector3D(0.5, 0.25, 0.0),
                    45
                );


    ellipses[0] = translateAndRotateZ(
                    createEllipse(side_count, radius),
                    QVector3D(-0.5, -0.5, 0.0),
                    180 + 30
                );


    ellipses[4] = translateAndRotateZ(
                    createEllipse(side_count, radius),
                    QVector3D(0.5, -0.5, 0.0),
                    0 - 30
                );

    // Push first and last ellipses to show bases
    for (size_t i = 1; i <= side_count; ++i) {
        polygons_.push_back(Polygon(
                               ellipses[0][i % side_count + 1],
                               ellipses[0][0],
                               ellipses[0][i],
                               color
                           ));
    }

    // Last ellipse pushed in reverse order;
    for (size_t i = 1; i <= side_count; ++i) {
        polygons_.push_back(Polygon(
                               ellipses[4][i],
                               ellipses[4][0],
                               ellipses[4][i % side_count + 1],
                               color
                           ));
    }

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 1; j <= side_count; ++j) {
            polygons_.push_back(Polygon(
                                    ellipses[i][j],
                                    ellipses[i + 1][j % side_count + 1],
                                    ellipses[i][j % side_count + 1],
                                    color
                                    ));
            polygons_.push_back(Polygon(
                                    ellipses[i][j],
                                    ellipses[i + 1][j],
                                    ellipses[i + 1][j % side_count + 1],
                                    color
                                    ));
        }
    }
}

std::vector<Polygon> HorseshoeMesh::getPolygons() const
{
    return polygons_;
}

int HorseshoeMesh::getSideCount()
{
    return side_count_;
}

std::vector<QVector3D> HorseshoeMesh::createEllipse(unsigned int side_count,
                                                float r) {
    std::vector<QVector3D> result;
    result.reserve(side_count + 1);

    result.push_back(QVector3D(0, 0, 0));
    for (size_t i = 0; i < side_count; ++i) {
        result.push_back(QVector3D(
                             r * cos(2 * M_PI * i / side_count),
                             0,
                             r * sin(2 * M_PI * i / side_count)
                             ));
    }

    return result;
}

std::vector<QVector3D> HorseshoeMesh::translateAndRotateZ(
        const std::vector<QVector3D>& ellipse,
        QVector3D centre_pos,
        float angle)
{
    QMatrix4x4 positor;
    positor.setToIdentity();
    positor.translate(centre_pos);
    positor.rotate(angle, 0.0, 0.0, 1.0);

    std::vector<QVector3D> translated_ellipse;
    translated_ellipse.reserve(ellipse.size());
    for (const auto& e : ellipse) {
        translated_ellipse.push_back(positor * e);
    }

    return translated_ellipse;
}
