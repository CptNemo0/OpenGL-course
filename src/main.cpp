#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <chrono>
#include <cmath>

#include "CityManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include "Mesh.hpp"
#include <iostream>
#include "Shader.hpp"
#include "Skybox.hpp"
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <stb_image.h>
#include <glm/gtx/rotate_vector.hpp> 

#include "utility.cpp"
#include "Object.hpp"

#ifndef MY_PI
#define MY_PI 3.141592653589793238463
#endif

#ifndef CAMERA_NORMAL_SPEED
#define CAMERA_NORMAL_SPEED 12.5f
#endif // !CAMERA_NORMAL_SPEED

#ifndef CAMERA_FAST_SPEED
#define CAMERA_FAST_SPEED 25.0f
#endif // !CAMERA_FAST_SPEED

constexpr float dtr = MY_PI / 180.0;

using namespace std;
using namespace ImGui;

int main()
{
    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = nullptr;
    GLFWvidmode* mode = nullptr;

    InitGLFW(window, monitor, mode, "Houses");
    InitGlad();
    InitMyGUI(window);

    auto basic_shader = std::make_shared<Shader>("BasicVertexShader.vert", "BasicFragmentShader.frag");
    auto instance_shader = std::make_shared<Shader>("VertexShader.vert", "FragmentShader.frag");
    auto skybox_shader = std::make_shared<Shader>("SkyboxVertex.vert", "SkyboxFragment.frag");
    auto reflect_shader = std::make_shared<Shader>("ReflectVertex.vert", "ReflectFragment.frag");
    auto refract_shader = std::make_shared<Shader>("RefractVertex.vert", "RefractFragment.frag");
    
    auto empty_mesh = std::make_shared<Mesh>("empty.obj");
    auto ground_mesh = std::make_shared<Mesh>("ground_mesh.obj");
    auto house_mesh = std::make_shared<Mesh>("house_mesh.obj");
    auto roof_mesh = std::make_shared<Mesh>("roof_mesh.obj");
    auto copule_mesh = std::make_shared<Mesh>("copule.obj");
    auto tank_turret_mesh = std::make_shared<Mesh>("turret.obj");
    auto tank_base_mesh = std::make_shared<Mesh>("tank_base.obj");
    auto left_thread_mesh = std::make_shared<Mesh>("thread_l_mesh.obj");
    auto right_thread_mesh = std::make_shared<Mesh>("thraed_r_mesh.obj");
    
    auto ground_texture = std::make_shared<Texture>("sand_texture.jpg");
    auto house_texture = std::make_shared<Texture>("white_texture.jpg");
    auto roof_texture = std::make_shared<Texture>("roof_texture.jpg");
    auto copule_texture = std::make_shared<Texture>("gold_texture.jpg");
    auto thread_texture = std::make_shared<Texture>("threads_texture.jpg");

    vector<string> skybox_textures
    {
        "px.png",
        "nx.png",
        "py.png",
        "ny.png",
        "pz.png",
        "nz.png"
    };

    CityManager city_manager = CityManager(ground_texture,
                                           house_texture,
                                           copule_texture,
                                           ground_mesh,
                                           house_mesh,
                                           copule_mesh);

    city_manager.PopulateVectors(); 
    
    

    auto skybox = std::make_shared<Skybox>(skybox_textures, skybox_shader);
    unsigned int skybox_texture_id = skybox->Load();

    auto tank_base = std::make_shared<Object>(tank_base_mesh, skybox_texture_id);
    tank_base->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    city_manager.ROOT->AddChild(tank_base);
    
    auto thread_r = std::make_shared<Object>(right_thread_mesh, thread_texture->id);
    tank_base->AddChild(thread_r);

    auto thread_l = std::make_shared<Object>(left_thread_mesh, thread_texture->id);
    tank_base->AddChild(thread_l);

    auto tank_turret = std::make_shared<Object>(tank_turret_mesh, skybox_texture_id);
    tank_turret->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    tank_base->AddChild(tank_turret);

    auto camera_container = std::make_shared<Object>(tank_base_mesh, skybox_texture_id);
    camera_container->SetPosition(glm::vec3(-5.0f, 3.5f, 0.0f));
    tank_turret->AddChild(camera_container);
    
    tank_base->SetScale(glm::vec3(0.25f, 0.25f, 0.25f));

    city_manager.ROOT->UpdateSelfAndChildren();

    city_manager.GetModelMatrices();
    city_manager.CreateBuffers();

    float delta_time = 0.0f;
    float t_0 = 0.0f;
    float t_1 = 0.16f;
    float idx = 0;

    glm::vec3 tank_front_vector{ 0.0f, 0.0f, 0.0f };

    glm::vec3 camera_position = glm::vec3(-5.0f, 10.0f, -5.0f);
    glm::vec3 camera_front = glm::vec3(1.0f, -1.0f, 1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::vec3(1.0f, 0.0f, 0.0f);

    float camera_speed = CAMERA_NORMAL_SPEED;
    float yaw = 0;
    float pitch = 0;
    float camera_radius = 4.0f;
    float camera_height = 4.0f;

    DirectionalLight directional_light;
    directional_light.direction = glm::vec3(-0.0f, 1.0f, 0.5f);
    directional_light.diffuse_color = glm::vec3(0.95f, 1.0f, 0.9f);
    directional_light.specular_color = glm::vec3(1.0f, 1.0f, 1.0f);
    directional_light.intensity = 1.0f;

    PointLight point_light;
    point_light.position = glm::vec3(0.0f, 10.0f, 0.0f);
    point_light.diffuse_color = glm::vec3(1.0f, 0.0f, 0.0f);
    point_light.specular_color = glm::vec3(1.0f, 0.5f, 0.5f);
    point_light.intensity = 400.0f;

    SpotLight spot_light_1;
    spot_light_1.position = glm::vec3(10.0f, 10.0f, 10.0f);
    spot_light_1.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spot_light_1.diffuse_color = glm::vec3(1.0f, 1.0f, 1.0f);
    spot_light_1.specular_color = glm::vec3(1.0f,1.0f, 1.0f);
    spot_light_1.intensity = 1.0f;
    spot_light_1.cut_off = 0.75f;

    SpotLight spot_light_2;
    spot_light_2.position = glm::vec3(50.0f, 10.0f, 50.0f);
    spot_light_2.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spot_light_2.diffuse_color = glm::vec3(1.0f, 1.0f, 1.0f);
    spot_light_2.specular_color = glm::vec3(1.0f, 1.0f, 1.0f);
    spot_light_2.intensity = 1.0f;
    spot_light_2.cut_off = 0.75f;

    auto skybox_view_matrix = glm::mat4(1.0f);
    auto skybox_projection_matrix = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        delta_time = t_1 - t_0;
        t_0 = (float)glfwGetTime();

        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT);         
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

#pragma region Camra
        glm::mat4 view_matrix = glm::mat4(1.0f);       // transformacja do view space

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        auto tmp_position_turret = glm::vec3(tank_turret->GetMM() * glm::vec4(tank_turret->transform.position, 1.0f));
        auto tmp_position_camera_container = glm::vec3(camera_container->GetMM() * glm::vec4(camera_container->transform.position, 1.0f));

        auto front = tmp_position_turret - tmp_position_camera_container;
        front = glm::normalize(front);

        view_matrix = glm::lookAt(tmp_position_camera_container, tmp_position_camera_container + front, camera_up);
        glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), (1920.0f / 1080.0f), 0.1f, 1000.0f);
        
        point_light.position = glm::vec3(100.0f + sin(idx * 0.01f) * 100.0f, 20.0f, 100.0f + cos(idx * 0.01f) * 100.0f);

