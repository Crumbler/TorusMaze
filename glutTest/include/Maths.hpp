#ifndef MATHS_HPP
#define MATHS_HPP

#include "Camera.hpp"
#include <math.h>
#include <glm/glm.hpp>

namespace Maths
{
    glm::mat4 createTransformationMartix(const glm::vec3& translation,
                                                    const glm::vec3& rotation,
                                                    const float scale);

    glm::mat4 createViewMatrix(const Camera& camera);

    float constrainAngle(float x);
    glm::vec3 orbitPos(const Camera& camera);
    glm::vec3 orbitTarget(const Camera& camera);
    glm::vec3 orbitUp(const Camera& camera);
    glm::vec3 constrainAngleVector(glm::vec3 v);
    glm::vec3 RayFromCam(const Camera& camera, const int x, const int y, const int width, const int height,
                         const glm::mat4& invProj, const glm::mat4& invView);

    constexpr float PI = acos(-1.0f);

}

#endif // MATHS_HPP
