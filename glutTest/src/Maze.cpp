#include "Maze.hpp"

#include <stack>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <fstream>

void Maze::writeToFile(Cell data[Constants::GRID_HEIGHT][Constants::GRID_WIDTH])
{
    using namespace Constants;

    std::ofstream myFile("maze.txt");

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH - 1; ++j)
            myFile << int(data[i][j]) << ' ';

        myFile << int(data[i][GRID_WIDTH - 1]) << '\n';
    }

    myFile.close();

    std::cout << "Wrote to file\n";
}

bool Maze::CheckWall(int aX, int aY, int bX, int bY)
{
    using namespace Constants;

    if (aX == GRID_WIDTH - 1 && bX == 0)
        aX -= GRID_WIDTH;

    if (bX == GRID_WIDTH - 1 && aX == 0)
        bX -= GRID_WIDTH;

    if (aY == GRID_HEIGHT - 1 && bY == 0)
        aY -= GRID_HEIGHT;

    if (bY == GRID_HEIGHT - 1 && aY == 0)
        bY -= GRID_HEIGHT;

    if ((abs(aX - bX) + abs(aY - bY)) != 1)
        return true;

    if (aX < bX)
    {
        aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
        aX = (aX + GRID_WIDTH) % GRID_WIDTH;

        return mazeData[aY][aX] & cWallRight;
    }
    else
        if (aX > bX)
        {
            aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
            bX = (bX + GRID_WIDTH) % GRID_WIDTH;

            return mazeData[aY][bX] & cWallRight;
        }
        else
            if (aY < bY)
            {
                aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
                aX = (aX + GRID_WIDTH) % GRID_WIDTH;

                return mazeData[aY][aX] & cWallBottom;
            }
            else
            {  // bY < aY
                bY = (bY + GRID_HEIGHT) % GRID_HEIGHT;
                aX = (aX + GRID_WIDTH) % GRID_WIDTH;

                return mazeData[bY][aX] & cWallBottom;
            }
}

void Maze::BreakWall(int aX, int aY, int bX, int bY)
{
    using namespace Constants;

    if (aX < bX)
    {
        aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
        aX = (aX + GRID_WIDTH) % GRID_WIDTH;
        bX = (bX + GRID_WIDTH) % GRID_WIDTH;

        mazeData[aY][aX] &= ~cWallRight;
        mazeData[aY][bX] &= ~cWallLeft;
    }
    else
        if (aX > bX)
        {
            aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
            aX = (aX + GRID_WIDTH) % GRID_WIDTH;
            bX = (bX + GRID_WIDTH) % GRID_WIDTH;

            mazeData[aY][bX] &= ~cWallRight;
            mazeData[aY][aX] &= ~cWallLeft;
        }
        else
            if (aY < bY)
            {
                aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
                bY = (bY + GRID_HEIGHT) % GRID_HEIGHT;
                aX = (aX + GRID_WIDTH) % GRID_WIDTH;

                mazeData[aY][aX] &= ~cWallBottom;
                mazeData[bY][aX] &= ~cWallTop;
            }
            else
            {  // bY < aY
                aY = (aY + GRID_HEIGHT) % GRID_HEIGHT;
                bY = (bY + GRID_HEIGHT) % GRID_HEIGHT;
                aX = (aX + GRID_WIDTH) % GRID_WIDTH;

                mazeData[bY][aX] &= ~cWallBottom;
                mazeData[aY][aX] &= ~cWallTop;
            }
}

std::pair<int, int> Maze::SafePoint(int x, int y)
{
    using Constants::GRID_WIDTH, Constants::GRID_HEIGHT;

    return std::pair<int, int>((x + GRID_WIDTH) % GRID_WIDTH,
                               (y + GRID_HEIGHT) % GRID_HEIGHT);
}

bool Maze::IsUnvisited(int x, int y)
{
    using namespace Constants;

    x = (x + GRID_WIDTH) % GRID_WIDTH;
    y = (y + GRID_HEIGHT) % GRID_HEIGHT;

    return (mazeData[y][x] & cVisited) == 0;
}

bool Maze::IsVisited(int x, int y)
{
    using namespace Constants;

    x = (x + GRID_WIDTH) % GRID_WIDTH;
    y = (y + GRID_HEIGHT) % GRID_HEIGHT;

    return (mazeData[y][x] & cVisited) != 0;
}

bool Maze::GetRandNeighbor(int& nX, int& nY)
{
    using PointPair = std::pair<int, int>;

    std::vector<PointPair> neighbors;

    if (IsUnvisited(nX - 1, nY))
        neighbors.push_back(PointPair(nX - 1, nY));

    if (IsUnvisited(nX, nY - 1))
        neighbors.push_back(PointPair(nX, nY - 1));

    if (IsUnvisited(nX + 1, nY))
        neighbors.push_back(PointPair(nX + 1, nY));

    if (IsUnvisited(nX, nY + 1))
        neighbors.push_back(PointPair(nX, nY + 1));

    if (neighbors.size())
    {
        const int randInd = rand() % neighbors.size();

        nX = neighbors[randInd].first;
        nY = neighbors[randInd].second;

        neighbors.clear();

        return true;
    }

    return false;
}

void Maze::FillMaze()
{
    memset(mazeData, cWalled, sizeof(Cell) * Constants::GRID_WIDTH * Constants::GRID_HEIGHT);
}

void Maze::GenerateMaze()
{
    FillMaze();

    using namespace Constants;
    using PointPair = std::pair<int, int>;

    std::stack<PointPair> points;

    int curY = 0, curX = 0, neighX, neighY;

    mazeData[curY][curX] |= cVisited;

    do
    {
        neighX = curX;
        neighY = curY;

        if (GetRandNeighbor(neighX, neighY))
        {
            points.push(SafePoint(curX, curY));

            BreakWall(curX, curY, neighX, neighY);

            curX = (neighX + GRID_WIDTH) % GRID_WIDTH;
            curY = (neighY + GRID_HEIGHT) % GRID_HEIGHT;

            mazeData[curY][curX] |= cVisited;
        }
        else
        {
            const auto p = points.top();
            points.pop();

            curX = p.first;
            curY = p.second;
        }
    }
    while (points.size());

    for (int i = 0; i < Constants::GRID_HEIGHT; ++i)
        for (int j = 0; j < Constants::GRID_WIDTH; ++j)
            if ((mazeData[i][j] & cVisited) == 0 || mazeData[i][j] == cWalled)
                std::cout << "wrong wall\n";
}
