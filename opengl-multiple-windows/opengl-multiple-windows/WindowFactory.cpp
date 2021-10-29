#include <sstream>
#include "WindowFactory.hpp"

namespace mch {
	Window::ListPtr WindowFactory::makeMultiple(const std::vector<OscColor>& colors, const GLFWkeyfun keyCallback) {
		const auto len = colors.size();
		auto windows = std::make_shared<Window::List>();

		for(size_t i = 0; i < len; i++) {
			auto w = Window::make({colors[i], this->size, this->nextTitle(i + 1)});
			w->setKeyCallback(keyCallback);
			windows->push_back(w);
		}
		return windows;
	}

	std::string WindowFactory::nextTitle(const size_t i) {
		return (std::stringstream() << this->title << ' ' << std::to_string(i)).str();
	}

	WindowFactory::WindowFactory(const Size& s, const std::string& title) : size(s), title(title) {}
}
