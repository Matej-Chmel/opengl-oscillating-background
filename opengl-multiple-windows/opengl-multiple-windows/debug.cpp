#include <iostream>
#include "debug.hpp"

namespace mch {
	void print(const glm::vec3& v) {
		std::cout << '(' << v.x << ", " << v.y << ", " << v.z << ")\n";
	}
}
