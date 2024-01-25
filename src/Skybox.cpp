#include "Skybox.hpp"

Skybox::Skybox(vector<string> new_faces_paths, shared_ptr<Shader> new_shader)
{
	this->faces_paths = new_faces_paths;
	this->shader = new_shader;
	this->id = -1;
	this->VBO = 0;
	this->VAO = 0;
}

unsigned int Skybox::Load()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(Vertex), &skybox_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	int w,  h,  c;
	for (int i = 0; i < faces_paths.size(); i++)
	{
		unsigned char* data = stbi_load(faces_paths[i].c_str(), &w, &h, &c, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			spdlog::error("CUBEMAP NOT LOADED!!!");
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	this->id = texture_id;
	return this->id;
}

void Skybox::Render(glm::mat4 vm, glm::mat4 pm)
{
	
	glDepthMask(GL_FALSE);
	shader->use();
	shader->setMatrix4("skybox_view_matrix", vm);
	shader->setMatrix4("skybox_projection_matrix", pm);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	
}
