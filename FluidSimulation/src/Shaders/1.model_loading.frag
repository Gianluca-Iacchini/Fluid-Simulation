#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool drawTexture;
uniform vec4 variantColor;


void main()
{    
    vec4 result;

    if (drawTexture)
    {
        vec4 tex2D = texture(texture_diffuse1, TexCoords);
        result = tex2D + vec4(1.0 - tex2D.a) * variantColor;
    }
    else
    {
        result = variantColor;
    }
    FragColor = result;
}