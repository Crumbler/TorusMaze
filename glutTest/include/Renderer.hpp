#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Maths.hpp"
#include "Entity.hpp"
#include "StaticShader.hpp"
#include "RawModel.hpp"
#include <glm/glm.hpp>
#include "TorusShader.hpp"
#include "TorusLinesShader.hpp"
#include "TorusSpotShader.hpp"
#include "TorusSpotEntity.hpp"

class Renderer
{
    public:
        Renderer(StaticShader& shader);
        void RenderCube(Entity& entity, StaticShader& shader);
        void Prepare();
        void RenderTorus(Entity& entity, TorusShader& shader);
        void RenderTorusLines(Entity& entity, TorusLinesShader& shader);
        void RenderTorusSpot(TorusSpotEntity& entity, TorusSpotShader& shader);
        void updateProjectionMatrix(ShaderProgram& shader, int width, int height);

        static constexpr float FOV = 90.0f,
                               NEAR_PLANE = 0.1f,
                               FAR_PLANE = 100.0f,
                               FOV_R = FOV / 180.0f * Maths::PI;
    protected:

    private:
        glm::mat4 projMat;

        void createProjectionMatrix();
};

#endif // RENDERER_HPP
