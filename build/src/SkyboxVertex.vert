#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 skybox_projection_matrix;
uniform mat4 skybox_view_matrix;

void main()
{
    TexCoords = aPos;
    gl_Position = skybox_projection_matrix * skybox_view_matrix * vec4(aPos, 1.0);
    
}