#ifndef SHADERINFO_HPP_INCLUDED
#define SHADERINFO_HPP_INCLUDED

struct ShaderInfo
{
    ShaderInfo(const char* file, GLenum type) : file(file), type(type) { };
    const char* file;
    GLenum type;
};

#endif // SHADERINFO_HPP_INCLUDED
