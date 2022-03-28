#include "StaticShader.hpp"

#include "Maths.hpp"

void StaticShader::Init()
{
    ShaderProgram::Init({ ShaderInfo(VERTEX_SHADER, GL_VERTEX_SHADER),
                          ShaderInfo(FRAGMENT_SHADER, GL_FRAGMENT_SHADER)});
}

void StaticShader::bindAttributes()
{
    bindAttribute(0, "position");
    bindAttribute(1, "color");
}

void StaticShader::getAllUniformLocations()
{
    loc_transMat = getUniformLocation("transMat");
    loc_projMat = getUniformLocation("projMat");
    loc_viewMat = getUniformLocation("viewMat");
}

void StaticShader::loadTransformationMatrix(const glm::mat4& mat)
{
    loadMatrix(loc_transMat, mat);
}

void StaticShader::loadProjectionMatrix(const glm::mat4& mat)
{
    loadMatrix(loc_projMat, mat);
}

void StaticShader::loadViewMatrix(const Camera& camera)
{
    loadMatrix(loc_viewMat, Maths::createViewMatrix(camera));
}
