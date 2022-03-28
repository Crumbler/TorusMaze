#ifndef TORUSSPOTENTITY_HPP
#define TORUSSPOTENTITY_HPP

#include "Maze.hpp"
#include "RawModel.hpp"
#include <glm/glm.hpp>

class TorusSpotEntity
{
    public:
        TorusSpotEntity(const RawModel& model, int x, int y);
        TorusSpotEntity(const RawModel& model);
        void TryMoveTo(int x, int y, Maze& maze);
        void MoveTo(int x, int y);
        void MoveTo(const glm::vec3& pos, Maze& maze);

        RawModel model;

        int curX, curY;
    protected:

    private:
};

#endif // TORUSSPOTENTITY_HPP
