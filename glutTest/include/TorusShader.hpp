#ifndef TORUSSHADER_HPP
#define TORUSSHADER_HPP

#include "ShaderProgram.hpp"
#include "Camera.hpp"

class TorusShader : public ShaderProgram
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
        static constexpr char VERTEX_SHADER[] = "torusVShader.txt",
                              FRAGMENT_SHADER[] = "torusFShader.txt";
};

#endif // TORUSSHADER_HPP
