#version 330 core
layout (location = 0) in vec3 refract_pos;
layout (location = 1) in vec3 refract_normal;

out vec3 out_refract_normal;
out vec3 out_refract_position;

uniform mat4 refract_model;
uniform mat4 refract_view;
uniform mat4 refract_projection;

void main()
{
    out_refract_normal = mat3(transpose(inverse(refract_model))) * refract_normal;
    out_refract_position = vec3(refract_model * vec4(refract_pos, 1.0));
    gl_Position = refract_projection * refract_view * vec4(out_refract_position, 1.0);
}  