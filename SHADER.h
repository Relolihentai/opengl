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
    //程序ID
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //从文件路径获取shader

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        //failbit系统级错误  badbit可恢复错误
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            //读取文件

            std::stringstream  vShaderStream, fShaderStream;
            //两个ss对象
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //将ifstream流对象，也就是文件对象的内容输出到ss流对象中


            vShaderFile.close();
            fShaderFile.close();
            //关闭文件

            vertexCode  = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            //将流对象转换到string类型


        }
        catch (std::ifstream::failure e)
        {
            printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        //将string转化为c字符串

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED:%s\n", infoLog);
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:%s\n", infoLog);
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED:%s\n", infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
    void use()
    {
        glUseProgram(ID);
    }
    //激活程序

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec2(const std::string &name, float v1, float v2) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
    }
    void setVec2(const std::string &name, glm::vec2 value) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
    }
    void setVec3(const std::string &name, float v1, float v2, float v3) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
    }
    void setVec3(const std::string &name, glm::vec3 value) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
    }
    void setVec4(const std::string &name, float v1, float v2, float v3, float v4) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
    }
    void setVec4(const std::string &name, glm::vec4 value) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
    }
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif


