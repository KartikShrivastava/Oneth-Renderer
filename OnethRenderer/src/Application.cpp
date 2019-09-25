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
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

glm::mat4 MyLookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& worldUp);
const float width = 800.0f;
const float height = 600.0f;
float mix = 0.2f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(-1.0f, 0.0, -1.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 300, lastY = 400;
float sensitivity = 0.05f;
float initialMouse = true;
float pitch = 0.0f;
float yaw = -90.0f;
float fov = 45.0f;

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
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
	
	-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
	
	-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
	
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
	
	-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
	
	-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f 
	};
	
	unsigned int cubeVbo;
	GLCall(glGenBuffers(1, &cubeVbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW));
	
	unsigned int texCubeVao;
	GLCall(glGenVertexArrays(1, &texCubeVao));
	GLCall(glBindVertexArray(texCubeVao));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glBindVertexArray(0));

	unsigned int lampVao;
	GLCall(glGenVertexArrays(1, &lampVao));
	GLCall(glBindVertexArray(lampVao));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glBindVertexArray(0));

	Shader texShader("src/shaders/1_VertexShader.glsl", "src/shaders/1_FragmentShader.glsl");
	Shader normalShader("src/shaders/2_VertexShader.glsl", "src/shaders/2_FragmentShader.glsl");
	Shader lampShader("src/shaders/3_LightVS.glsl", "src/shaders/3_LightFS.glsl");

	Texture tex1("res/textures/wood.jpg", GL_RGB, GL_RGB, 0, false);
	Texture tex2("res/textures/yayi.png", GL_RGBA, GL_RGBA, 1, true);
	tex1.UnBind();
	tex2.UnBind();

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

	tex1.Bind();
	tex2.Bind();
	texShader.Bind();
	texShader.SetUniform1i("u_textureWood", 0);
	texShader.SetUniform1i("u_textureYayi", 1);
	texShader.SetUniform3fv("u_lightColor", glm::value_ptr(lightColor));
	texShader.UnBind();
	
	normalShader.Bind();
	normalShader.SetUniform3fv("u_lightColor", glm::value_ptr(lightColor));
	normalShader.UnBind();

	lampShader.Bind();
	lampShader.SetUniform3fv("u_lightColor", glm::value_ptr(lightColor));
	lampShader.UnBind();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		ProcessInput(window);
		
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		texShader.Bind();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.0f, 0.0f));
		model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
		glm::mat4 mvp = projection * view * model;
		texShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		texShader.SetUniform1f("u_mixValue", mix);
		texShader.SetUniformMat4fv("u_model", glm::value_ptr(model));
		texShader.SetUniform3fv("u_lightPos", glm::value_ptr(lightPos));
	
		GLCall(glBindVertexArray(texCubeVao));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		normalShader.Bind();
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -5.0f));
		model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		mvp = projection * view * model;
		normalShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		normalShader.SetUniform3fv("u_objectColor", glm::value_ptr(objectColor));
		normalShader.SetUniformMat4fv("u_model", glm::value_ptr(model));
		normalShader.SetUniform3fv("u_lightPos", glm::value_ptr(lightPos));
		normalShader.SetUniform3fv("u_camPos", glm::value_ptr(cameraPos));
		GLCall(glBindVertexArray(lampVao));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		lampShader.Bind();
		model = glm::translate(glm::mat4(1.0f), lightPos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		mvp = projection * view * model;
		lampShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		GLCall(glBindVertexArray(lampVao));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &texCubeVao);
	glDeleteVertexArrays(1, &lampVao);
	glDeleteBuffers(1, &cubeVbo);

	glfwTerminate();
	std::cin.get();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	GLCall(glViewport(0, 0, width, height));
}

void ProcessInput(GLFWwindow* window) {
	float cameraSpeed = 5.0f * deltaTime;
	float lightSpeed = 2.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		lightPos += glm::vec3(0.0f, 0.0f, -lightSpeed);
	else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos += glm::vec3(-lightSpeed, 0.0f, 0.0f);
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPos += glm::vec3(0.0f, lightSpeed, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos += glm::vec3(lightSpeed, 0.0f, 0.0f);
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPos += glm::vec3(0.0f, -lightSpeed, 0.0f);
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		lightPos += glm::vec3(0.0f, 0.0f, lightSpeed);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))* cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))* cameraSpeed;

	if(glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		mix = mix >= 1.0f ? 1.0f : mix + 0.001f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		mix = mix <= 0.0f ? 0.0f : mix - 0.001f;

}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (initialMouse) {
		lastX = xpos;
		lastY = ypos;
		initialMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = - ypos + lastY;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw = std::fmod((yaw+xoffset),360.0f);
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
	cameraFront = front;
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
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