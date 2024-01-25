#include "Object.hpp"

Object::Object()
{
	transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.rotationEuler = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Object::Object(std::shared_ptr<Mesh> new_mesh, unsigned int id)
{
	transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.rotationEuler = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->mesh = new_mesh;
	this->texture = new Texture(id);
}

void Object::RenderReflective()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
	mesh->DrawCubemap();
}

void Object::Render() const
{
	glBindTexture(GL_TEXTURE_2D, texture->id);
	mesh->Draw();
}

void Object::AddChild(std::shared_ptr<Object> obj)
{
	children.emplace_back(obj);
	children.back()->parent = this;
}

void Object::UpdateSelfAndChildren()
{
	if (parent)
	{
		transform.modelMatrix = parent->transform.modelMatrix * transform.getLocalModelMatrix();
	}
	else
	{
		transform.modelMatrix = transform.getLocalModelMatrix();
	}

	for (auto&& child : children)
	{
		child->UpdateSelfAndChildren();
	}
}

glm::mat4 Object::GetMM()
{
	if (parent != nullptr)
	{
		return parent->GetMM() * transform.getLocalModelMatrix();
	}
	else
	{
		return transform.getLocalModelMatrix();
	}
	
}

void Object::SetPosition(glm::vec3 new_position)
{
	this->transform.position = new_position;
	UpdateSelfAndChildren();
}

void Object::SetScale(glm::vec3 new_scale)
{
	this->transform.scale = new_scale;
	UpdateSelfAndChildren();
}

void Object::SetRotation(glm::vec3 new_rotation)
{
	this->transform.rotationEuler = new_rotation;
	UpdateSelfAndChildren();
}


