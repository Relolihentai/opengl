#version 450 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 WorldPos;
out vec3 Normal;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    WorldPos = vec3(model * vec4(aPos, 1));
    TexCoord = aTexCoord;
    Normal = aNormal;
}