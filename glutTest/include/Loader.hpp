#ifndef LOADER_H
#define LOADER_H

#include "RawModel.hpp"
#include "Maze.hpp"
#include <glm/glm.hpp>
#include <vector>

class Loader
{
    public:
        Loader();
        RawModel loadToVAO(const std::vector<float>& positions,
                           const std::vector<int>& indices,
                           const std::vector<float>& colors);

        RawModel loadTorus();
        RawModel loadTorusLines(const Maze& maze);
        RawModel loadTorusSpot(const glm::vec3& color);
        void CleanUp();

        static void MazeToAttribs(const Maze& maze, std::vector<float>& attribs);

        std::vector<float> mazeAttribs;
    protected:

    private:
        std::vector<GLuint> vaos, vbos;

        int createVAO();
        void storeDataInAttributeListInt(int attributeNumber, const std::vector<int>& data);
        void storeDataInAttributeListDynamic(int attributeNumber, const std::vector<float>& data);
        void storeDataInAttributeList(int attributeNumber, const std::vector<float>& data);
        void sDIALTorusLines(int attributeNumber, const std::vector<float>& data);
        void sDIALTorusSpot(int attributeNumber, const std::vector<float>& data);
        void unbindVAO();

        void bindIndicesBuffer(const std::vector<int>& indices);

        static void genTorusLinesVerticesAngular(std::vector<float>& angles);
        static void getTorusVertices(std::vector<float>& vertices);
        static void genTorusIndices(std::vector<int>& indices);
        static void getTorusLinesVertices(std::vector<float>& vertices);
};

#endif // LOADER_H
