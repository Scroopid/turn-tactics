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

Shady::CameraArcball::CameraArcball(GLuint max, GLFWwindow* window) {
	radius = 10;
	theta = 0;
	phi = 0;
	matrix_id = max;
	this->window = window;
	if(window != nullptr) {
		//glfwSetScrollCallback(window, this->scroll_callback);
	}
}

glm::vec3 Shady::CameraArcball::get_position() {
	return glm::vec3(position);
}

void Shady::CameraArcball::move(glm::vec3 add) {
	position += add;
}

void Shady::CameraArcball::radius_add(float add) {
	radius += add;
	if(radius < 1) radius = 1;
	else if(radius > 100) radius = 100;
}

void Shady::CameraArcball::theta_add(float add) {
	theta += add;
	while(theta < 0)
		theta += 2*PI;
	while(theta > 2*PI)
		theta -= 2*PI;
}

void Shady::CameraArcball::phi_add(float add) {
	if(phi + add >= PI / 2 || phi + add <= 0) return;
	phi += add;
}

void Shady::CameraArcball::adjust_angle(float theta_adder, float phi_adder) {
	theta_add(theta_adder);
	phi_add(phi_adder);
}

void Shady::CameraArcball::update(bool use_mouse_input) {

	if(use_mouse_input) {
		double cur_dif[2];
		double temp_x, temp_y;
		glfwGetCursorPos(window, &temp_x, &temp_y);
		cur_dif[0] = last_x - temp_x;
		cur_dif[1] = last_y - temp_y;
		last_x = temp_x; last_y = temp_y;

		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		double scroll2;
		GLFWscrollfun(window);
		if(state == GLFW_PRESS)
			adjust_angle(2 * PI * float(-cur_dif[0] / 1024), PI * float(-cur_dif[1] / 768));

		radius_add(-scroll);
		scroll = 0;
	}

	last[0] = position[0] + radius*cos(phi)*cos(theta);
	last[1] = position[1] + radius*sin(phi);
	last[2] = position[2] + radius*cos(phi) * sin(theta);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 View = glm::lookAt(last, position, up);

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 MVP = Projection * View * Model;

	glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &MVP[0][0]);
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

ENGINE_API std::ostream & Shady::operator<<(std::ostream & output, const CameraArcball& cam) {
	output << "X: " << cam.position[0] << ", Y: " << cam.position[1] << ", Z: " << cam.position[2] << "\n";
	output << "Radius: " << cam.radius << ", Theta: " << cam.theta << ", Phi: " << cam.phi << "\n";
	return output;
}

void Shady::CameraArcball::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scroll += yoffset;
}
