#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "structs.hpp"

class Shader
{
    private:
        unsigned int ID;
        

    public:
        Shader();
        Shader(const char* vertexPath, const char* fragmtPath);
        
        void use();
        void end();

        unsigned int GetID();

        void setMatrix4(const std::string& name, glm::mat4 value) const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, glm::vec3 value) const;
        void setDirectionalLight(const std::string& name, DirectionalLight value);
        void setPointLight(const std::string& name, PointLight value);
        void setSpotLight(const std::string& name, SpotLight value);
};

#endif
