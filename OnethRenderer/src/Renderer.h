#pragma once

class Renderer {
private:

public:
	const float width = 800.0f;
	const float height = 600.0f;
	float mix = 0.2f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = 300, lastY = 400;
	float initialMouse = true;
};