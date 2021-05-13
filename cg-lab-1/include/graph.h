#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <GLFW/glfw3.h>
#include <vector>

#include "line.h"

std::vector<Coords> getGraphVertices(size_t tickCount);

#endif