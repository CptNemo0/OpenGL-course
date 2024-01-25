#version 330 core
out vec4 reflect_frag_color;

in vec3 out_reflect_normal;
in vec3 out_reflect_position;

uniform vec3 reflect_camera_position;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(out_reflect_position - reflect_camera_position);
    vec3 R = reflect(I, normalize(out_reflect_normal));
    reflect_frag_color = vec4(texture(skybox, R).rgb, 1.0);
}