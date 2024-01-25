#pragma once
#include "glm/glm.hpp"
#include "string"
#include "glad/glad.h"
#include <stb_image.h>
#include "spdlog/spdlog.h"
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

struct Texture
{
	unsigned int id;

    Texture(unsigned int new_id)
    {
        this->id = new_id;
    }

    Texture(const std::string& path)
    {
        id = 0;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // ustawienie textur
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int t_width, t_height, t_c;
        unsigned char* img = stbi_load(path.c_str(), &t_width, &t_height, &t_c, 0);
        if (img)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
            glGenerateMipmap(GL_TEXTURE_2D);
            spdlog::info("Texture loaded!!");
        }
        else
        {
            
            spdlog::error("Error loading texture!");
        }
        stbi_image_free(img);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

struct Transform
{
    glm::vec3 position      { 0.0f, 0.0f, 0.0f }; 
    glm::vec3 rotationEuler { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale         { 1.0f, 1.0f, 1.0f };

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::mat4 getLocalModelMatrix()
    {
        const glm::mat4 scale_m     = glm::scale(glm::mat4(1.0f), scale);

        const glm::mat4 rotation_X  = glm::rotate(glm::mat4(1.0f), glm::radians(rotationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 rotation_Y  = glm::rotate(glm::mat4(1.0f), glm::radians(rotationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 rotation_Z  = glm::rotate(glm::mat4(1.0f), glm::radians(rotationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
        
        const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = translation * rotation_Y * rotation_X * rotation_Z * scale_m;
        
        return modelMatrix;
    }

};

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float intensity;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float intensity;
};

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float cut_off;
    float intensity;
};