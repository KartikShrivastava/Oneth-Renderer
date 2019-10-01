#pragma once

class Renderer {
private:

public:
	const int width = 1080;
	const int height = 700;
	float mix = 0.2f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = 300, lastY = 400;
	float initialMouse = true;
};