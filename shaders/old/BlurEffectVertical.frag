#version 440 core

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 Blur;

uniform sampler2D glow;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 texOffset = 1.0 / textureSize(glow, 0);
	vec3 result = texture(glow, TexCoords).rgb * weight[0];

	for(int i = 1; i < 5; i++) {
		result += texture(glow, TexCoords + vec2(0.0, texOffset.y * i)).rgb * weight[i];
		result += texture(glow, TexCoords - vec2(0.0, texOffset.y * i)).rgb * weight[i];
	}

	Blur = vec4(result, 1.0);
}
