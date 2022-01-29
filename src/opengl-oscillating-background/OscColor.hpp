#pragma once
#include <glm/vec3.hpp>

namespace mch {
	class OscColor {
		glm::vec3 c;
		const glm::vec3 speed;
		const glm::vec3 elapsedStart;

	public:
		OscColor(const glm::vec3& c, const float speed);
		void update(const long long elapsedMS);
		const glm::vec3 val();
	};
}
