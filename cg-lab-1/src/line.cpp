#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "line.h"

PolarCoords calcPolar(float phy, int a) {
    float r = a * cos(7 * phy);

    return PolarCoords{ phy, r };
}

std::vector<PolarCoords> calcInterval(float phyLow, float phyHigh, int a, float step) {
    std::vector<PolarCoords> result;

    for (float i = phyLow; i <= phyHigh; i += step) {
        result.push_back(calcPolar(i, a));
    }

    return result;
}

std::vector<Coords> convertToCartesian(const std::vector<PolarCoords>& original) {
    std::vector<Coords> result;
    result.reserve(original.size());

    for (const auto& e : original) {
        result.push_back({
                e.r * cos(e.phy),
                e.r * sin(e.phy)
            });
    }

    return result;
}

// This maybe a little rediculous, but this finds the biggest value in whole vector
// Value will be necessary to create 1:1 X-Y graph
float findBiggest(const std::vector<Coords>& coords) {
    // Expected that coords is not empty
    // Pick any value from coords.
    // It might or might not be the biggest
    float biggest = coords[0].x;

    for (const auto& e : coords) {
        if (biggest < e.x) { biggest = e.x; }
        if (biggest < e.y) { biggest = e.y; }
    }

    return biggest;
}

std::vector<GLfloat> normalize(const std::vector<Coords>& coords, const float maxCoord) {
    std::vector<GLfloat> vertices(coords.size() * 2);
    size_t pos = 0;
    
    for (const auto& e : coords) {
        vertices[pos++] = e.x / maxCoord; // x
        vertices[pos++] = e.y / maxCoord; // y
    }

    return vertices;
}
