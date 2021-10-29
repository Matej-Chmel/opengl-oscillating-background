#pragma once
#include <vector>
#include "OscColor.hpp"
#include "Window.hpp"

namespace mch {
	class WindowFactory {
		const Size size;
		const std::string title;

	public:
		Window::ListPtr makeMultiple(const std::vector<OscColor>& colors, const GLFWkeyfun keyCallback);
		std::string nextTitle(const size_t i);
		WindowFactory(const Size& s, const std::string& title);
	};
}
