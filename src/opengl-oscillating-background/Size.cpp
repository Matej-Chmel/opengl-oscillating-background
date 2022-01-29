#include "Size.hpp"

namespace mch {
	Size::Size(const GLFWvidmode* m) : width(m->width), height(m->height) {}
	Size::Size(const int width, const int height) : width(width), height(height) {}
}
