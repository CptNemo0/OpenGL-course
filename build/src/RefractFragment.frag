#version 330 core
out vec4 refract_frag_color;

in vec3 out_refract_normal;
in vec3 out_refract_position;

uniform vec3 refract_camera_position;
uniform samplerCube skybox;
uniform float refraction_ratio;

void main()
{
    float ratio = 0.90f;
    vec3 I = normalize(out_refract_position - refract_camera_position);
    vec3 R = refract(I, normalize(out_refract_normal), ratio);
    refract_frag_color = vec4(texture(skybox, R).rgb, 1.0);
}