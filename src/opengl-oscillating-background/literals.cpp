#include "literals.hpp"

namespace mch::literals {
	std::stringstream operator"" _f(const char* const s, const size_t _) {
		return std::stringstream() << s;
	}
}
