#include "TorusSpotEntity.hpp"

#include "Constants.hpp"
#include "Maths.hpp"
#include <iostream>

TorusSpotEntity::TorusSpotEntity(const RawModel& model, int x, int y) : model(model)
{
    curX = x;
    curY = y;
    MoveTo(curX, curY);
}

TorusSpotEntity::TorusSpotEntity(const RawModel& model) : model(model)
{
    curX = curY = 0;
    MoveTo(curX, curY);
}

void TorusSpotEntity::TryMoveTo(int x, int y, Maze& maze)
{
    if (!maze.CheckWall(x, y, curX, curY))
        MoveTo(x, y);
}

void TorusSpotEntity::MoveTo(const glm::vec3& pos, Maze& maze)
{
    using namespace Constants;

    const float baseAngle = atan2f(pos.x, pos.z);

    int newY = floor(baseAngle / (2.0f * Maths::PI / GRID_HEIGHT));

    newY = (newY + GRID_HEIGHT) % GRID_HEIGHT;

    const glm::vec3 xAxis = glm::normalize(glm::vec3(pos.x, 0.0f, pos.z)),
                    yAxis = glm::vec3(0.0f, 1.0, 0.0f),
                    deltaPos = pos - xAxis * R;

    const float endAngle = atan2f(glm::dot(yAxis, deltaPos), glm::dot(xAxis, deltaPos));

    int newX = floor(endAngle / (2.0f * Maths::PI / GRID_WIDTH));

    newX = (newX + GRID_WIDTH) % GRID_WIDTH;

    TryMoveTo(newX, newY, maze);
}

void TorusSpotEntity::MoveTo(int x, int y)
{
    using Maths::PI;
    using namespace Constants;

    curX = (x + GRID_WIDTH) % GRID_WIDTH;
    curY = (y + GRID_HEIGHT) % GRID_HEIGHT;

    const std::vector<float> angles =
    {
        curY * PI * 2.0f / GRID_HEIGHT, curX * PI * 2.0f / GRID_WIDTH,
        (curY + 1) * PI * 2.0f / GRID_HEIGHT, curX * PI * 2.0f / GRID_WIDTH,
        curY * PI * 2.0f / GRID_HEIGHT, (curX + 1) * PI * 2.0f / GRID_WIDTH,
        (curY + 1) * PI * 2.0f / GRID_HEIGHT, (curX + 1) * PI * 2.0f / GRID_WIDTH
    };

    model.updateVBODynamic(0, angles);
}
