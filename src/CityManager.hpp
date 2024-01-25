#pragma once

#include "Mesh.hpp"
#include "Object.hpp"
#include "structs.hpp"
#include <memory>

using namespace std;

enum class TYPES : unsigned int
{
	ROOT = 0,
	GROUND = 1,
	HOUSE = 2,
	ROOF = 3
};

enum class TRANSFORM_TYPES : unsigned int
{
	POSITION = 0,
	ROTATION = 1,
	SCALE = 2
};

class CityManager
{
private:
	shared_ptr<Texture> ground_texture;
	shared_ptr<Texture> house_texture;
	shared_ptr<Texture> roof_texture;

	shared_ptr<Mesh> ground_mesh;
	shared_ptr<Mesh> house_mesh;
	shared_ptr<Mesh> roof_mesh;

	

public:
	shared_ptr<Object> ROOT;

	int grid_x_size;
	int grid_y_size;

	unsigned int VBO_grounds;
	unsigned int VBO_houses;
	unsigned int VBO_roofs;

	vector<shared_ptr<Object>> grounds;
	vector<shared_ptr<Object>> houses;
	vector<shared_ptr<Object>> roofs;

	vector<glm::mat4> grounds_model_matrices;
	vector<glm::mat4> houses_model_matrices;
	vector<glm::mat4> roofs_model_matrices;

	CityManager(shared_ptr<Texture> gt, shared_ptr<Texture> ht, shared_ptr<Texture> rt, shared_ptr<Mesh> gm, shared_ptr<Mesh> hm, shared_ptr<Mesh> rm);
	void GetModelMatrices();
	void PopulateVectors();
	void CreateBuffers();
	void DrawCity();
	void ChangeTransform(int id, TYPES type, TRANSFORM_TYPES t_type, glm::vec3 new_value, bool apply);
};