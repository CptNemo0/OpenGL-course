#version 330 core
layout (location = 0) in vec3 reflect_pos;
layout (location = 1) in vec3 reflect_normal;

out vec3 out_reflect_normal;
out vec3 out_reflect_position;

uniform mat4 reflect_model;
uniform mat4 reflect_view;
uniform mat4 reflect_projection;

void main()
{
    out_reflect_normal = mat3(transpose(inverse(reflect_model))) * reflect_normal;
    out_reflect_position = vec3(reflect_model * vec4(reflect_pos, 1.0));
    gl_Position = reflect_projection * reflect_view * vec4(out_reflect_position, 1.0);
}  