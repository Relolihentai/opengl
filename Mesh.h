#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

#include "Shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 texcoord;
};
struct Texture
{
    unsigned int id;
    std::string type;
    aiString path;
};
class Mesh
{
    public :
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader shader);
    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};
#endif