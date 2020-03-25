#version 300 es
layout (location = 0) in vec3 lPos;
layout (location = 1) in vec3 lNormal;
layout (location = 2) in vec2 lTexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	texCoord = lTexCoord;
	fragPos = vec3(model * vec4(lPos, 1.0f));
	normal = lNormal;

	gl_Position = projection * view * model * vec4(lPos, 1.0f);
}