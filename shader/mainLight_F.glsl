#version 450 core
in vec2 f_uv;
in vec3 f_normal;
in vec3 f_worldPos;
out vec4 finalColor;
uniform vec3 lightColor;
void main()
{
    finalColor = vec4(lightColor, 1);
}