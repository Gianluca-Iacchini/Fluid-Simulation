#version 410 core
// Basic model drawing fragment shader from learnopengl.com
// https://learnopengl.com/Model-Loading/
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec4 variantColor;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords) * variantColor;
}