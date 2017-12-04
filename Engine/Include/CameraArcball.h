#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#define PI 3.1415

namespace Shady {
	class ENGINE_API CameraArcball {
	private:
		GLuint matrix_id;
		float radius;
		float theta;
		float phi;
		glm::vec3 position { 0,0,0 };
		glm::vec3 up { 0,1,0 };
		glm::vec3 last { 0,0,0 };
		double last_x = 0;
		double last_y = 0;
		double scroll;
		GLFWwindow* window;
		bool Fix();
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	public:
		CameraArcball(GLuint, GLFWwindow* = nullptr);
		glm::vec3 get_position();
		void move(glm::vec3);
		void radius_add(float add);
		void theta_add(float add);
		void phi_add(float add);
		void adjust_angle(float theta_adder, float phi_adder);
		void update(bool use_mouse_input = false);
		CameraArcball operator+(const glm::vec3&);
		CameraArcball operator-(const glm::vec3&);
		CameraArcball operator+=(const glm::vec3&);
		CameraArcball operator-=(const glm::vec3&);
		friend ENGINE_API std::ostream &operator<<(std::ostream& output, const CameraArcball& cam);
	};
}