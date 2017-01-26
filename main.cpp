#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"

int main();

void render(const Model * model);

const mat4 I;
const vec3 O = { 0, 0, 0 };
const vec3 X = { 1, 0, 0 };
const vec3 Y = { 0, 1, 0 };
const vec3 Z = { 0, 0, 1 };

int main()
{
	vec3 cameraPosition = vec3(0, 0, -10);
	vec3 cameraCenter = O;

	try
	{
		Window window = { "DV1541 3D Project", 800, 600 };
		Shader shader = { "shaders/VertexShader.glsl", "shaders/GeometryShader.glsl", "shaders/FragmentShader.glsl" };
		Model manet = { "models/manet.obj" };

		shader.use();
		shader.setUniform("viewProj",
			perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f) *
			lookAt(vec3(0, 0, -10), O, Y)
		);

		while (window.isOpen())
		{
			window.pollEvents();
			shader.setUniform("globalTime", Window::getTime());
			shader.setUniform("world", 
				rotate(
					rotate(
					rotate(I, (float)glfwGetTime(), Y),
					(float)glfwGetTime(), Z), (float)glfwGetTime(), X)
			);

			render(&manet);

			window.swapBuffers();
		}
	}
	catch (const char * message)
	{
		printf("Error: %s", message);
		printf("\nPress enter to exit...");
		getchar();
	}
	
	return 0;
}

void render(const Model * model)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model->draw();
}
