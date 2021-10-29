#include <glm/glm.hpp>
#include "OscColor.hpp"

namespace mch {
	static float oscSolveElapsed(const float val, const float speed) {
		return glm::asin(val) / speed;
	}

	static glm::vec3 oscSolveElapsed(const glm::vec3& val, const glm::vec3& speed) {
		return {oscSolveElapsed(val.x, speed.x), oscSolveElapsed(val.y, speed.y), oscSolveElapsed(val.z, speed.z)};
	}

	OscColor::OscColor(const glm::vec3& c, const float speed) : c(c), speed(speed), elapsedStart(oscSolveElapsed(c, this->speed)) {
	}

	void OscColor::update(const long long elapsedMS) {
		for(size_t i = 0; i < 3; i++)
			this->c[i] = glm::abs(glm::sin((this->elapsedStart[i] + elapsedMS) * this->speed[i]));
	}

	const glm::vec3 OscColor::val() {
		return this->c;
	}
}
