#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, float v1, float v2) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec3(const std::string &name, float v1, float v2, float v3) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec4(const std::string &name, float v1, float v2, float v3, float v4) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif


