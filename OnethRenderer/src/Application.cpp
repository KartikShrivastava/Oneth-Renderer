#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CheckGLErrors.h"
#include "Shader.h"
#include "Texture.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

float mix = 0.5f;
float scaleFactor = 1.0f;

int main() {
	if(!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "OnethRenderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Unable to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Unable to intialize GLAD" << std::endl;
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f
	};
	
	unsigned int indices[] = { 
		0, 1, 2,
		1, 3, 2
	};
	
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glGenBuffers(1, &ebo));
	GLCall(glGenVertexArrays(1, &vao));

	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	GLCall(glBindVertexArray(0));

	glm::mat4 modelToWorldMat = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));
	modelToWorldMat = glm::rotate(modelToWorldMat, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelToWorldMat = glm::scale(modelToWorldMat, glm::vec3(0.5f, 0.5f, 0.5f));

	Shader shader1("src/shaders/1_VertexShader.glsl", "src/shaders/1_FragmentShader.glsl");
	Shader shader2("src/shaders/2_VertexShader.glsl", "src/shaders/2_FragmentShader.glsl");

	Texture tex1("res/textures/brick.jpg", GL_RGB, GL_RGB, 0, false);
	Texture tex2("res/textures/yayi.png", GL_RGBA, GL_RGBA, 1, true);
	tex1.UnBind();
	tex2.UnBind();

	tex1.Bind();
	tex2.Bind();
	shader1.Bind();
	shader1.SetUniform1i("u_textureWood", 0);
	shader1.SetUniform1i("u_textureYayi", 1);
	shader1.SetUniformMat4fv("u_mTWMat", glm::value_ptr(modelToWorldMat));
	shader1.UnBind();
	
	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		
		GLCall(glClearColor(0.01f, 0.0f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		shader1.Bind();
		//modelToWorldMat = glm::rotate(glm::mat4(1.0f), (float)sin(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelToWorldMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)*scaleFactor);
		shader1.SetUniformMat4fv("u_mTWMat", glm::value_ptr(modelToWorldMat));
		shader1.SetUniform1f("u_mixValue", mix);
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glBindVertexArray(vao));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		shader2.Bind();
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glBindVertexArray(vao));
		//GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	glfwTerminate();
	std::cin.get();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	GLCall(glViewport(0, 0, width, height));
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mix = mix >= 1.0f ? 1.0f : mix + 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mix = mix <= 0.0f ? 0.0f : mix - 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		scaleFactor += 0.001f;
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		scaleFactor -= 0.001f;
	}
}
