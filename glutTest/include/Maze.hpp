#ifndef MAZE_HPP
#define MAZE_HPP

#include "Constants.hpp"
#include <vector>

using Cell = unsigned char;

class Maze
{
    public:
        void GenerateMaze();
        bool CheckWall(int aX, int aY, int bX, int bY);
        Cell mazeData[Constants::GRID_HEIGHT][Constants::GRID_WIDTH];

        static constexpr Cell cVisited = 1,
                              cWallLeft = 2,
                              cWallTop = 4,
                              cWallRight = 8,
                              cWallBottom = 16,
                              cWalled = cWallLeft | cWallTop | cWallRight | cWallBottom,
                              cWalledVisited = cWalled | cVisited;
    protected:

    private:
        void FillMaze();
        void BreakWall(int aX, int aY, int bX, int bY);
        bool IsVisited(int x, int y);
        bool IsUnvisited(int x, int y);
        bool GetRandNeighbor(int& nX, int& nY);
        static std::pair<int, int> SafePoint(int x, int y);
        static void writeToFile(Cell data[Constants::GRID_HEIGHT][Constants::GRID_WIDTH]);
};

#endif // MAZE_HPP
