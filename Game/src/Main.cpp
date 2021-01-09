#include <iostream>
#include "Canis/Window.h"

int main()
{
	Window window("Game", 1280, 1280);

	float x = 0.0f;
	float y = 0.0f;
	float speed = 2.5f;
	while (!window.IsClosed())
	{
		if (x < 0.9f)
			x += speed / 10 * window.GetDeltaTime();
		if (x > 1.0f)
			x = 1.0;

		if (y < 1.0f)
			y += speed / 10 * window.GetDeltaTime();
		if (y > 1.0f)
			y = 1.0;

		// window.DrawQuad(x, 0.0f, 0.5f, 0.8f);
		// window.DrawTriangle(0.5f, 0.2f);
		window.DrawQuad(-0.5f * sin(y), 0.5f * x, 0.8f, 0.2f, 0.9f, 1.0f);
		window.DrawQuad(0.5f * sin(y), -0.5f * x, 0.2f, 1.0f, 0.3f, 1.0f);
		window.Update();

	}
}