#include "Loader.hpp"
#include <GL/glew.h>

#include "Maze.hpp"
#include "Maths.hpp"
#include "Constants.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

Loader::Loader()
{

}

void Loader::MazeToAttribs(const Maze& maze, std::vector<float>& attribs)
{
    using namespace Constants;

    float* attribPointer = attribs.data();

    const auto& mz = maze.mazeData;

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            if (mz[i][j] & Maze::cWallTop)
                attribPointer[0] = attribPointer[1] = 1.0f;
            else
                attribPointer[0] = attribPointer[1] = 0.0f;

            attribPointer += 2;
        }

        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            if (mz[i][j] & Maze::cWallLeft)
                attribPointer[0] = attribPointer[1] = 1.0f;
            else
                attribPointer[0] = attribPointer[1] = 0.0f;

            attribPointer += 2;
        }
    }
}

void pushVec2(std::vector<float>& vector, const glm::vec2& v)
{
    vector.push_back(v.x);
    vector.push_back(v.y);
}

void pushVec3(std::vector<float>& vector, const glm::vec3& v)
{
    vector.push_back(v.x);
    vector.push_back(v.y);
    vector.push_back(v.z);
}

glm::vec2 genTorusVertexAngular(int i, int j)
{
    using Maths::PI;
    return glm::vec2(i * 2.0f * PI / Constants::GRID_HEIGHT,
                     j * 2.0f * PI / Constants::GRID_WIDTH);
}

glm::vec3 genTorusVertex(int i, int j)
{
    constexpr glm::vec3 up(0.0f, 1.0f, 0.0f);

    using Maths::PI;
    using namespace Constants;

    const float baseAngle = i * 2.0f * PI / GRID_HEIGHT,
                endAngle = j * 2.0f * PI / GRID_WIDTH;

    glm::vec3 base(R * sinf(baseAngle), 0.0f, R * cosf(baseAngle));
    glm::vec3 end(0.0f, r * sinf(endAngle), r * cosf(endAngle));
    end = glm::rotate(end, baseAngle, up);

    return base + end;
}

void genTorusVertices(std::vector<float>& vertices)
{
    using namespace Constants;

    for (int i = 0; i < GRID_HEIGHT; ++i)
        for (int j = 0; j < GRID_WIDTH; ++j)
            pushVec3(vertices, genTorusVertex(i, j));
}

void Loader::sDIALTorusSpot(int attributeNumber, const std::vector<float>& data)
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attributeNumber, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

RawModel Loader::loadTorusSpot(const glm::vec3& color)
{
    const std::vector<float> angles(8), rectCoords =
    {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
    };
    std::vector<float> colors(12);

    for (int i = 0; i < colors.size(); i += 3)
    {
        colors[i] = color.x;
        colors[i + 1] = color.y;
        colors[i + 2] = color.z;
    }

    int vaoID = createVAO();
    sDIALTorusSpot(0, angles);
    storeDataInAttributeList(1, colors);
    sDIALTorusLines(2, rectCoords);
    unbindVAO();
    return RawModel(vaoID, angles.size() / 2);
}

void Loader::genTorusIndices(std::vector<int>& indices)
{
    using namespace Constants;

    for (int i = 0; i < GRID_HEIGHT - 1; ++i)
    {
        const int firstInd = i * GRID_WIDTH,
                  secondInd = (i + 1) * GRID_WIDTH;

        indices.push_back(firstInd);

        for (int j = 1; j < GRID_WIDTH; ++j)
            indices.insert(indices.end(),
                           { secondInd + j,
                             firstInd + j });

        indices.insert(indices.end(),
                       { secondInd,
                         firstInd,
                         secondInd + 1});
    }

    const int firstInd = (GRID_HEIGHT - 1) * GRID_WIDTH,
              secondInd = 0;

    indices.push_back(firstInd);

    for (int j = 1; j < GRID_WIDTH; ++j)
            indices.insert(indices.end(),
                           { secondInd + j,
                             firstInd + j });

    indices.insert(indices.end(),
                   { secondInd,
                     firstInd,
                     secondInd + 1});
}

RawModel Loader::loadTorus()
{
    std::vector<int> indices;
    std::vector<float> vertices;

    genTorusVertices(vertices);
    genTorusIndices(indices);

    int vaoID = createVAO();
    bindIndicesBuffer(indices);
    storeDataInAttributeList(0, vertices);
    unbindVAO();
    return RawModel(vaoID, indices.size());
}

void genTorusLinesVertices(std::vector<float>& vertices)
{
    using namespace Constants;

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            pushVec3(vertices, genTorusVertex(i, j));
            pushVec3(vertices, genTorusVertex(i, (j + 1) % GRID_WIDTH));
        }

        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            pushVec3(vertices, genTorusVertex(i, j));
            pushVec3(vertices, genTorusVertex((i + 1) % GRID_HEIGHT, j));
        }
    }
}

void Loader::genTorusLinesVerticesAngular(std::vector<float>& angles)
{
    using namespace Constants;

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            pushVec2(angles, genTorusVertexAngular(i, j));
            pushVec2(angles, genTorusVertexAngular(i, (j + 1) % GRID_WIDTH));
        }

        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            pushVec2(angles, genTorusVertexAngular(i, j));
            pushVec2(angles, genTorusVertexAngular((i + 1) % GRID_HEIGHT, j));
        }
    }
}

RawModel Loader::loadTorusLines(const Maze& maze)
{
    std::vector<float> angles;

    genTorusLinesVerticesAngular(angles);

    mazeAttribs = std::vector<float>(angles.size() / 2);
    MazeToAttribs(maze, mazeAttribs);

    int vaoID = createVAO();
    sDIALTorusLines(0, angles);
    storeDataInAttributeListDynamic(1, mazeAttribs);
    unbindVAO();
    return RawModel(vaoID, angles.size() / 2);
}

RawModel Loader::loadToVAO(const std::vector<float>& positions,
                           const std::vector<int>& indices,
                           const std::vector<float>& colors)
{
    int vaoID = createVAO();
    bindIndicesBuffer(indices);
    storeDataInAttributeList(0, positions);
    storeDataInAttributeList(1, colors);
    unbindVAO();
    return RawModel(vaoID, indices.size());
}

int Loader::createVAO()
{
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    vaos.push_back(vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

void Loader::sDIALTorusLines(int attributeNumber, const std::vector<float>& data)
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attributeNumber, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::storeDataInAttributeList(int attributeNumber, const std::vector<float>& data)
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::storeDataInAttributeListInt(int attributeNumber, const std::vector<int>& data)
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(attributeNumber, 1, GL_INT, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::storeDataInAttributeListDynamic(int attributeNumber, const std::vector<float>& data)
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attributeNumber, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::unbindVAO()
{
    glBindVertexArray(0);
}

void Loader::CleanUp()
{
    if (!vaos.empty())
        glDeleteVertexArrays(vaos.size(), vaos.data());

    if (!vbos.empty())
        glDeleteBuffers(vbos.size(), vbos.data());
}

void Loader::bindIndicesBuffer(const std::vector<int>& indices)
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}
