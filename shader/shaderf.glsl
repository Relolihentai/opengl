#version 450 core

in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D texture1;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    //context
    vec3 lightDir = normalize(lightPos - WorldPos);
    vec3 worldNormal = normalize(Normal);
    float lambert = max(dot(lightDir, worldNormal), 0);

    vec4 baseColor = vec4(lightColor * objectColor, 1);
    FragColor = texture(texture1, TexCoord) * baseColor;
    FragColor = vec4(lambert, lambert, lambert, 1);
}