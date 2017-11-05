#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#define PI 3.1415

namespace Shady {
	class CameraArcball {
	private:
		float radius;
		float theta;
		float phi;
		glm::vec3 position { 0,0,0 };
		glm::vec3 up { 0,1,0 };
		glm::vec3 last { 0,0,0 };
		bool Fix();
	public:
		CameraArcball();
		glm::vec3 get_position();
		void move(glm::vec3);
		void radius_add(float add);
		void theta_add(float add);
		void phi_add(float add);
		void update();
		CameraArcball operator+(const glm::vec3&);
		CameraArcball operator-(const glm::vec3&);
		CameraArcball operator+=(const glm::vec3&);
		CameraArcball operator-=(const glm::vec3&);
		friend std::ostream &operator<<(std::ostream& output, const CameraArcball& cam);
	};
}