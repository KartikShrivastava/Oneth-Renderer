#pragma once

class Renderer {
private:

public:
	const int width = 800;
	const int height = 600;
	float mix = 0.2f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = 300, lastY = 400;
	float initialMouse = true;
};