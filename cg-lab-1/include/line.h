#pragma once
#ifndef LINE_H
#define LINE_H

#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

struct PolarCoords {
    float phy, r;
};

struct Coords {
    float x, y;
};

PolarCoords calcPolar(float phy, int a);

std::vector<PolarCoords> calcInterval(float phyLow, float phyHigh, int a = 2, float step = 0.01);

std::vector<Coords> convertToCartesian(const std::vector<PolarCoords>& original);

float findBiggest(const std::vector<Coords>& coords);

std::vector<GLfloat> normalize(const std::vector<Coords>& coords, const float maxCoord);

#endif
