#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in mat4 instanceMatrix;

out vec3 fragment_position;
out vec3 normal_vec;
out vec2 texture_position;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_position;

void main()
{
	vec4 mm = instanceMatrix * vec4(aPos, 1.0f);
	gl_Position = projection_matrix * view_matrix * mm;
	fragment_position = vec3(mm);
	normal_vec = normalize(vec3(transpose(inverse(instanceMatrix)) * vec4(aNormal, 1.0)));
	texture_position = aTex;
}