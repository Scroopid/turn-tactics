#include "Input\InputManager.h"

namespace Shady {
	namespace Input {
		InputManager::InputManager() {
			//TODO create priority stack with ability to lock out elements not on top.
			//TODO create awesome function binding stuff, and one that allows you to create a callback for a method in a class instance. kuz that would be cool.
		}


		InputManager::~InputManager() {
		}

		void InputManager::add_listener(std::unique_ptr<InputListener> listener) {
			auto m = (input_things.top()).linsteners;
			for each(KeyboardKeys a in listener->keys) {
				if(m.find(a) == m.end) {
					auto asdf = new std::list<std::unique_ptr<InputListener>>;
					m[a] = *asdf;
				}
				(m[a]).push_back(listener);
			}
		}

		std::unique_ptr<InputListener> InputManager::create_listener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback, 
			std::list<KeyboardKeys> keys) {
			InputListener* handler = new InputListener(callback, keys);
			std::unique_ptr<InputListener> listener_pointer(handler);
			return listener_pointer;
		}

		template<T> std::unique_ptr<InputListener> InputManager::create_listener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback, 
			std::list<KeyboardKeys> keys, T obj) {
			InputListener* handler = new InputListener(callback, keys, obj);
			std::unique_ptr<InputListener> listener_pointer(handler);
			return listener_pointer;
		}

		void InputManager::remove_listener(std::unique_ptr<InputListener> listener) {
			auto m = (input_things.top()).linsteners;
			for each(KeyboardKeys a in listener->keys) {
				if(m.find(a) == m.end) {
					return;
				}
				(m[a]).remove(listener);
			}
		}

		void InputManager::push_input_group() {
			input_things.push(InputGroup());
		}

		void InputManager::pop_input_group() {
			input_things.pop();
		}

		InputManager InputManager::get_instance() {
			return instance;
		}

		void InputManager::set_window(GLFWwindow * window_) {
			window = window_;
		}

		InputListener::InputListener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback_, std::list<KeyboardKeys> keys_) {
			callback = callback_;
			keys = keys_;
			obj = nullptr;
		}
		template<T> InputListener::InputListener(std::function<void(KeyboardKeys, KeyboardKeyState)> callback_, std::list<KeyboardKeys> keys_, 
			T obj_) {
			callback = callback_;
			keys = keys_;
			obj = obj_;//TODO Make this make sense. need to store optional object to call function from;
		}
	}
}