#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
    public:
        glm::vec3 position = glm::vec3(0.0f),
                  rotation = glm::vec3(0.0f),
                  up = glm::vec3(0.0f, 1.0f, 0.0f),
                  target = glm::vec3(0.0f);

        float targetOrbitDistance = 2.0f;
        float orbitDistance = targetOrbitDistance;

        void changeOrbitDistance(float dd);
        void changeOrbitDistanceInstant(float dd);
        void changeRotation(float dx, float dy);

        void updateSmooth(float dt);
    protected:

    private:

};

#endif // CAMERA_HPP
