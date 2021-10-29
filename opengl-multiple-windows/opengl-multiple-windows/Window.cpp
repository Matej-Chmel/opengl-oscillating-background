#include <GL/glew.h>
#include "AppError.hpp"
#include "literals.hpp"
#include "Window.hpp"

namespace mch {
	static void clearColor(const glm::vec3& c) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(c.x, c.y, c.z, 1.f);
	}

	Window::Ptr Window::make(const WindowData& d) {
		return std::make_shared<Window>(d);
	}

	Window::~Window() {
		glfwDestroyWindow(this->w);
	}

	bool Window::closing() {
		return glfwWindowShouldClose(this->w) == GLFW_TRUE;
	}

	void Window::render() {
		this->use();
		glViewport(0, 0, this->d.size.width, this->d.size.height);
		clearColor(this->d.color.val());
		glfwSwapBuffers(this->w);
	}

	void Window::setKeyCallback(const GLFWkeyfun cb) {
		glfwSetKeyCallback(this->w, cb);
	}

	void Window::updateColor(const long long elapsedMS) {
		this->d.color.update(elapsedMS);
	}

	void Window::use() {
		glfwMakeContextCurrent(this->w);
	}

	Window::Window(const WindowData& d)
		: d(d), w(glfwCreateWindow(this->d.size.width, this->d.size.height, this->d.title.c_str(), nullptr, nullptr)) {

		if(!this->w)
			throw AppError("Could not create window \""_f << this->d.title << "\".");
	}
}
