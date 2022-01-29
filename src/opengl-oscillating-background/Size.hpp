#pragma once
#include <GLFW/glfw3.h>

namespace mch {
	struct Size {
		const int width;
		const int height;

		Size(const GLFWvidmode* m);
		Size(const int width, const int height);
	};
}
