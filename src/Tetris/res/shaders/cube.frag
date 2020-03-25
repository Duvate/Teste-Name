#version 300 es
precision highp float;

out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 viewPos;
uniform sampler2D tex;
uniform int texIndex;
uniform float alpha;

//hardcoded
const float shininess = 16.0f;
const float maxTextures = 8.0f;

void main()
{
	fragColor = texture(tex, vec2((texCoord.x + float(texIndex)) / maxTextures, texCoord.y));
	fragColor.a = alpha;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-viewDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec4 specularVector = vec4(vec3(fragColor) * spec, 0.0f);
	fragColor += specularVector;
}