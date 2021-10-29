#include <chrono>
#include <cstdlib>
#include <exception>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
namespace chrono = std::chrono;

constexpr auto SPEED = 0.001f;
constexpr auto TITLE = "Window";
constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;

constexpr glm::vec3 BLUE{0.f, 0.f, 1.f};
constexpr glm::vec3 GREEN{0.f, 1.f, 0.f};
constexpr glm::vec3 RED{1.f, 0.f, 0.f};
constexpr glm::vec3 YELLOW{1.f, 1.f, 0.f};

bool running = true;

class AppError : public std::exception {
public:
	AppError(const char* const msg) : std::exception(msg) {}
	AppError(const std::string& msg) : AppError(msg.c_str()) {}
	AppError(const std::stringstream& s) : AppError(s.str()) {}
};

static void clearColor(const glm::vec3& c) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(c.x, c.y, c.z, 1.f);
}

static void errorCallback(const int code, const char* const desc) {
	std::cerr << "[ERROR " << code << "] " << desc << ".\n";
}

static void keyCallback(GLFWwindow*, int key, int, int, int) {
	if(key == GLFW_KEY_ESCAPE)
		running = false;
}

static std::stringstream operator"" _f(const char* const s, const size_t _) {
	return std::stringstream() << s;
}

static float oscSolveElapsed(const float val, const float speed) {
	return glm::asin(val) / speed;
}

static glm::vec3 oscSolveElapsed(const glm::vec3& val, const glm::vec3& speed) {
	return {oscSolveElapsed(val.x, speed.x), oscSolveElapsed(val.y, speed.y), oscSolveElapsed(val.z, speed.z)};
}

class OscColor {
	glm::vec3 c;
	const glm::vec3 speed;
	const glm::vec3 elapsedStart;

public:
	OscColor(const glm::vec3& c, const float speed) : c(c), speed(speed), elapsedStart(oscSolveElapsed(c, this->speed)) {}

	void update(const long long elapsedMS) {
		for(size_t i = 0; i < 3; i++)
			this->c[i] = glm::abs(glm::sin((this->elapsedStart[i] + elapsedMS) * this->speed[i]));
	}

	const glm::vec3 val() {
		return this->c;
	}
};

// DEBUG
void print(const glm::vec3& v) {
	std::cout << '(' << v.x << ", " << v.y << ", " << v.z << ")\n";
}

struct Size {
	const int width;
	const int height;
};

struct WindowData {
	OscColor color;
	Size size;
	std::string title;
};

class Window {
	WindowData d;
	GLFWwindow* w;

public:
	typedef std::shared_ptr<Window> Ptr;
	typedef std::list<Ptr> List;
	typedef std::shared_ptr<List> ListPtr;

	static Ptr make(const WindowData& d) {
		return std::make_shared<Window>(d);
	}

	~Window() {
		glfwDestroyWindow(this->w);
	}

	bool closing() {
		return glfwWindowShouldClose(this->w) == GLFW_TRUE;
	}

	void render() {
		this->use();
		glViewport(0, 0, this->d.size.width, this->d.size.height);
		clearColor(this->d.color.val());
		glfwSwapBuffers(this->w);
	}

	void setKeyCallback(const GLFWkeyfun cb) {
		glfwSetKeyCallback(this->w, cb);
	}

	void updateColor(const long long elapsedMS) {
		this->d.color.update(elapsedMS);
	}

	void use() {
		glfwMakeContextCurrent(this->w);
	}

	Window(const WindowData& d)
		: d(d), w(glfwCreateWindow(this->d.size.width, this->d.size.height, this->d.title.c_str(), nullptr, nullptr)) {

		if(!this->w)
			throw AppError("Could not create window \""_f << this->d.title << "\".");
	}
};

class WindowFactory {
	const Size size;
	const std::string title;

public:
	Window::ListPtr makeMultiple(const std::vector<OscColor>& colors) {
		const auto len = colors.size();
		auto windows = std::make_shared<Window::List>();

		for(size_t i = 0; i < len; i++) {
			auto w = Window::make({colors[i], this->size, this->nextTitle(i + 1)});
			w->setKeyCallback(keyCallback);
			windows->push_back(w);
		}
		return windows;
	}

	std::string nextTitle(const size_t i) {
		return (std::stringstream() << this->title << ' ' << std::to_string(i)).str();
	}

	WindowFactory(const Size& s, const std::string& title) : size(s), title(title) {}
};

int main() {
	try {
		glfwSetErrorCallback(errorCallback);

		if(glfwInit() == GLFW_FALSE)
			throw AppError("Could not init GLFW.");

		auto windows = WindowFactory({WIDTH, HEIGHT}, TITLE).makeMultiple({
			OscColor(BLUE, SPEED),
			OscColor(GREEN, SPEED),
			OscColor(RED, SPEED),
			OscColor(YELLOW, SPEED)
		});
		(*windows->begin())->use();

		glewExperimental = GL_TRUE;
		const auto err = glewInit();

		if(err != GLEW_OK)
			throw AppError("Could not init GLEW. "_f << glewGetErrorString(err) << '.');

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

	} catch(const AppError& e) {
		glfwTerminate();

		std::cerr << "[AppError] " << e.what() << '\n';
		std::cout << "Press ENTER to exit.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
