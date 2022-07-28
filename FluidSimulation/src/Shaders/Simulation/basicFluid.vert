#version 410 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aCellCoords;



out int vInstance;

out vec2 TexCoords;
out vec2 CellCoords;

void main()
{
    vInstance = gl_InstanceID;
    TexCoords = aTexCoord;
    CellCoords = aCellCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
}