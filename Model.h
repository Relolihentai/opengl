#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include "Mesh.h"
#include "Shader.h"

class Model 
{
    public:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;
        Model(char *path);
        void Draw(Shader &shader);
    private:
        void loadModel(std::string const &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
};
#endif