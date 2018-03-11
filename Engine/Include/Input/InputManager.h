#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stack>
#include <functional>
#include <map>
#include <memory>
#include <list>
#include "Input\ShadyKeyboard.h"

namespace Shady {
	namespace Input {
		class InputListener {
		public:
			InputListener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback_, std::list<KeyboardKeys> keys_);
			template<T> InputListener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback_, std::list<KeyboardKeys> keys_, T obj_);
			friend class InputManager;
		private:
			std::unique_ptr<void> obj;
			std::function<void(KeyboardKeys, KeyboardKeyState)> callback;
			std::list<KeyboardKeys> keys;
		};

		struct InputGroup {
			std::map<KeyboardKeys, std::list<std::unique_ptr<InputListener>>> linsteners;
			std::string Identifier;
		};

		class InputManager {
		public:
			InputManager();
			~InputManager();
			void add_listener(std::unique_ptr<InputListener> handler);
			std::unique_ptr<InputListener> create_listener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback, std::list<KeyboardKeys> keys);
			template<T> std::unique_ptr<InputListener> create_listener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback, std::list<KeyboardKeys> keys, T obj);
			void remove_listener(std::unique_ptr<InputListener> handler);
			void push_input_group();
			void pop_input_group();
			InputManager get_instance();
			void set_window(GLFWwindow* window);
		private:
			GLFWwindow * window;
			std::stack<InputGroup> input_things;
			static InputManager instance;
		};
	}
}