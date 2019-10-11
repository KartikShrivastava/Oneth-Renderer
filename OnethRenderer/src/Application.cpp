#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer.h"
#include "Camera.h"
#include "CheckGLErrors.h"
#include "Shader.h"
#include "TextureStbImage.h"
#include "Model.h"
#include "expat/imgui/imgui.h"
#include "expat/imgui/imgui_impl_glfw_gl3.h"
#include "Text.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 lightPos = glm::vec3(0.0f, 2.0, 0.0f);
glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
glm::vec3 lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 firstCubePos = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 cubeSetupPos = glm::vec3(-12.0f, 0.0f, 3.0f);
glm::vec3 ImportedModelSetupPos = glm::vec3(-6.0f, -1.5f, 3.0f);
bool mouseReleased = true;
std::string debugLog="";

Renderer renderer;
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

int main() {
	if(!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
	-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

	 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
	};
	
	unsigned int cubeVbo;
	GLCall(glGenBuffers(1, &cubeVbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW));
	
	unsigned int texturedCubeVao;
	GLCall(glGenVertexArrays(1, &texturedCubeVao));
	GLCall(glBindVertexArray(texturedCubeVao));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glBindVertexArray(0));

	unsigned int nonTexturedCubeVao;
	GLCall(glGenVertexArrays(1, &nonTexturedCubeVao));
	GLCall(glBindVertexArray(nonTexturedCubeVao));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glBindVertexArray(0));
	
	Shader lampShader_1("src/shaders/1_SimpleVertexShader.glsl", "src/shaders/1_SimpleFragmentShader.glsl");
	Shader noTexShader_2("src/shaders/2_VertexShader.glsl", "src/shaders/2_MaterialFragmentShader.glsl");
	Shader texShader_3("src/shaders/3_VertexShader.glsl", "src/shaders/3_TexturedFragmentShader.glsl");
	Shader lightMapShader_4("src/shaders/3_VertexShader.glsl", "src/shaders/4_TexturedMaterialFragmentShader.glsl");
	Shader directionalLightShader_5("src/shaders/3_VertexShader.glsl", "src/shaders/5_DirectionalLightFS.glsl");
	Shader pointLightShader_6("src/shaders/3_VertexShader.glsl", "src/shaders/6_PointLightFS.glsl");
	Shader spotLightShader_7("src/shaders/3_VertexShader.glsl", "src/shaders/7_SpotLightSmoothFS.glsl");
	Shader mixedLightShader_8("src/shaders/3_VertexShader.glsl", "src/shaders/8_MixedLightFS.glsl");

	TextureStbImage tex1("res/textures/wood.jpg", false);
	TextureStbImage tex2("res/textures/yayi.png", true);
	TextureStbImage tex3("res/textures/diffuseMap.png", false);
	TextureStbImage tex4("res/textures/specularMap.png", false);
	TextureStbImage tex5("res/textures/emissionMap.png", false);

	//setting up uniform(s) whose values are updated only once
	{
		noTexShader_2.Bind();			//u_mvp, u_model, u_light.position, u_camPos			:: uniform(s) updated each frame
		noTexShader_2.SetUniform3f("u_material.ambient", 1.0f, 0.5f, 0.31f);
		noTexShader_2.SetUniform3f("u_material.diffuse", 1.0f, 0.5f, 0.31f);
		noTexShader_2.SetUniform3f("u_material.specular", 1.0f, 0.5f, 0.31f);
		noTexShader_2.SetUniform1f("u_material.shininess", 32.0f);
		noTexShader_2.SetUniform3fv("u_light.ambient", glm::value_ptr(lightAmbient));
		noTexShader_2.SetUniform3fv("u_light.diffuse", glm::value_ptr(lightDiffuse));
		noTexShader_2.SetUniform3fv("u_light.specular", glm::value_ptr(lightSpecular));
		noTexShader_2.UnBind();
	}

	{
		texShader_3.Bind();				//u_mvp, u_model, u_mixValue, u_lightPos				:: uniform(s) updated each frame
		texShader_3.SetUniform1i("u_textureWood", 0);
		texShader_3.SetUniform1i("u_textureYayi", 1);
		texShader_3.SetUniform3fv("u_lightColor", glm::value_ptr(lightDiffuse));
		texShader_3.UnBind();
	}

	{
		lightMapShader_4.Bind();			//u_mvp, u_model, u_light.position, u_camPos			:: uniform(s) updated each frame
		lightMapShader_4.SetUniform1i("u_material.diffuseMap", 0);
		lightMapShader_4.SetUniform1i("u_material.specularMap", 1);
		lightMapShader_4.SetUniform1i("u_material.emissionMap", 2);
		lightMapShader_4.SetUniform1f("u_material.shininess", 64.0f);
		lightMapShader_4.SetUniform3fv("u_light.ambient", glm::value_ptr(lightAmbient));
		lightMapShader_4.SetUniform3fv("u_light.diffuse", glm::value_ptr(lightDiffuse));
		lightMapShader_4.SetUniform3fv("u_light.specular", glm::value_ptr(lightSpecular));
		lightMapShader_4.UnBind();
	}
	//directional light
	{
		directionalLightShader_5.Bind();
		directionalLightShader_5.SetUniform1i("u_material.diffuseMap", 2);
		directionalLightShader_5.SetUniform1i("u_material.specularMap", 3);
		directionalLightShader_5.SetUniform1f("u_material.shine", 128.0f);
		directionalLightShader_5.SetUniform3f("u_light.direction", -0.2f, -1.0f, -0.3f);
		directionalLightShader_5.SetUniform3fv("u_light.ambient", glm::value_ptr(lightAmbient));
		directionalLightShader_5.SetUniform3fv("u_light.diffuse", glm::value_ptr(lightDiffuse));
		directionalLightShader_5.SetUniform3fv("u_light.specular", glm::value_ptr(lightSpecular));
		directionalLightShader_5.UnBind();
	}
	//point light
	{
		pointLightShader_6.Bind();
		pointLightShader_6.SetUniform1i("u_material.diffuseMap", 2);
		pointLightShader_6.SetUniform1i("u_material.specularMap", 3);
		pointLightShader_6.SetUniform1f("u_material.shininess", 128.0f);
		pointLightShader_6.SetUniform3fv("u_light.ambient", glm::value_ptr(lightAmbient));
		pointLightShader_6.SetUniform3fv("u_light.diffuse", glm::value_ptr(lightDiffuse));
		pointLightShader_6.SetUniform3fv("u_light.specular", glm::value_ptr(lightSpecular));
		pointLightShader_6.SetUniform1f("u_light.constant", 1.0f);
		pointLightShader_6.SetUniform1f("u_light.linear", 0.09f);
		pointLightShader_6.SetUniform1f("u_light.quadratic", 0.032f);
		pointLightShader_6.UnBind();
	}
	//spot light (smooth)
	{
		spotLightShader_7.Bind();
		spotLightShader_7.SetUniform1i("u_material.diffuseMap", 2);
		spotLightShader_7.SetUniform1i("u_material.specularMap", 3);
		spotLightShader_7.SetUniform1f("u_material.shininess", 128.0f);
		spotLightShader_7.SetUniform1f("u_light.innerCutoff", glm::cos(glm::radians(5.5f)));		//for smooth spotlight
		spotLightShader_7.SetUniform1f("u_light.outerCutoff", glm::cos(glm::radians(10.5f)));		//for smooth spotlight
		spotLightShader_7.SetUniform3f("u_light.ambient", 0.15f, 0.15f, 0.15f);
		spotLightShader_7.SetUniform3f("u_light.diffuse", 0.5f, 0.5f, 0.5f);
		spotLightShader_7.SetUniform3f("u_light.specular", 1.0f, 1.0f, 1.0f);
		spotLightShader_7.SetUniform1f("u_light.constant", 1.0f);
		spotLightShader_7.SetUniform1f("u_light.linear", 0.027f);
		spotLightShader_7.SetUniform1f("u_light.quadratic", 0.0028f);
		spotLightShader_7.UnBind();
	}

	glm::vec3 pointLightPosition[] = {
		cubeSetupPos + glm::vec3( 3.0f,  0.0f, 2.0f),
		cubeSetupPos + glm::vec3( 5.0f,  0.0f, 3.0f),
		cubeSetupPos + glm::vec3( 6.0f,  0.0f,-2.0f),
		cubeSetupPos + glm::vec3( 3.0f,  1.0f, 0.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 0.5f, 0.2f),
		glm::vec3(0.2f, 1.0f, 1.0f),
		glm::vec3(0.8f, 0.2f, 0.5f),
		glm::vec3(0.2f, 0.2f, 1.0f)
	};
	//mixed light 
	{
		mixedLightShader_8.Bind();
		mixedLightShader_8.SetUniform1f("u_material.shininess", 64.0f);

		mixedLightShader_8.SetUniform3f("u_dLight.direction", -0.5f, -0.5f, -0.3f);
		mixedLightShader_8.SetUniform3f("u_dLight.ambient" , 0.1f, 0.1f, 0.1f);
		mixedLightShader_8.SetUniform3f("u_dLight.diffuse" , 0.4f, 0.4f, 0.4f);
		mixedLightShader_8.SetUniform3f("u_dLight.specular", 0.0f, 0.0f, 0.0f);
		
		//number of point lights (4 in this case) is defined inside the shader
		mixedLightShader_8.SetUniform3fv("u_pLight[0].position", glm::value_ptr(pointLightPosition[0]));
		mixedLightShader_8.SetUniform3f("u_pLight[0].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader_8.SetUniform3fv("u_pLight[0].diffuse", glm::value_ptr(pointLightColors[0]));
		mixedLightShader_8.SetUniform3fv("u_pLight[0].specular", glm::value_ptr(pointLightColors[0]));
		mixedLightShader_8.SetUniform1f("u_pLight[0].constant", 1.0f);
		mixedLightShader_8.SetUniform1f("u_pLight[0].linear", 0.045f);
		mixedLightShader_8.SetUniform1f("u_pLight[0].quadratic", 0.0075f);

		mixedLightShader_8.SetUniform3fv("u_pLight[1].position", glm::value_ptr(pointLightPosition[1]));
		mixedLightShader_8.SetUniform3f("u_pLight[1].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader_8.SetUniform3fv("u_pLight[1].diffuse", glm::value_ptr(pointLightColors[1]));
		mixedLightShader_8.SetUniform3fv("u_pLight[1].specular", glm::value_ptr(pointLightColors[1]));
		mixedLightShader_8.SetUniform1f("u_pLight[1].constant", 1.0f);
		mixedLightShader_8.SetUniform1f("u_pLight[1].linear", 0.045f);
		mixedLightShader_8.SetUniform1f("u_pLight[1].quadratic", 0.0075f);

		mixedLightShader_8.SetUniform3fv("u_pLight[2].position", glm::value_ptr(pointLightPosition[2]));
		mixedLightShader_8.SetUniform3f("u_pLight[2].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader_8.SetUniform3fv("u_pLight[2].diffuse", glm::value_ptr(pointLightColors[2]));
		mixedLightShader_8.SetUniform3fv("u_pLight[2].specular", glm::value_ptr(pointLightColors[2]));
		mixedLightShader_8.SetUniform1f("u_pLight[2].constant", 1.0f);
		mixedLightShader_8.SetUniform1f("u_pLight[2].linear", 0.045f);
		mixedLightShader_8.SetUniform1f("u_pLight[2].quadratic", 0.0075f);

		mixedLightShader_8.SetUniform3fv("u_pLight[3].position", glm::value_ptr(pointLightPosition[3]));
		mixedLightShader_8.SetUniform3f("u_pLight[3].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader_8.SetUniform3fv("u_pLight[3].diffuse", glm::value_ptr(pointLightColors[3]));
		mixedLightShader_8.SetUniform3fv("u_pLight[3].specular", glm::value_ptr(pointLightColors[3]));
		mixedLightShader_8.SetUniform1f("u_pLight[3].constant", 1.0f);
		mixedLightShader_8.SetUniform1f("u_pLight[3].linear", 0.045f);
		mixedLightShader_8.SetUniform1f("u_pLight[3].quadratic", 0.0075f);

		mixedLightShader_8.SetUniform3f("u_sLight.ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader_8.SetUniform3f("u_sLight.diffuse", 0.8f, 0.8f, 0.8f);
		mixedLightShader_8.SetUniform3f("u_sLight.specular", 1.0f, 1.0f, 1.0f);
		mixedLightShader_8.SetUniform1f("u_sLight.constant", 1.0f);
		mixedLightShader_8.SetUniform1f("u_sLight.linear", 0.027f);
		mixedLightShader_8.SetUniform1f("u_sLight.quadratic", 0.0028f);
		mixedLightShader_8.SetUniform1f("u_sLight.innerCutoff", glm::cos(glm::radians(5.5f)));		//for smooth spotlight
		mixedLightShader_8.SetUniform1f("u_sLight.outerCutoff", glm::cos(glm::radians(10.5f)));		//for smooth spotlight
		mixedLightShader_8.UnBind();
	}

	glm::vec3 cubePositions[] = {
		glm::vec3( 2.0f, -2.0f,  0.0f),
		glm::vec3( 3.1f, -2.0f,  0.1f),
		glm::vec3( 4.2f, -2.0f,  0.5f),
		glm::vec3( 5.3f, -2.0f, -0.3f),
		glm::vec3( 6.4f, -2.0f, -0.2f),
		glm::vec3( 2.3f, -1.0f,  0.2f),
		glm::vec3( 3.1f,  0.0f,  0.1f),
		glm::vec3( 3.9f, -1.0f,  0.5f),
		glm::vec3( 4.8f, -2.0f,  2.3f),
		glm::vec3( 5.4f, -2.0f, -1.6f)
	};

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsClassic();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Model crytek("res/nanosuit/nanosuit.obj");
	//Model crytek("res/EUL/EUL4.obj");
	//ImportedModelSetupPos = glm::vec3(-6.0f, 1.5f, 3.0f);

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 mvp;

	Text textRenderer("res/fonts/pixel.ttf", 48);
	Shader textShader("src/shaders/12_textVertexShader.glsl", "src/shaders/12_textFragmentShader.glsl");
	glm::mat4 textOrthoProjMat = glm::ortho(0.0f, 1.0f * renderer.width, 0.0f, 1.0f * renderer.height);
	textShader.Bind();
	textShader.SetUniformMat4fv("u_projection", glm::value_ptr(textOrthoProjMat));
	textShader.UnBind();

	float x = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float currFrame = (float)glfwGetTime();
		renderer.deltaTime = currFrame - renderer.lastFrame;
		renderer.lastFrame = currFrame;

		ProcessInput(window);
		
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		ImGui_ImplGlfwGL3_NewFrame();

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.fov), (float)renderer.width / renderer.height, 0.1f, 100.0f);

		{
			GLCall(glBindVertexArray(nonTexturedCubeVao));
			lampShader_1.Bind();
			model = glm::translate(glm::mat4(1.0f), lightPos);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			mvp = projection * view * model;
			lampShader_1.SetUniform3fv("u_lightColor", glm::value_ptr(lightDiffuse));
			lampShader_1.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			lampShader_1.UnBind();
		}
		
		{
			GLCall(glBindVertexArray(nonTexturedCubeVao));
			noTexShader_2.Bind();
			model = glm::translate(glm::mat4(1.0f), firstCubePos + glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::rotate(model, float(glfwGetTime()), glm::vec3(1.0f, 0.2f, 0.7f));
			mvp = projection * view * model;
			noTexShader_2.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			noTexShader_2.SetUniformMat4fv("u_model", glm::value_ptr(model));
			noTexShader_2.SetUniform3fv("u_light.position", glm::value_ptr(lightPos));
			noTexShader_2.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			noTexShader_2.UnBind();
		}

		{
			GLCall(glBindVertexArray(texturedCubeVao));
			tex1.Bind(0);
			tex2.Bind(1);
			texShader_3.Bind();
			model = glm::translate(glm::mat4(1.0f), firstCubePos + glm::vec3(2.0f, 0.0f, 0.0f));
			model = glm::rotate(model, float(glfwGetTime()), glm::vec3(1.0f, 0.2f, 0.7f));
			mvp = projection * view * model;
			texShader_3.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			texShader_3.SetUniformMat4fv("u_model", glm::value_ptr(model));
			texShader_3.SetUniform1f("u_mixValue", renderer.mix);
			texShader_3.SetUniform3fv("u_lightPos", glm::value_ptr(lightPos));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			texShader_3.UnBind();
			tex1.UnBind();
			tex2.UnBind();
		}

		{
			GLCall(glBindVertexArray(texturedCubeVao));
			tex3.Bind(0);
			tex4.Bind(1);
			tex5.Bind(2);
			lightMapShader_4.Bind();
			model = glm::translate(glm::mat4(1.0f), firstCubePos + glm::vec3(4.0f, 0.0f, 0.0f));
			model = glm::rotate(model, float(glfwGetTime()), glm::vec3(1.0f, 0.2f, 0.7f));
			mvp = projection * view * model;
			lightMapShader_4.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			lightMapShader_4.SetUniformMat4fv("u_model", glm::value_ptr(model));
			lightMapShader_4.SetUniform3fv("u_light.position", glm::value_ptr(lightPos));
			lightMapShader_4.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			lightMapShader_4.UnBind();
			tex3.UnBind();
			tex4.UnBind();
			tex5.UnBind();
		}

		{//4 point lights & 10 cubes setup
			GLCall(glBindVertexArray(nonTexturedCubeVao));
			lampShader_1.Bind();
			for (int i = 0; i < 4; ++i) {
				model = glm::translate(glm::mat4(1.0f), pointLightPosition[i]);
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				mvp = projection * view * model;
				lampShader_1.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
				lampShader_1.SetUniform3fv("u_lightColor", glm::value_ptr(pointLightColors[i]));
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			}
			lampShader_1.UnBind();

			GLCall(glBindVertexArray(texturedCubeVao));
			tex3.Bind(0);
			tex4.Bind(1);
			mixedLightShader_8.Bind();
			mixedLightShader_8.SetUniform1i("u_material.texture_diffuse1", 0);
			mixedLightShader_8.SetUniform1i("u_material.texture_specular1", 1);

			for (int i = 0; i < 10; ++i) {
				model = glm::translate(glm::mat4(1.0f), cubeSetupPos + cubePositions[i]);
				mvp = projection * view * model;
				mixedLightShader_8.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
				mixedLightShader_8.SetUniformMat4fv("u_model", glm::value_ptr(model));
				mixedLightShader_8.SetUniform3fv("u_sLight.direction", glm::value_ptr(camera.front));
				mixedLightShader_8.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			}
			mixedLightShader_8.UnBind();
			tex3.UnBind();
			tex4.UnBind();
		}

		{//imported model setup
			mixedLightShader_8.Bind();
			model = glm::translate(glm::mat4(1.0f), ImportedModelSetupPos);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			mvp = projection * view * model;
			mixedLightShader_8.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			mixedLightShader_8.SetUniformMat4fv("u_model", glm::value_ptr(model));
			mixedLightShader_8.SetUniform3fv("u_sLight.direction", glm::value_ptr(camera.front));
			mixedLightShader_8.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));
			crytek.Draw(mixedLightShader_8);
			mixedLightShader_8.UnBind();
		}
		
		textRenderer.Draw(textShader, debugLog, 10.0f, 10.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		
		{
			ImGui::Begin("Edit");
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			//ImGui::SliderFloat("Slide", &x, 0.0f, 10.0f);
			//ImGui::Text("Change point lights position:");
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &texturedCubeVao);
	glDeleteVertexArrays(1, &nonTexturedCubeVao);
	glDeleteBuffers(1, &cubeVbo);

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	GLCall(glViewport(0, 0, width, height));
	renderer.width = width;
	renderer.height = height;
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
		renderer.lastX = (float)xpos;
		renderer.lastY = (float)ypos;
		renderer.initialMouse = false;
	}
	float xoffset = (float)xpos - renderer.lastX;
	float yoffset = -(float)ypos + renderer.lastY;
	renderer.lastX = (float)xpos;
	renderer.lastY = (float)ypos;

	if (glfwGetMouseButton(window, 0) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		debugLog = "Mouse pressed";
		camera.ProcessMouse(xoffset, yoffset);
		mouseReleased = false;
	}
	else if (!mouseReleased && glfwGetMouseButton(window, 0) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		debugLog = "Mouse released";
		mouseReleased = true;
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessScroll((float)yoffset);
}