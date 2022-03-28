#ifndef STATICSHADER_HPP
#define STATICSHADER_HPP

#include <ShaderProgram.hpp>
#include "Camera.hpp"

class StaticShader : public ShaderProgram
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
        GLint loc_transMat, loc_projMat, loc_viewMat;
        static constexpr char VERTEX_SHADER[] = "vShader.txt",
                              FRAGMENT_SHADER[] = "fShader.txt";
};

#endif // STATICSHADER_HPP
