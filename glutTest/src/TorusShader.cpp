#include "TorusShader.hpp"

#include "Maths.hpp"

void TorusShader::Init()
{
    ShaderProgram::Init({ ShaderInfo(VERTEX_SHADER, GL_VERTEX_SHADER),
                          ShaderInfo(FRAGMENT_SHADER, GL_FRAGMENT_SHADER)});
}

void TorusShader::bindAttributes()
{
    bindAttribute(0, "position");
}

void TorusShader::getAllUniformLocations()
{
    loc_transMat = getUniformLocation("transMat");
    loc_projMat = getUniformLocation("projMat");
    loc_viewMat = getUniformLocation("viewMat");
}

void TorusShader::loadTransformationMatrix(const glm::mat4& mat)
{
    loadMatrix(loc_transMat, mat);
}

void TorusShader::loadProjectionMatrix(const glm::mat4& mat)
{
    loadMatrix(loc_projMat, mat);
}

void TorusShader::loadViewMatrix(const Camera& camera)
{
    loadMatrix(loc_viewMat, Maths::createViewMatrix(camera));
}
