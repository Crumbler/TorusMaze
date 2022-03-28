#include "TorusSpotShader.hpp"

#include "Constants.hpp"
#include "Maths.hpp"
#include "ShaderInfo.hpp"

void TorusSpotShader::Init()
{
    ShaderProgram::Init({ ShaderInfo(VERTEX_SHADER, GL_VERTEX_SHADER),
                          ShaderInfo(FRAGMENT_SHADER, GL_FRAGMENT_SHADER)});

    glUseProgram(programID);

    loadFloat(loc_innerR, Constants::R);
    loadFloat(loc_outerR, Constants::r);

    glUseProgram(0);
}

void TorusSpotShader::bindAttributes()
{
    bindAttribute(0, "position");
    bindAttribute(1, "vColor");
    bindAttribute(2, "rectCoord");
}

void TorusSpotShader::getAllUniformLocations()
{
    loc_innerR = getUniformLocation("innerR");
    loc_outerR = getUniformLocation("outerR");
    loc_transMat = getUniformLocation("transMat");
    loc_projMat = getUniformLocation("projMat");
    loc_viewMat = getUniformLocation("viewMat");
}

void TorusSpotShader::loadTransformationMatrix(const glm::mat4& mat)
{
    loadMatrix(loc_transMat, mat);
}

void TorusSpotShader::loadProjectionMatrix(const glm::mat4& mat)
{
    loadMatrix(loc_projMat, mat);
}

void TorusSpotShader::loadViewMatrix(const Camera& camera)
{
    loadMatrix(loc_viewMat, Maths::createViewMatrix(camera));
}
