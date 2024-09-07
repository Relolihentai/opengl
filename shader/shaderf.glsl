#version 450 core

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
};
struct PointLight
{
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    sampler2D albedoTex;
    sampler2D metallic_roughnessTex;
    sampler2D emission_aoTex;
    vec3 baseColor;
};

in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform Material material;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    //context
    vec3 worldNormal = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 lightColor = directionalLight.color;
    vec3 viewDir = normalize(cameraPos - WorldPos);
    vec3 reflectDir = normalize(reflect(-lightDir, worldNormal));

    //Lighting
    float lambert = max(dot(lightDir, worldNormal), 0);
    float phong = pow(max(dot(viewDir, reflectDir), 0), 32);

    vec4 mainTex = texture(material.albedoTex, TexCoord);
    vec3 mainTexColor = mainTex.xyz;
    vec3 diffuse = vec3(lambert, lambert, lambert);
    vec3 specular = phong * lightColor;

    FragColor = vec4((diffuse + specular) * material.baseColor * mainTexColor, 1) * 0;

    //additional Light Context
    vec3 addLightDir = normalize(pointLight.position - WorldPos);
    vec3 addLightColor = pointLight.color;
    vec3 addReflectDir = normalize(reflect(-addLightDir, worldNormal));
    float distance = length(pointLight.position - WorldPos);
    float addLightAttenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + 
                                    pointLight.quadratic * (distance * distance));
    
    //additional Lighting
    float addLambert = max(dot(addLightDir, worldNormal), 0);
    float addPhong = pow(max(dot(viewDir, addReflectDir), 0), 32);
    vec3 addDiffuse = vec3(addLambert, addLambert, addLambert);
    vec3 addSpecular = addPhong * addLightColor;
    FragColor += vec4((addDiffuse + addSpecular) * material.baseColor * mainTexColor * addLightAttenuation, 1);
}