#pragma endregion

        skybox->Render(glm::mat4(glm::mat3(view_matrix)), projection_matrix);

        glUseProgram(0);

        refract_shader->use();
        refract_shader->setMatrix4("refract_model", tank_base->GetMM());
        refract_shader->setMatrix4("refract_view", view_matrix);
        refract_shader->setMatrix4("refract_projection", projection_matrix);
        refract_shader->setVec3("refract_camera_position", camera_position);
        tank_base->RenderReflective();

        glUseProgram(0);

        reflect_shader->use();
        reflect_shader->setMatrix4("reflect_model", tank_turret->GetMM());
        reflect_shader->setMatrix4("reflect_view", view_matrix);
        reflect_shader->setMatrix4("reflect_projection", projection_matrix);
        reflect_shader->setVec3("reflect_camera_position", camera_position);
        tank_turret->RenderReflective();

        glUseProgram(0);

        basic_shader->use();
        basic_shader->setFloat("idx", idx);
        basic_shader->setMatrix4("view_matrix", view_matrix);
        basic_shader->setMatrix4("projection_matrix", projection_matrix);

        basic_shader->setMatrix4("model_matrix", thread_r->GetMM());
        thread_r->Render();

        basic_shader->setMatrix4("model_matrix", thread_l->GetMM());
        thread_l->Render();

        glUseProgram(0);

        instance_shader->use();
        instance_shader->setMatrix4("view_matrix", view_matrix);
        instance_shader->setMatrix4("projection_matrix", projection_matrix);
        instance_shader->setDirectionalLight("directional_light", directional_light);
        instance_shader->setPointLight("point_light", point_light);
        instance_shader->setSpotLight("spot_light_1", spot_light_1);
        instance_shader->setSpotLight("spot_light_2", spot_light_2);
        instance_shader->setVec3("camera_position", camera_position);
        city_manager.DrawCity();

        glUseProgram(0);
#pragma region Keyboard controll
        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            glfwSetWindowShouldClose(window, 1);
        }

        if (glfwGetKey(window, GLFW_KEY_P))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

#pragma region Camera Controll

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            tank_base->SetPosition(tank_base->transform.position + tank_front_vector * 0.25f);
            idx += 0.01f;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            tank_base->SetPosition(tank_base->transform.position - tank_front_vector * 0.25f);
            idx -= 0.01f;
        }

        if (glfwGetKey(window, GLFW_KEY_A))
        {
            yaw = yaw + 1.0f;
            tank_base->SetRotation(glm::vec3(0.0f, yaw, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
            yaw = yaw - 1.0f;
            tank_base->SetRotation(glm::vec3(0.0f, yaw, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT))
        {
            glm::vec3 new_rot = tank_turret->transform.rotationEuler + glm::vec3(0.0f, 1.0f, 0.0f);
            tank_turret->SetRotation(new_rot);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            glm::vec3 new_rot = tank_turret->transform.rotationEuler + glm::vec3(0.0f, -1.0f, 0.0f);
            tank_turret->SetRotation(new_rot);
        }
        
        tank_front_vector.x = cos(glm::radians(yaw)) * 1.0f;
        tank_front_vector.y = sin(glm::radians(0.0f));
        tank_front_vector.z = sin(glm::radians(yaw)) * -1.0f;
        
        camera_position = glm::vec3(camera_container->GetMM() * glm::vec4(camera_container->transform.position, 1.0f));
#pragma endregion

#pragma endregion
        
        
        if (idx > 0.5f)
        {
            idx = 0.0f;
        }

        if (idx < -0.5f)
        {
            idx = 0.0f;
        }
        
        t_1 = (float)glfwGetTime();

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    spdlog::info("Exiting program. Goodbye!");
    return 0;
}

