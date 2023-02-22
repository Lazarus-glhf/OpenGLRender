#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed!" << std::endl;
		return -2;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-10.5f, -10.5f, 0.0f, 0.0f, // 0
		 10.5f, -10.5f, 1.0f, 0.0f, // 1
		 10.5f,  10.5f, 1.0f, 1.0f, // 2
		-10.5f,  10.5f, 0.0f, 1.0f  // 3
	};

	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	//Buffer
	VertexArray va;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indicies, 6);

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::vec4 vp(-0.5f, -0.5f, 0.0f, 1.0f);
	glm::vec4 res = proj * vp;
	std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	vp = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
	res = vp * proj;
	std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	vp = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	res = vp * proj;
	std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	vp = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
	res = vp * proj;
	std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	// glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

	// glm::mat4 mvp = proj * view;

	// Shader
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", proj);

	Texture texture("res/texture/Avatar.jpg");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.UnBind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	Renderer renderer;

	float r = 0.0f;
	float increment = 0.05f;
	
	/* Loop until user close the windows */
	while(!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		shader.Bind();
		// shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer.Draw(va, ib, shader);

		if (r > 1.0f)
		{
			increment = -0.05f;
		}
		else if (r < 0.0f)
		{
			increment = 0.05f;
		}
		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* poll for and process events */
		glfwPollEvents(); 
	}

	glfwTerminate();
	return 0;
}
