#ifndef RAWMODEL_HPP_INCLUDED
#define RAWMODEL_HPP_INCLUDED

#include <GL/glew.h>
#include <vector>

class RawModel
{
    public:
        RawModel();
        RawModel(GLuint vaoID, GLuint vertexCount);
        GLuint getVaoID();
        GLuint getVertexCount();
        void updateVBO(int attributeNumber, const std::vector<float>& data);
        void updateVBODynamic(int attributeNumber, const std::vector<float>& data);
    protected:

    private:
        GLuint vaoID, vertexCount;
};

#endif // RAWMODEL_HPP_INCLUDED
