#include "Entity.hpp"

#include "Maths.hpp"

Entity::Entity(const RawModel& model, const glm::vec3& position,
               const glm::vec3 rotation, const float scale)
    : model(model), position(position), rotation(rotation), scale(scale) { }

void Entity::MoveBy(const float dx, const float dy, const float dz)
{
    position.x += dx;
    position.y += dy;
    position.z += dz;
}

void Entity::MoveBy(const glm::vec3& dv)
{
    position += dv;
}

void Entity::RotateBy(const float dx, const float dy, const float dz)
{
    using Maths::constrainAngle;
    rotation.x = constrainAngle(rotation.x + dx);
    rotation.y = constrainAngle(rotation.y + dy);
    rotation.z = constrainAngle(rotation.z + dz);
}

void Entity::RotateBy(const glm::vec3& dv)
{
    rotation = Maths::constrainAngleVector(rotation + dv);
}
