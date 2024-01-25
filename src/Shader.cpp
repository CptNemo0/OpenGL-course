#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmtPath)
{
    // Zmienne z kodem zrodlowym shaderow
    std::string v;
    std::string f;

    std::ifstream vertexReader;
    std::ifstream fragmtReader;

    try
    {
        std::stringstream vertexStream;
        std::stringstream fragmtStream;

        // Otworzenie plikow
        vertexReader.open(vertexPath);
        fragmtReader.open(fragmtPath);

        // Odczyt
        vertexStream << vertexReader.rdbuf();
        fragmtStream << fragmtReader.rdbuf();

        vertexReader.close();
        fragmtReader.close();

        v = vertexStream.str();
        f = fragmtStream.str();
    }
    catch (std::fstream::failure e)
    {
        spdlog::error("You have f'ed up!");
    }

    const char* vertexSource = v.c_str();
    const char* fragmtSource = f.c_str();

    unsigned int vertex;
    unsigned int fragmt;

    int success;
    char* infoLog = new char[512];

    // Kompilacja shaderow
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        spdlog::error("Vertex shader compilation failed");
        spdlog::error(infoLog);
        exit(1);
    }
    else
    {
        spdlog::info("Vertex shader compiled");
    }

    // Fragment shader
    fragmt = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmt, 1, &fragmtSource, NULL);
    glCompileShader(fragmt);
    // check for shader compile errors
    glGetShaderiv(fragmt, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmt, 512, NULL, infoLog);
        spdlog::error("Fragment shader compilation failed");
        spdlog::error(infoLog);
        exit(1);
    }
    else
    {
        spdlog::info("Fragment shader compiled");
    }

    // Stworzenie programu wykonujacego shadery
    ID = glCreateProgram();     // Tworzenie programy shaderow
    glAttachShader(ID, vertex); // Przyczepienie vertex shaderu do dzielonego programu
    glAttachShader(ID, fragmt); // Przyczepienie fragment shaderu do dzielonego programu
    glLinkProgram(ID);          // Utworzenie ostatecznego obiektu programu polaczonego z shaderami

    glDeleteShader(vertex);
    glDeleteShader(fragmt);
}
Shader::Shader() {}
void Shader::use()
{
    glUseProgram(ID); // Wykonanie programu o id ID
}

void Shader::end()
{
    glDeleteProgram(ID);
}

unsigned int Shader::GetID()
{
    return ID;
}

// Funkcje ustalajace uniformy - zmienne dzielone miedzy shaderami, ktore moga byc ustawiane "na biegu"
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMatrix4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
    
}

void Shader::setDirectionalLight(const std::string& name, DirectionalLight value)
{
    std::string direction_name = name + ".direction";
    std::string diffuse_color_name = name + ".diffuse_color";
    std::string specular_color_name = name + ".specular_color";
    std::string intensity_name = name + ".intensity";

    setVec3(direction_name, value.direction);
    setVec3(diffuse_color_name, value.diffuse_color);
    setVec3(specular_color_name, value.specular_color);
    setFloat(intensity_name, value.intensity);
}

void Shader::setPointLight(const std::string& name, PointLight value)
{
    std::string position_name = name + ".position";
    std::string diffuse_color_name = name + ".diffuse_color";
    std::string specular_color_name = name + ".specular_color";
    std::string intensity_name = name + ".intensity";

    setVec3(position_name, value.position);
    setVec3(diffuse_color_name, value.diffuse_color);
    setVec3(specular_color_name, value.specular_color);
    setFloat(intensity_name, value.intensity);
}

void Shader::setSpotLight(const std::string& name, SpotLight value)
{
    std::string position_name = name + ".position";
    std::string direction_name = name + ".direction";
    std::string diffuse_color_name = name + ".diffuse_color";
    std::string specular_color_name = name + ".specular_color";
    std::string intensity_name = name + ".intensity";
    std::string cut_off_name = name + ".cut_off";

    setVec3(position_name, value.position);
    setVec3(direction_name, value.direction);
    setVec3(diffuse_color_name, value.diffuse_color);
    setVec3(specular_color_name, value.specular_color);
    setFloat(intensity_name, value.intensity);
    setFloat(cut_off_name, value.cut_off);
}
