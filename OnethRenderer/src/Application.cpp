#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLCall(x)   ClearErrors();\
					x;\
					CheckErrors(#x);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

void ClearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

void CheckErrors(const char* function) {
	while (GLenum error = glGetError()) {
		std::stringstream ss;
		ss << std::hex << error;
		std::cout << "OPENGK error occoured : 0x" << ss.str() << "\ngl function : " << function << std::endl;
	}
}

int main() {
	if(!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 500, "OnethRenderer", NULL, NULL);
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

	const char* vertexShader =
		"#version 430 core\n"
		"layout (location = 0) in vec4 pos;\n"
		"\n"
		"void main(){\n"
		"gl_Position = pos;\n"
		"}\0";

	const char* fragmentShader1 =
		"#version 430 core\n"
		"\n"
		"out vec4 col;\n"
		"\n"
		"void main(){\n"
		"	col = vec4(0.8, 0.0, 0.2, 1.0);\n"
		"}\0";

	const char* fragmentShader2 =
		"#version 430 core\n"
		"\n"
		"out vec4 col;\n"
		"\n"
		"void main(){\n"
		"	col = vec4(1.0, 0.8, 0.5, 1.0);\n"
		"}\0";

	GLCall(unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(vertexID, 1, &vertexShader, NULL));
	GLCall(glCompileShader(vertexID));
	
	GLCall(unsigned int fragmentID1 = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragmentID1, 1, &fragmentShader1, NULL));
	GLCall(glCompileShader(fragmentID1));

	GLCall(unsigned int fragmentID2 = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragmentID2, 1, &fragmentShader2, NULL));
	GLCall(glCompileShader(fragmentID2));
	
	GLCall(unsigned int programID1 = glCreateProgram());
	GLCall(glAttachShader(programID1, vertexID));
	GLCall(glAttachShader(programID1, fragmentID1));
	GLCall(glLinkProgram(programID1));
	GLCall(unsigned int programID2 = glCreateProgram());
	GLCall(glAttachShader(programID2, vertexID));
	GLCall(glAttachShader(programID2, fragmentID2));
	GLCall(glLinkProgram(programID2));

	int status;
	GLCall(glGetProgramiv(programID1, GL_LINK_STATUS, &status));
	if (!status) {
		int len;
		GLCall(glGetProgramiv(programID1, GL_INFO_LOG_LENGTH, &len));
		char* buff = (char*)alloca(len * sizeof(char));
		GLCall(glGetProgramInfoLog(programID1, len, &len, buff));
		std::cout << "Compilation failed\n" << buff << std::endl;
	}

	float vertices[] = {
		-0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f};
	
	unsigned int indices[] = { 
		0, 1, 2,
		0, 2, 3};
	
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glGenBuffers(1, &ebo));
	GLCall(glGenVertexArrays(1, &vao));

	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	GLCall(glBindVertexArray(0));


	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		
		GLCall(glClearColor(0.1f, 0.0f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(programID1));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glBindVertexArray(vao));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		GLCall(glUseProgram(programID2));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glBindVertexArray(vao));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
}