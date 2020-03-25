#version 300 es
layout (location = 0) in vec3 lPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 texCoord;

void main()
{
    texCoord = lPos;

	mat4 viewTemp = mat4(mat3(view)); // remove translation
    vec4 pos = projection * viewTemp * vec4(lPos, 1.0);
    gl_Position = pos.xyww;
}  