#version 430 core
in vec2 Texture;

uniform sampler2D ourTexture;
uniform float idx;

out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, Texture + vec2(idx, 0.0f));   
}