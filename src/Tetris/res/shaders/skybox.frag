#version 300 es
precision highp float;

out vec4 fragColor;

in vec3 texCoord;

uniform samplerCube albedoMap;

void main()
{    
    fragColor = texture(albedoMap, texCoord);
}