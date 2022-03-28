#include "Renderer.hpp"
#include <GL/glew.h>

#include "Maths.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>
#include "TorusShader.hpp"

void Renderer::Prepare()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderCube(Entity& entity, StaticShader& shader)
{
    RawModel& model = entity.model;
    glm::mat4 transMat = Maths::createTransformationMartix(entity.position,
                                                           entity.rotation,
                                                           entity.scale);
    shader.loadTransformationMatrix(transMat);
    glBindVertexArray(model.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1); // DrawArrays when there aren't indices
    glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Renderer::RenderTorusLines(Entity& entity, TorusLinesShader& shader)
{
    RawModel& model = entity.model;
    glm::mat4 transMat = Maths::createTransformationMartix(entity.position,
                                                           entity.rotation,
                                                           entity.scale);
    shader.loadTransformationMatrix(transMat);
    glBindVertexArray(model.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_LINES, 0, model.getVertexCount());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Renderer::RenderTorus(Entity& entity, TorusShader& shader)
{
    RawModel& model = entity.model;
    glm::mat4 transMat = Maths::createTransformationMartix(entity.position,
                                                           entity.rotation,
                                                           entity.scale);
    shader.loadTransformationMatrix(transMat);
    glBindVertexArray(model.getVaoID());
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLE_STRIP, model.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::RenderTorusSpot(TorusSpotEntity& entity, TorusSpotShader& shader)
{
    RawModel& model = entity.model;

    // identity matrix
    shader.loadTransformationMatrix(glm::mat4(1.0f));

    glBindVertexArray(model.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, model.getVertexCount());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Renderer::updateProjectionMatrix(ShaderProgram& shader, int width, int height)
{
    const float ratio = (float)width / (float)height;
    projMat = glm::perspective(FOV_R, ratio, NEAR_PLANE, FAR_PLANE);
    shader.Start();
    shader.loadProjectionMatrix(projMat);
    shader.Stop();
}

void Renderer::createProjectionMatrix()
{
    constexpr float ratio = 16.0f / 9.0f;
    projMat = glm::perspective(FOV_R, ratio, NEAR_PLANE, FAR_PLANE);
}

Renderer::Renderer(StaticShader& shader)
{
    createProjectionMatrix();
    shader.Start();
    shader.loadProjectionMatrix(projMat);
    shader.Stop();
}
