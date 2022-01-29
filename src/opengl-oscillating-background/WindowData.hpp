#pragma once
#include <string>
#include "OscColor.hpp"
#include "Size.hpp"

namespace mch {
	struct WindowData {
		OscColor color;
		Size size;
		std::string title;
	};
}
