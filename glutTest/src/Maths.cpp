#include "Maths.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Constants.hpp"

extern bool doubleOrbit;

glm::mat4 Maths::createTransformationMartix(const glm::vec3& translation,
                                            const glm::vec3& rotation,
                                            const float scale)
{
    using glm::rotate, glm::vec3;

    glm::mat4 mat(1.0f);

    mat = glm::translate(mat, translation);
    mat = rotate(mat, rotation.x, vec3(1.0f, 0.0f, 0.0f));
    mat = rotate(mat, rotation.y, vec3(0.0f, 1.0f, 0.0f));
    mat = rotate(mat, rotation.z, vec3(0.0f, 0.0f, 1.0f));
    mat = glm::scale(mat, vec3(scale));

    return mat;
}

glm::vec3 Maths::orbitTarget(const Camera& camera)
{
    if (doubleOrbit)
    {
        return glm::vec3(sinf(camera.rotation.x),
                         0.0f,
                         cosf(camera.rotation.x)) * Constants::R;
    }
    else
        return camera.target;
}

glm::vec3 Maths::orbitUp(const Camera& camera)
{
    using glm::rotate, glm::vec3;

    vec3 res(0.0f, 1.0f, 0.0f);

    res = rotate(res, camera.rotation.y, vec3(-1.0, 0.0f, 0.0f));

    res = rotate(res, camera.rotation.x, vec3(0.0f, 1.0f, 0.0f));

    return res;
}

glm::vec3 Maths::orbitPos(const Camera& camera)
{
    const float s1 = sinf(camera.rotation.x),
                c1 = cosf(camera.rotation.x);

    const float flatDist = cosf(camera.rotation.y),
                vertDist = sinf(camera.rotation.y);

    const glm::vec3 base(s1 * Constants::R,
                         0.0f,
                         c1 * Constants::R);

    const glm::vec3 end = glm::vec3(flatDist * s1,
                                    vertDist,
                                    flatDist * c1) * camera.orbitDistance;

    if (doubleOrbit)
        return base + end;
    else
        return end;
}

glm::mat4 Maths::createViewMatrix(const Camera& camera)
{
    return glm::lookAt(orbitPos(camera), orbitTarget(camera), orbitUp(camera));
}

float Maths::constrainAngle(float x)
{
    x = fmod(x + PI * 2.0f, PI * 2.0f);

    return x;
}

glm::vec3 Maths::constrainAngleVector(glm::vec3 v)
{
    return glm::vec3(constrainAngle(v.x),
                     constrainAngle(v.y),
                     constrainAngle(v.z));
}

glm::vec3 Maths::RayFromCam(const Camera& camera, const int x, const int y, const int width, const int height,
                            const glm::mat4& invProj, const glm::mat4& invView)
{
    using glm::vec4, glm::vec3;

    const float aX = (2.0f * x) / width - 1.0f;
    const float aY = 1.0f - (2.0f * y) / height;

    const vec4 ray_clip = vec4(aX, aY, -1.0f, 1.0f);

    vec4 ray_eye = invProj * ray_clip;

    ray_eye.z = -1.0f;
    ray_eye.w = 0.0f;

    const vec4 ray_wor = invView * ray_eye;

    const vec3 r3 = glm::normalize(vec3(ray_wor.x, ray_wor.y, ray_wor.z));

    return r3;
}
