#ifndef TORUSLINESSHADER_HPP
#define TORUSLINESSHADER_HPP

#include <ShaderProgram.hpp>
#include "Camera.hpp"

class TorusLinesShader : public ShaderProgram
{
    public:
        void Init();
        void loadTransformationMatrix(const glm::mat4& mat);
        void loadProjectionMatrix(const glm::mat4& mat);
        void loadViewMatrix(const Camera& camera);
    protected:
        void bindAttributes();
        void getAllUniformLocations();
    private:
        GLint loc_transMat, loc_projMat, loc_viewMat, loc_innerR, loc_outerR;
        static constexpr char VERTEX_SHADER[] = "torusLinesVShader.txt",
                              GEOMETRY_SHADER[] = "torusLinesGShader.txt",
                              FRAGMENT_SHADER[] = "torusLinesFShader.txt";
};


#endif // TORUSLINESSHADER_HPP
