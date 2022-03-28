#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "GL/glew.h"
#include <glm/glm.hpp>
#include "ShaderInfo.hpp"
#include <initializer_list>

class ShaderProgram
{
    public:
        void Init(std::initializer_list<ShaderInfo> shaderInfos);
        void Start();
        void Stop();
        void CleanUp();
        virtual void loadProjectionMatrix(const glm::mat4& mat);
        GLuint getProgramID() const;
        void loadFloat(GLint location, float value);

    protected:
        virtual void bindAttributes();
        virtual void getAllUniformLocations();
        void bindAttribute(GLuint attribute, const char* variableName);

        void loadVector(GLint location, glm::vec3 vector);
        void loadBoolean(GLint location, bool value);
        void loadMatrix(GLint location, glm::mat4 value);
        GLint getUniformLocation(const char* uniformName);
        GLuint programID, vertexShaderID, fragmentShaderID;
    private:


        void inh_bindAttributes();
        void inh_getAllUniformLocations();

    static GLuint loadShader(const char* file, GLenum type);

};

#endif // SHADERPROGRAM_HPP
