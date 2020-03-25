#version 300 es
layout (location = 0) in vec2 lPos;
layout (location = 1) in vec2 lTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = lTexCoords;
    gl_Position = vec4(lPos, 0.0, 1.0); 
}  