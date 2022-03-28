#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <RawModel.hpp>
#include <glm/glm.hpp>

class Entity
{
    public:
        Entity(const RawModel& model, const glm::vec3& position,
               const glm::vec3 rotation, const float scale);

        void MoveBy(const float dx, const float dy, const float dz);
        void MoveBy(const glm::vec3& dv);
        void RotateBy(const float dx, const float dy, const float dz);
        void RotateBy(const glm::vec3& dv);

        RawModel model;
        glm::vec3 position, rotation;
        float scale;
    protected:

    private:

};

#endif // ENTITY_HPP
