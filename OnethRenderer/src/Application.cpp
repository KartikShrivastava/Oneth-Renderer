#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "CheckGLErrors.h"
#include "Shader.h"
#include "Texture.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 lightPos = glm::vec3(-1.0f, 0.0, -1.0f);

Renderer renderer;
Camera camera;

int main() {
	if(!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(renderer.width, renderer.height, "OnethRenderer", NULL, NULL);
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
	Shader normalShader("src/shaders/2_VertexShader.glsl", "src/shaders/2_2_FragmentShader.glsl");
	Shader lampShader("src/shaders/3_LightVS.glsl", "src/shaders/3_LightFS.glsl");
	Shader lightMapShader("src/shaders/1_VertexShader.glsl", "src/shaders/4_LightMapFS.glsl");

	Texture tex1("res/textures/wood.jpg", false, 0);
	Texture tex2("res/textures/yayi.png", true, 1);
	Texture tex3("res/textures/diffuseMap.png", false, 2);
	Texture tex4("res/textures/specularMap.png", false, 3);
	Texture tex5("res/textures/emissionMap.png", false, 4);
	tex1.UnBind();
	tex2.UnBind();
	tex3.UnBind();
	tex4.UnBind();
	tex5.UnBind();

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	//glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);		//used for 2_FragmentShader

	tex1.Bind();
	tex2.Bind();
	texShader.Bind();
	texShader.SetUniform1i("u_textureWood", 0);
	texShader.SetUniform1i("u_textureYayi", 1);
	texShader.SetUniform3fv("u_lightColor", glm::value_ptr(lightColor));
	texShader.UnBind();
	
	normalShader.Bind();
	normalShader.SetUniform3f("u_material.ambient", 1.0f, 0.5f, 0.31f);
	normalShader.SetUniform3f("u_material.diffuse", 1.0f, 0.5f, 0.31f);
	normalShader.SetUniform3f("u_material.specular", 1.0f, 0.5f, 0.31f);
	normalShader.SetUniform1f("u_material.shininess", 32.0f);
	normalShader.SetUniform3f("u_light.ambient", 0.2f, 0.2f, 0.2);
	normalShader.SetUniform3f("u_light.diffuse", 0.5f, 0.5f, 0.5f);
	normalShader.SetUniform3f("u_light.specular", 1.0f, 1.0f, 1.0f);
	normalShader.UnBind();

	lampShader.Bind();
	lampShader.SetUniform3fv("u_lightColor", glm::value_ptr(lightColor));
	lampShader.UnBind();

	tex3.Bind();
	tex4.Bind();
	tex5.Bind();
	lightMapShader.Bind();
	lightMapShader.SetUniform1i("u_material.diffuseMap", 2);
	lightMapShader.SetUniform1i("u_material.specularMap", 3);
	lightMapShader.SetUniform1i("u_material.emissionMap", 4);
	lightMapShader.SetUniform1f("u_material.shininess", 64);
	lightMapShader.SetUniform3f("u_light.ambient", 0.2f, 0.2f, 0.2);
	lightMapShader.SetUniform3f("u_light.diffuse", 0.5f, 0.5f, 0.5f);
	lightMapShader.SetUniform3f("u_light.specular", 1.0f, 1.0f, 1.0f);
	lightMapShader.UnBind();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float currFrame = glfwGetTime();
		renderer.deltaTime = currFrame - renderer.lastFrame;
		renderer.lastFrame = currFrame;

		ProcessInput(window);
		
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		texShader.Bind();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.0f, 0.0f));
		model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.2f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), renderer.width / renderer.height, 0.1f, 100.0f);
		glm::mat4 mvp = projection * view * model;
		texShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		texShader.SetUniform1f("u_mixValue", renderer.mix);
		texShader.SetUniformMat4fv("u_model", glm::value_ptr(model));
		texShader.SetUniform3fv("u_lightPos", glm::value_ptr(lightPos));
	
		GLCall(glBindVertexArray(texCubeVao));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		normalShader.Bind();
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -5.0f));
		model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		mvp = projection * view * model;
		normalShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		//normalShader.SetUniform3fv("u_objectColor", glm::value_ptr(objectColor));		//used for 2_FragmentShader
		normalShader.SetUniformMat4fv("u_model", glm::value_ptr(model));
		normalShader.SetUniform3fv("u_light.position", glm::value_ptr(lightPos));
		normalShader.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));
		GLCall(glBindVertexArray(lampVao));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		lampShader.Bind();
		model = glm::translate(glm::mat4(1.0f), lightPos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		mvp = projection * view * model;
		lampShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		GLCall(glBindVertexArray(lampVao));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		lightMapShader.Bind();
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -2.0f, -2.5f));
		//model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.7f, 1.0f, 0.0f));
		mvp = projection * view * model;
		lightMapShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
		lightMapShader.SetUniformMat4fv("u_model", glm::value_ptr(model));
		lightMapShader.SetUniform3fv("u_light.position", glm::value_ptr(lightPos));
		lightMapShader.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));
		GLCall(glBindVertexArray(texCubeVao));
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
	float lightSpeed = 2.0f * renderer.deltaTime;

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
		camera.ProcessKeyboard(FORWARD, renderer.deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, renderer.deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, renderer.deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, renderer.deltaTime);

	if(glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		renderer.mix = renderer.mix >= 1.0f ? 1.0f : renderer.mix + 0.001f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		renderer.mix = renderer.mix <= 0.0f ? 0.0f : renderer.mix - 0.001f;

}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (renderer.initialMouse) {
		renderer.lastX = xpos;
		renderer.lastY = ypos;
		renderer.initialMouse = false;
	}
	float xoffset = xpos - renderer.lastX;
	float yoffset = - ypos + renderer.lastY;
	renderer.lastX = xpos;
	renderer.lastY = ypos;

	camera.ProcessMouse(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessScroll(yoffset);
}