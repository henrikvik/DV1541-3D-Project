#version 440 core
layout(location = 0)in vec3 fragment_position;
layout(location = 1)in vec3 fragment_normal;
layout(location = 2)in vec2 texCoords_in;

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 bright_color;
	
vec3 ambient_light = vec3(0.f, 0.f, 0.f);
vec3 light_position = vec3(0.f, 0.f, -100.f);
vec3 light_color = vec3(1.f, 1.f, 1.f);
float specularStrength = 0.5f;

uniform vec3 view;
uniform sampler2D diffuseTexture;

vec3 ambient()
{
  return vec3(ambient_light);
}

vec3 diffuse()
{
  vec3 lightDir = normalize(light_position - fragment_position);


  return diffuseColor * max(dot(fragment_normal, lightDir), 0.f);
}

vec4 specular()
{
 	vec4 specularColor = vec4(light_color, 1.f);

	vec3 lightDir = normalize(light_position - fragment_position);
	vec3 viewDir = normalize(viewPosition - fragment_position);
	vec3 reflectDir = reflect(-lightDir, fragment_normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	return specularColor * spec;
}

void main()
{
  fragment_color = normalize(gl_FragCoord);
  fragment_color *= (ambient() + diffuse() + specular());
}
