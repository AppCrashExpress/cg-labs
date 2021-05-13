#include <vector>

#include "graph.h"
#include "line.h"


std::vector<Coords> getGraphVertices(size_t tickCount) {
    
    // Account for arrowheads at the ends;
    tickCount += 2;

    constexpr float tickLength = 0.1f;
    std::vector<Coords> result((2 + 2 * tickCount) * 2);

    for (size_t i = 0; i < tickCount; ++i) {
        result[i * 2].x = result[i * 2 + 1].x = -1.0f + i * (2.0f / (tickCount - 1));

        result[i * 2].y     = 0 + tickLength / 2;
        result[i * 2 + 1].y = 0 - tickLength / 2;
    }

    for (size_t i = tickCount; i < tickCount * 2; ++i) {
        result[i * 2].y = result[i * 2 + 1].y = -1.0f + (i - tickCount) * (2.0f / (tickCount - 1));

        result[i * 2].x     = 0 + tickLength / 2;
        result[i * 2 + 1].x = 0 - tickLength / 2;
    }
    
    
    // X axis
    result[4 * tickCount].y = result[4 * tickCount + 1].y = 0;
    result[4 * tickCount].x     = -1.0f;
    result[4 * tickCount + 1].x =  1.0f;
    // Change first and last tick into arrowhead
    result[0].x = result[2 * (tickCount - 1)].x = 1.0f - tickLength / 2;
    result[0].y = tickLength / 4;
    result[2 * (tickCount - 1)].y = - tickLength / 4;
    result[1] = result[2 * (tickCount - 1) + 1] = { 1.0f, 0.0f };

    // Y axis
    result[4 * tickCount + 2].x = result[4 * tickCount + 3].x = 0;
    result[4 * tickCount + 2].y = -1.0f;
    result[4 * tickCount + 3].y =  1.0f;
    // Change first and last tick into arrowhead
    result[2 * tickCount].y = result[tickCount * 4 - 2].y = 1.0f - tickLength / 2;
    result[2 * tickCount].x = tickLength / 4;
    result[tickCount * 4 - 2].x = -tickLength / 4;
    result[2 * tickCount + 1] = result[tickCount * 4 - 1] = { 0.0f, 1.0f };

    return result;
}
