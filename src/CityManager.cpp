#include "CityManager.hpp"

CityManager::CityManager(shared_ptr<Texture> gt, shared_ptr<Texture> ht, shared_ptr<Texture> rt, shared_ptr<Mesh> gm, shared_ptr<Mesh> hm, shared_ptr<Mesh> rm)
    :ground_texture(gt),
    house_texture(ht),
    roof_texture(rt),
    ground_mesh(gm),
    house_mesh(hm),
    roof_mesh(rm)
{
    this->grid_x_size = 200;
    this->grid_y_size = 200;
    this->ROOT = make_shared<Object>();

    unsigned int vec4Size = sizeof(glm::vec4);

    grounds_model_matrices = vector<glm::mat4>();
    houses_model_matrices = vector<glm::mat4>();
    roofs_model_matrices = vector<glm::mat4>();
}

void CityManager::PopulateVectors()
{
    for (int i = 0; i < grid_x_size; i++)
    {
        for (int j = 0; j < grid_y_size; j++)
        {
            auto ground_obj = make_shared<Object>();
            ROOT->AddChild(ground_obj);
            grounds.push_back(ground_obj);

            auto house_obj = std::make_shared<Object>();
            ground_obj->AddChild(house_obj);
            houses.push_back(house_obj);

            auto roof_obj = std::make_shared<Object>();
            house_obj->AddChild(roof_obj);
            roofs.push_back(roof_obj);

            ground_obj->transform.position = glm::vec3(i * 4, 0.0f, j * 8);

            house_obj->UpdateSelfAndChildren();
            ground_obj->UpdateSelfAndChildren();
        }
    }

    ROOT->UpdateSelfAndChildren();
}

void CityManager::GetModelMatrices()
{
    grounds_model_matrices.clear();
    houses_model_matrices.clear();
    roofs_model_matrices.clear();

    if (grounds.size() == houses.size() && houses.size() == roofs.size())
    {
        spdlog::info("Getting model matrices");

        for (int i = 0; i < grounds.size(); i++)
        {
            this->grounds_model_matrices.push_back(grounds[i]->transform.modelMatrix);
            this->houses_model_matrices.push_back(houses[i]->transform.modelMatrix);
            this->roofs_model_matrices.push_back(roofs[i]->transform.modelMatrix);
        }
    }
}

void CityManager::CreateBuffers()
{
    
    std::size_t vec4Size = sizeof(glm::vec4);
    
    glGenBuffers(1, &VBO_grounds);
    glGenBuffers(1, &VBO_houses);
    glGenBuffers(1, &VBO_roofs);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_grounds);
    glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &grounds_model_matrices[0], GL_DYNAMIC_DRAW);
    unsigned int VAO_grounds = ground_mesh->VAO;
    glBindVertexArray(VAO_grounds);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO_houses);
    glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &houses_model_matrices[0], GL_DYNAMIC_DRAW);
    unsigned int VAO_houses = house_mesh->VAO;
    glBindVertexArray(VAO_houses);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_roofs);
    glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &roofs_model_matrices[0], GL_DYNAMIC_DRAW);
    unsigned int VAO_roof = roof_mesh->VAO;
    glBindVertexArray(VAO_roof);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1); // 1 oznacza ze updateujemy te wartosc co 1 instancje obiektu
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CityManager::DrawCity()
{
    glBindTexture(GL_TEXTURE_2D, ground_texture->id);
    glBindVertexArray(ground_mesh->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(ground_mesh->indices.size()), GL_UNSIGNED_INT, 0, grid_x_size * grid_y_size);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, house_texture->id);
    glBindVertexArray(house_mesh->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(house_mesh->indices.size()), GL_UNSIGNED_INT, 0, grid_x_size * grid_y_size);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, roof_texture->id);
    glBindVertexArray(roof_mesh->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(roof_mesh->indices.size()), GL_UNSIGNED_INT, 0, grid_x_size * grid_y_size);
    glBindVertexArray(0);
}

void CityManager::ChangeTransform(int id, TYPES type, TRANSFORM_TYPES t_type, glm::vec3 new_value, bool apply)
{
    switch (type)
    {
    case TYPES::ROOT:
        switch (t_type)
        {
        case TRANSFORM_TYPES::POSITION:
            ROOT->SetPosition(new_value);
            break;
        case TRANSFORM_TYPES::ROTATION:
            ROOT->SetRotation(new_value);
            break;
        case TRANSFORM_TYPES::SCALE:
            ROOT->SetScale(new_value);
            break;
        }

        if (grounds.size() == houses.size() && houses.size() == roofs.size())
        {
            spdlog::info("Getting model matrices");

            for (int i = 0; i < grounds.size(); i++)
            {
                this->grounds_model_matrices[i] = grounds[i]->transform.modelMatrix;
                this->houses_model_matrices[i] = houses[i]->transform.modelMatrix;
                this->roofs_model_matrices[i] =roofs[i]->transform.modelMatrix;
            }
        }

        if (apply)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_grounds);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &grounds_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_houses);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &houses_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_roofs);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &roofs_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        

        break;

    case TYPES::GROUND:

        switch (t_type)
        {
        case TRANSFORM_TYPES::POSITION:
            grounds[id]->SetPosition(new_value);
            break;
        case TRANSFORM_TYPES::ROTATION:
            grounds[id]->SetRotation(new_value);
            break;
        case TRANSFORM_TYPES::SCALE:
            grounds[id]->SetScale(new_value);
            break;
        }
        this->grounds_model_matrices[id] = grounds[id]->transform.modelMatrix;
        this->houses_model_matrices[id] = houses[id]->transform.modelMatrix;
        this->roofs_model_matrices[id] = roofs[id]->transform.modelMatrix;

        if (apply)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_grounds);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &grounds_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_houses);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &houses_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_roofs);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &roofs_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        break;

    case TYPES::HOUSE:

        switch (t_type)
        {
        case TRANSFORM_TYPES::POSITION:
            houses[id]->SetPosition(new_value);
            break;
        case TRANSFORM_TYPES::ROTATION:
            houses[id]->SetRotation(new_value);
            break;
        case TRANSFORM_TYPES::SCALE:
            houses[id]->SetScale(new_value);
            break;
        }

        
        this->houses_model_matrices[id] = houses[id]->transform.modelMatrix;
        this->roofs_model_matrices[id] = roofs[id]->transform.modelMatrix;

        if(apply)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_houses);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &houses_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_roofs);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &roofs_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        break;
    case TYPES::ROOF:

        switch (t_type) 
        {
        case TRANSFORM_TYPES::POSITION:
            roofs[id]->SetPosition(new_value);
            break;
        case TRANSFORM_TYPES::ROTATION:
            roofs[id]->SetRotation(new_value);
            break;
        case TRANSFORM_TYPES::SCALE:
            roofs[id]->SetScale(new_value);
            break;
        }

        this->roofs_model_matrices[id] = roofs[id]->transform.modelMatrix;

        if(apply)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_roofs);
            glBufferData(GL_ARRAY_BUFFER, grid_x_size * grid_y_size * sizeof(glm::mat4), &roofs_model_matrices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        break;
    }
}


