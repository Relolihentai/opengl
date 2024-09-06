#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

class Texture2D{
public:
    int width, height, nrChannels;
    int ThisTextureUnit;
    Texture2D(const char* picturePath, int Wrapping, int Filtering, int MipmapFiltering, int loc) {

        stbi_set_flip_vertically_on_load(true);

        if (Wrapping == GL_REPEAT || Wrapping == GL_MIRRORED_REPEAT || Wrapping == GL_CLAMP_TO_EDGE || Wrapping == GL_CLAMP_TO_BORDER) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrapping);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrapping);
        }
        else
            std::cout << "ERROR: Parameter 'Wrapping' is not one of gl_texture_wrapping_parameter.\n";

        if (Filtering == GL_LINEAR || Filtering == GL_NEAREST)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filtering);
        else
            std::cout << "ERROR: Parameter 'Filtering' is not one of gl_texture_filtering_parameter.\n";
        if (MipmapFiltering == GL_LINEAR_MIPMAP_LINEAR || MipmapFiltering == GL_LINEAR_MIPMAP_NEAREST || MipmapFiltering == GL_NEAREST_MIPMAP_NEAREST || MipmapFiltering == GL_NEAREST_MIPMAP_LINEAR)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MipmapFiltering);
        else
            std::cout << "ERROR: Parameter 'MipmapFiltering' is not one of gl_texture_filtering_parameter.\n";

        unsigned char* data = stbi_load(picturePath, &width, &height, &nrChannels, 0);
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0 + loc);
        glBindTexture(GL_TEXTURE_2D, texture);

        if (data) {
            if (nrChannels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if (nrChannels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);

    }

};

#endif

//int Texture2D::TextureUnit = 0;