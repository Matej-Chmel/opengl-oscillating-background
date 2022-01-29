#pragma once
#include <GLFW/glfw3.h>
#include <list>
#include <memory>
#include "WindowData.hpp"

namespace mch {
	class Window {
		WindowData d;
		GLFWwindow* w;

	public:
		typedef std::shared_ptr<Window> Ptr;
		typedef std::list<Ptr> List;
		typedef std::shared_ptr<List> ListPtr;

		static Ptr make(const WindowData& d);

		~Window();
		bool closing();
		void move(const int x, const int y);
		void render();
		void setKeyCallback(const GLFWkeyfun cb);
		void updateColor(const long long elapsedMS);
		void use();
		Window(const WindowData& d);
	};
}
