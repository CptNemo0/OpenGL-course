#pragma once

#include "Mesh.hpp"
#include "glm/glm.hpp"
#include "structs.hpp"
#include <memory>

class Object
{
private:
	std::shared_ptr<Mesh> mesh;
	//Shader* shader;
	Texture* texture;

public:
	Transform transform;

	std::list<std::shared_ptr<Object>> children;
	Object* parent = nullptr;

	Object();
	Object(std::shared_ptr<Mesh> new_mesh, unsigned int id);
	void RenderReflective();
	void Render() const;

	template<typename... TArgs>
	void AddChild(const TArgs&... args);
	void AddChild(std::shared_ptr<Object> obj);
	void UpdateSelfAndChildren();
	glm::mat4 GetMM();
	void SetPosition(glm::vec3 new_position);
	void SetScale(glm::vec3 new_scale);
	void SetRotation(glm::vec3 new_rotation);
	
};

template<typename ...TArgs>
inline void Object::AddChild(const TArgs & ...args)
{
	children.emplace_back(std::make_shared<Object>(args...));
	children.back()->parent = this;
}