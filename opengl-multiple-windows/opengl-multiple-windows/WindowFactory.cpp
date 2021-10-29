#include <sstream>
#include "WindowFactory.hpp"

namespace mch {
	Window::ListPtr WindowFactory::makeMultiple(const std::vector<OscColor>& colors, const GLFWkeyfun keyCallback) {
		const auto len = colors.size();
		const auto halfLen = len / 2.f;

		auto windows = std::make_shared<Window::List>();
		const auto winBarHeight = this->screenResolution.height * 0.03125f;
		const auto winSize = Size(this->screenResolution.width / halfLen, this->screenResolution.height / halfLen - winBarHeight);

		for(int i = 0; i < len; i++) {
			auto w = Window::make({colors[i], winSize, this->nextTitle(i + 1)});
			w->move((i & 1) * winSize.width, (i / 2) * winSize.height + winBarHeight);
			w->setKeyCallback(keyCallback);
			windows->push_back(w);
		}
		return windows;
	}

	std::string WindowFactory::nextTitle(const size_t i) {
		return (std::stringstream() << this->title << ' ' << std::to_string(i)).str();
	}

	WindowFactory::WindowFactory(const Size& screenResolution, const std::string& title) : screenResolution(screenResolution), title(title) {}
}
