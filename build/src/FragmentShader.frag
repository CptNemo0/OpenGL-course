#version 430 core
#extension GL_EXT_gpu_shader4 : enable

in vec3 fragment_position;
in vec3 normal_vec;
in vec2 texture_position;

out vec4 fragment_color;

#define SPOT_LIGHTS_NUM 2

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuse_color;
    vec3 specular_color;
    float intensity;
};

struct PointLight
{
    vec3 position;
    vec3 diffuse_color;
    vec3 specular_color;
    float intensity;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 diffuse_color;
    vec3 specular_color;
    float cut_off;
    float intensity;
};

uniform sampler2D ourTexture;

uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform SpotLight spot_light_1;
uniform SpotLight spot_light_2;

uniform vec3 camera_position;

uniform float shininess = 200.0;
uniform float gamma = 2.2;

float GetDistanceFactor(float distance, float intensity)
{
    return (intensity / (distance + 1));
}

vec3 DirectionalLightColor(DirectionalLight light, vec3 view_direction)
{
    light.direction = normalize(light.direction);

    //diffuse - lambert
    float diffuse = max(dot(light.direction, normal_vec), 0.0);

    float specular = 0.0f;
    if(diffuse != 0)
    {
        //specualr - bp
        vec3 half_direction = normalize(light.direction + view_direction);
        float cosine = max(dot(half_direction, normal_vec), 0.0);
        specular = pow(cosine, shininess);
    }
    
    return light.diffuse_color * diffuse * light.intensity+ light.specular_color * specular * light.intensity;
}

vec3 PointLightColor(PointLight light, vec3 view_direction)
{
    vec3 light_direction = light.position - fragment_position;
    vec3 light_direction_normalized = normalize(light_direction);

    float distance = length(light_direction);
    distance = distance * distance;
    float distance_factor = GetDistanceFactor(distance, light.intensity);

    //diffuse - lambert
    float diffuse = max(dot(light_direction_normalized, normal_vec), 0.0);

    float specular = 0.0f;
    if(diffuse != 0)
    {
        //specualr - bp
        vec3 half_direction = normalize(light_direction + view_direction);
        float cosine = max(dot(half_direction, normal_vec), 0.0);
        specular = pow(cosine, shininess);
    }
    
    return light.diffuse_color * diffuse * distance_factor + light.specular_color * specular * distance_factor;
}

vec3 SpotLightColor(SpotLight light, vec3 view_direction)
{
    vec3 light_direction = light.position - fragment_position;
    vec3 light_direction_normalized = normalize(light_direction);

    float diffuse = 0.0f;
    float specular = 0.0f;

    float theta = dot(light_direction_normalized, normalize(-light.direction));

    if(theta > light.cut_off)
    {
        //diffuse - lambert
        diffuse = max(dot(light_direction_normalized, normal_vec), 0.0);

        if(diffuse != 0)
        {
            //specular - bp
            vec3 half_direction = normalize(light_direction + view_direction);
            float cosine = max(dot(half_direction, normal_vec), 0.0);
            specular = pow(cosine, shininess);
        }
    }

    return light.diffuse_color * diffuse * light.intensity+ light.specular_color * specular * light.intensity;
}

void main()
{   
    vec3 view_direction = normalize(camera_position - fragment_position);

    vec3 color = vec3(0.0f, 0.0f, 0.0f);

    color += DirectionalLightColor(directional_light, view_direction);
    color += PointLightColor(point_light, view_direction);
    color += SpotLightColor(spot_light_1, view_direction);
    color += SpotLightColor(spot_light_2, view_direction);

    color = pow(color, vec3(1.0/gamma));

    fragment_color = vec4(color, 1.0f) * texture(ourTexture, texture_position);
}