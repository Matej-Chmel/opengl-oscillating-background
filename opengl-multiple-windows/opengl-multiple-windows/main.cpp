#include <chrono>
#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include "AppError.hpp"
#include "colors.hpp"
#include "literals.hpp"
#include "WindowFactory.hpp"
namespace chrono = std::chrono;
using namespace mch::colors;
using namespace mch::literals;

constexpr auto SPEED = 0.001f;
constexpr auto TITLE = "Window";
constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;

bool running = true;

static void errorCallback(const int code, const char* const desc) {
	std::cerr << "[ERROR " << code << "] " << desc << ".\n";
}

static void keyCallback(GLFWwindow*, int key, int, int, int) {
	if(key == GLFW_KEY_ESCAPE)
		running = false;
}

int main() {
	try {
		glfwSetErrorCallback(errorCallback);

		if(glfwInit() == GLFW_FALSE)
			throw mch::AppError("Could not init GLFW.");

		auto windows = mch::WindowFactory({WIDTH, HEIGHT}, TITLE).makeMultiple({
			mch::OscColor(BLUE, SPEED),
			mch::OscColor(GREEN, SPEED),
			mch::OscColor(RED, SPEED),
			mch::OscColor(YELLOW, SPEED)
		}, keyCallback);
		(*windows->begin())->use();

		glewExperimental = GL_TRUE;
		const auto err = glewInit();

		if(err != GLEW_OK)
			throw mch::AppError("Could not init GLEW. "_f << glewGetErrorString(err) << '.');

		const auto start = chrono::system_clock::now();

		while(running && windows->size() > 0) {
			const auto elapsed = chrono::duration_cast<chrono::milliseconds>(start - chrono::system_clock::now()).count();
			auto i = windows->begin();

			while(i != windows->end()) {
				const auto w = *i;

				if(w->closing())
					i = windows->erase(i);
				else {
					w->updateColor(elapsed);
					w->render();
					i++;
				}
			}

			glfwPollEvents();
		}

		glfwTerminate();

	} catch(const mch::AppError& e) {
		glfwTerminate();

		std::cerr << "[AppError] " << e.what() << '\n';
		std::cout << "Press ENTER to exit.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
