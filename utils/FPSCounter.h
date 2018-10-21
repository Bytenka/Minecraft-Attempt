#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

namespace tk
{
struct FPSCounter
{
  public:
	FPSCounter()
		: displayFPS(true), initialTime(glfwGetTime()), enlapsedTime(0), frames(0)
	{
	}
	~FPSCounter() {}

	void frameBegin()
	{
		frameStartTime = glfwGetTime();
	}

	void frameEnd()
	{
		frames++;
		enlapsedTime += glfwGetTime() - frameStartTime;
		if ((glfwGetTime() - initialTime) > 1)
		{
			if (displayFPS)
				LOG_INFO("FPS: " << frames);

			enlapsedTime = 0;
			frames = 0;
			initialTime = glfwGetTime();
		}
	}
	bool displayFPS;

  private:
	double initialTime;
	double frameStartTime;
	double enlapsedTime;
	unsigned int frames;
};

} // namespace tk