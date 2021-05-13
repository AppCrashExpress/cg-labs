#ifndef HORSESHOE_H
#define HORSESHOE_H

#include <QVector3D>
#include <vector>

#include "polygon.h"

class HorseshoeMesh
{
public:
    HorseshoeMesh();

    void createHorseshoe(unsigned int side_count,
                         QVector3D color = QVector3D(1.0f, 1.0f, 1.0f));
    std::vector<Polygon> getPolygons() const;
    std::vector<float>   getData()     const;

    int getSideCount();

private:
    std::vector<QVector3D> createEllipse(unsigned int side_count, float radius);
    std::vector<QVector3D> translateAndRotateZ(
            const std::vector<QVector3D>& ellipse,
            QVector3D centre_pos,
            float angle
    );

    std::vector<Polygon> polygons_;
    int side_count_;
};

#endif // HORSESHOE_H
