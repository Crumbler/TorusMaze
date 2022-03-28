#ifndef RAWMODEL_H
#define RAWMODEL_H

#include <GL/glew.h>

class RawModel
{
    public:
        RawModel(GLuint vaoID, GLuint vertexCount);

    protected:

    private:
        GLuint vaoID, vertexCount;
};

#endif // RAWMODEL_H
