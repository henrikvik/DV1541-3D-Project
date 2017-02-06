#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "LightManager.h"
#include "FullscreenQuad.h"
#include "Input.h"
#include "DefferedRenderer.h"

const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 600;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		Window window = { "DV1541 3D Project", WINDOW_WIDTH, WINDOW_HEIGHT };
		Camera camera = { {0, 0, -5}, {0,0,0}, {0,1,0} };
		

		DefferedRenderer defferedRenderer(WINDOW_WIDTH, WINDOW_HEIGHT);

		Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");

		GameObjectManager objectManager;
		LightManager lightManager;
		lightManager.add(LightManager::Light( vec3(-10,  0, -10), vec3(1, 0, 0) ));
		lightManager.add(LightManager::Light( vec3( 10, -10, -10), vec3(.75, 1, 0) ));
		lightManager.add(LightManager::Light( vec3(  0, 10,   0), vec3(1, 1, 1) ));

		Window window = { "DV1541 3D Project", 800, 600 };
		Shader shader = { "shaders/VertexShader.glsl", "shaders/BackfaceCulling.glsl", "shaders/FragmentShader.glsl" };
		Shader glowShader = { "shaders/VertexShader.glsl", "shader/brightnessFragmentShader.glsl" };
		Model manet = { "models/manet.obj" };

		GLboolean horizontal = true, firstIteration = true;
		GLuint amount = 10; //5 horizontal, 5 vertical 


		shader.use();
		shader.setUniform("view", lookAt(vec3(0, 0, -10), O, Y));
		shader.setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
		shader.setUniform("viewPoint", vec3(0, 0, -10));
		objectManager.add(&manet);


		double thisFrame = 0, lastFrame = 0, deltaTime = 0;
		while (window.isOpen())
		{
			thisFrame = window.getTime();
			deltaTime = thisFrame - lastFrame;
			window.pollEvents();
			shader.use();
			shader.setUniform("globalTime", (float)glfwGetTime());
			shader.setUniform("world", 
				rotate(
					rotate(
					rotate(I, (float)glfwGetTime(), Y),
					(float)glfwGetTime(), Z), (float)glfwGetTime(), X)
			);
			render(&manet);

			//glow


			camera.update(deltaTime);
			defferedRenderer.draw(camera, objectManager, lightManager);

			window.swapBuffer();
			window.clearBuffer();
			lastFrame = thisFrame;
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
