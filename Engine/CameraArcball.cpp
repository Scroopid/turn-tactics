#include "CameraArcball.h"

bool Shady::CameraArcball::Fix() {
	while(theta < 0)
		theta += PI;
	while(theta > PI)
		theta -= PI;
	while(phi < 0)
		phi += 2 * PI;
	while(phi > 2 * PI)
		phi -= 2 * PI;

	return false;
}

Shady::CameraArcball::CameraArcball() {
	radius = 10;
	theta = PI/2;
	phi = PI;
}

glm::vec3 Shady::CameraArcball::get_position() {
	return glm::vec3(position);
}

void Shady::CameraArcball::move(glm::vec3 add) {
	position += add;
}

void Shady::CameraArcball::radius_add(float add) {
	radius += add;
}

void Shady::CameraArcball::theta_add(float add) {
	theta += add;
	while(theta < 0)
		theta += PI;
	while(theta > PI)
		theta -= PI;
}

void Shady::CameraArcball::phi_add(float add) {
	phi += add;
	while(phi < 0)
		phi += 2 * PI;
	while(phi > 2 * PI)
		phi -= 2 * PI;
}

void Shady::CameraArcball::update() {
	last[0] = position[0] + radius*cos(phi)*sin(theta);
	last[1] = position[1] + radius*sin(phi)*sin(theta);
	last[2] = position[2] + radius*cos(theta);
	glm::lookAt(last, position, up);
}

Shady::CameraArcball Shady::CameraArcball::operator+(const glm::vec3& add) {
	position += add;
	return *this;
}

Shady::CameraArcball Shady::CameraArcball::operator-(const glm::vec3& add) {
	position -= add;
	return *this;
}

Shady::CameraArcball Shady::CameraArcball::operator+=(const glm::vec3& add) {
	position += add;
	return *this;
}

Shady::CameraArcball Shady::CameraArcball::operator-=(const glm::vec3& add) {
	position -= add;
	return *this;
}

std::ostream & Shady::operator<<(std::ostream & output, const CameraArcball& cam) {
	output << "X: " << cam.position[0] << ", Y: " << cam.position[1] << ", Z: " << cam.position[2] << "\n";
	output << "Radius: " << cam.radius << ", Theta: " << cam.theta << ", Phi: " << cam.phi << "\n";
	return output;
}
