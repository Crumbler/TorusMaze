#include "Camera.hpp"

#include <algorithm>
#include "Maths.hpp"

void Camera::changeOrbitDistance(float dd)
{
    targetOrbitDistance = std::clamp(targetOrbitDistance + dd, 1.5f, 5.0f);
}

void Camera::changeOrbitDistanceInstant(float dd)
{
    targetOrbitDistance = std::clamp(targetOrbitDistance + dd, 1.5f, 5.0f);
    orbitDistance = targetOrbitDistance;
}

void Camera::changeRotation(float dx, float dy)
{
    rotation.x += dx;
    rotation.y += dy;
    rotation.x = Maths::constrainAngle(rotation.x);

    constexpr float mult = 2.0f;

    if (rotation.y > Maths::PI * mult)
        rotation.y -= Maths::PI * mult;

    if (rotation.y < -Maths::PI * mult)
        rotation.y += Maths::PI * mult;
}

void Camera::updateSmooth(float dt)
{
    constexpr float smoothRatio = 10.0f;
    const float change = std::clamp(smoothRatio * dt, 0.0f, 1.0f);
    orbitDistance += (targetOrbitDistance - orbitDistance) * change;
}
