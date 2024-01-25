#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "spdlog/spdlog.h"

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include "stb_image.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Shader.hpp"

static int InitGLFW(GLFWwindow*& window, GLFWmonitor*& monitor, GLFWvidmode*& mode, const char* window_name);
static int InitGlad();
static int InitMyGUI(GLFWwindow*& window);

static int GetTexturePathsFromFile(const std::string path, std::vector<std::string>& textures_paths);
static unsigned int LoadSingleTexture(const std::string filePath);
static int LoadTextures(std::vector<unsigned int>& textures, const std::vector<std::string>& textures_paths);
static void FixCoordinates(Shader* shader);
static void CalculateMousePosition(double& x, double& y, GLFWvidmode*& mode);
#endif // !UTILITY_HPP




