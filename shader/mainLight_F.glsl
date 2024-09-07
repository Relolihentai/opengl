#version 450 core
out vec4 finalColor;
uniform vec3 lightColor;
void main()
{
    finalColor = vec4(lightColor, 1);
}