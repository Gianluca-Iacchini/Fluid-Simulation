#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCords;


out vec2 uv;


void main()
{
    uv = aTexCords;

    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
}