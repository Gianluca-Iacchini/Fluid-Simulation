#version 410

in vec2 uv;
out vec4 Fragcolor;

uniform sampler2D colorTexture;

// Draws a texture to a screen wide quad.
void main() {
 
	Fragcolor = texture(colorTexture, uv);
}