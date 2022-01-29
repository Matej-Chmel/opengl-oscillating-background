#include <sstream>
#include "literals.hpp"
#include "WindowFactory.hpp"

namespace mch {
	Window::ListPtr WindowFactory::makeMultiple(const std::vector<OscColor>& colors, const GLFWkeyfun keyCallback) {
		const auto len = colors.size();
		const auto halfLen = float(len) / 2.f;

		auto windows = std::make_shared<Window::List>();
		const auto winBarHeight = float(this->screenResolution.height) * 0.03125f;
		const auto winSize = Size(
			int(float(this->screenResolution.width) / halfLen),
			int(float(this->screenResolution.height) / halfLen - winBarHeight)
		);

		for(size_t i = 0; i < len; i++) {
			auto w = Window::make({colors[i], winSize, this->nextTitle(i + 1)});
			w->move(
				(i & 1) * winSize.width,
				int(float((i / 2) * winSize.height) + winBarHeight)
			);
			w->setKeyCallback(keyCallback);
			windows->push_back(w);
		}
		return windows;
	}

	std::string WindowFactory::nextTitle(const size_t i) {
		return (""_f << this->title << ' ' << std::to_string(i)).str();
	}

	WindowFactory::WindowFactory(const Size& screenResolution, const std::string& title) : screenResolution(screenResolution), title(title) {}
}
