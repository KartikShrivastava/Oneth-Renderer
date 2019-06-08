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
glm::mat4 MyLookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& worldUp);

const unsigned int width = 800;
const unsigned int height = 600;
float mix = 0.2f;
float scaleFactor = 1.0f;

int main() {
	if(!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "OnethRenderer", NULL, NULL);
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

	float verticesPlane[] = {
		-0.5f, -0.5f,  0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f,  0.0f,	1.0f, 1.0f
	};

	float verticesBox[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	unsigned int vbo;
	unsigned int vao;
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glGenVertexArrays(1, &vao));

	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glBindVertexArray(0));

	glm::mat4 model = glm::mat4(1.0f);
	
	//camera simulation
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -3.0f));

	glm::mat4 view = MyLookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 mvp = projection * view * model;

	Shader shader1("src/shaders/1_VertexShader.glsl", "src/shaders/1_FragmentShader.glsl");
	Shader shader2("src/shaders/2_VertexShader.glsl", "src/shaders/2_FragmentShader.glsl");

	Texture tex1("res/textures/wood.jpg", GL_RGB, GL_RGB, 0, false);
	Texture tex2("res/textures/yayi.png", GL_RGBA, GL_RGBA, 1, true);
	tex1.UnBind();
	tex2.UnBind();

	tex1.Bind();
	tex2.Bind();
	shader1.Bind();
	shader1.SetUniform1i("u_textureWood", 0);
	shader1.SetUniform1i("u_textureYayi", 1);
	shader1.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
	shader1.UnBind();
	
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		shader1.Bind();
		for(int i=0;i<10;++i){
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			view = MyLookAt(glm::vec3(sin(glfwGetTime()) * 10.0f, 0.0f, cos(glfwGetTime()) * 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = projection * view * model;
			shader1.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			shader1.SetUniform1f("u_mixValue", mix);
			GLCall(glBindVertexArray(vao));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

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

glm::mat4 MyLookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& worldUp) {
	glm::vec3 cameraZ = glm::normalize(cameraPos - cameraTarget);	//direction vector
	glm::vec3 cameraX = glm::cross(worldUp, cameraZ);				//right
	glm::vec3 cameraY = glm::cross(cameraZ, cameraX);				//UP
	glm::mat4 viewMatrix(cameraX.x, cameraX.y, cameraX.z, 0,
		cameraY.x, cameraY.y, cameraY.z, 0,
		cameraZ.x, cameraZ.y, cameraZ.z, 0,
		0, 0, 0, 1);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), -cameraPos);
	view = glm::transpose(viewMatrix) * view;
	return view;
}