#pragma once
#include <sstream>

namespace mch {
	namespace literals {
		std::stringstream operator"" _f(const char* const s, const size_t _);
	}

	using namespace mch::literals;
}
