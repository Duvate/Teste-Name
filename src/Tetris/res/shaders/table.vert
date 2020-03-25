#version 300 es
layout (location = 0) in vec3 lPos;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 offset;
/*
 * 0 = big ground square
 * 1 = small ground squares (does nothing special)
 * 2 = left / right squares
 * 3 = front / bottom squares
 */
uniform int type;

uniform int width;
uniform int depth;

void swap(inout float a, inout float b)
{
	float temp = a;
	a = b;
	b = temp;
}

void main()
{
	vec3 pos = lPos;
	if (type == 0) pos *= vec3(float(width), 1.0f, float(depth));
	else if (type == 2) swap(pos.x, pos.y);
	else if (type == 3) swap(pos.y, pos.z);
	pos += offset - vec3(0.5f, 0.5f, 0.5f); // hardcoded small square offset
	gl_Position = projection * view * vec4(pos, 1.0f);
}