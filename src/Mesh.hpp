#pragma once

#include "structs.hpp"
#include <vector>
#include "glad/glad.h"
#include "Shader.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

constexpr float DTR = 3.14159265359 / 180.0;

class Mesh
{
private:
	Assimp::Importer importer;
	const aiScene* scene;
	const aiMesh* mesh;

	
	unsigned int VBO;
	unsigned int EBO;
	unsigned int indices_num;
	
	void Setup();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;
	Mesh();
	Mesh(std::string path);
	void Draw() const;
	void DrawCubemap() const;
};