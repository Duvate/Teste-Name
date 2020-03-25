#version 300 es
precision highp float;
out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int WINDOW_HEIGHT;
uniform int postProcessIndex;

void main()
{    
	if (postProcessIndex == 0)      fragColor = texture(screenTexture, TexCoords);						  // no postProcessIndex
	else if (postProcessIndex == 1) fragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0); // inversion
	else if (postProcessIndex == 2)																		  // grayscale
	{
		fragColor = texture(screenTexture, TexCoords);
		float average = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
		fragColor = vec4(vec3(average), 1.0f);	
	}
	else 
	{
		float offset = 2.0f / float(WINDOW_HEIGHT);
	
	    vec2 offsets[9] = vec2[](
	        vec2(-offset,  offset), // top-left
	        vec2( 0.0f,    offset), // top-center
	        vec2( offset,  offset), // top-right
	        vec2(-offset,  0.0f),   // center-left
	        vec2( 0.0f,    0.0f),   // center-center
	        vec2( offset,  0.0f),   // center-right
	        vec2(-offset, -offset), // bottom-left
	        vec2( 0.0f,   -offset), // bottom-center
	        vec2( offset, -offset)  // bottom-right    
	    );
	
		float kernel[9];
		if (postProcessIndex == 3) // blur
			kernel = float[](
				1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f,
				2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f,
				1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f
			);

		else if (postProcessIndex == 4) // edge detection
			kernel = float[](
				1.0f, 1.0f, 1.0f,
				1.0f, -8.0f, 1.0f,
				1.0f, 1.0f, 1.0f
			);

		else if (postProcessIndex == 5) // test
			kernel = float[](
			-1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f
			);

	    vec3 sampleTex[9];
	    for(int i = 0; i < 9; i++)
	    {
	        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	    }

	    vec3 col = vec3(0.0f);
	    for(int i = 0; i < 9; i++)
	        col += sampleTex[i] * kernel[i];

		fragColor = vec4(col, 1.0f);
	}
}