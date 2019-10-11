#pragma once

class Renderer {
private:

public:
	int width = 800;
	int height = 500;
	float mix = 0.2f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = 300, lastY = 400;
	float initialMouse = true;
};