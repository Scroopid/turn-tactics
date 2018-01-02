#include "CameraArcball.h"

bool Shady::CameraArcball::fix() {
	while(theta < 0)
		theta += PI * 2;
	while(theta > PI * 2)
		theta -= PI * 2;
	while(phi < 0)
		phi += PI / 2;
	while(phi > PI / 2)
		phi -= PI / 2;

	return false;
}

Shady::CameraArcball::CameraArcball(GLuint max, short theta_snaps, short phi_snaps) {
	radius = 10;
	theta = 0;
	phi = 0;
	theta_snap_amount = theta_snaps;
	phi_snap_amount = phi_snaps;
	matrix_id = max;
}

glm::vec3 Shady::CameraArcball::get_position() {
	return glm::vec3(position);
}

glm::vec3 Shady::CameraArcball::get_facing_vector() {
	glm::vec3 normal = glm::normalize(last - position);
	return normal;
}

glm::vec3 Shady::CameraArcball::get_facing_vector_XZ() {
	glm::vec3 newlast(last);
	newlast[1] = 0;
	glm::vec3 newposition(position);
	newposition[1] = 0;
	glm::vec3 normal = glm::normalize(newlast - newposition);
	return normal;
}

void Shady::CameraArcball::position_move(glm::vec3 add) {
	position += add;
}

void Shady::CameraArcball::position_move_facing(glm::vec3 add) {
	glm::vec3 facing = get_facing_vector_XZ();
	glm::vec3 forward = facing * add[2];
	facing = facing.zyx();
	facing[2] = -facing[2];
	glm::vec3 right = facing * add[0];
	position += forward;
	position -= right;
}

void Shady::CameraArcball::position_set(glm::vec3 values) {
	position = values;
}

void Shady::CameraArcball::radius_add(float add) {
	radius += add;
	if(radius < 1) radius = 1;
	else if(radius > 100) radius = 100;
}

void Shady::CameraArcball::radius_set(float amount) {
	radius = amount;
}

void Shady::CameraArcball::theta_add(float add) {
	theta += add;
	while(theta < 0)
		theta += 2*PI;
	while(theta > 2*PI)
		theta -= 2*PI;
}

void Shady::CameraArcball::theta_set(float angle) {
	theta = angle;
	fix();
}

void Shady::CameraArcball::phi_add(float add) {
	if(phi + add >= PI / 2 || phi + add <= 0) return;
	phi += add;
}

void Shady::CameraArcball::phi_set(float angle) {
	phi = angle;
	fix();
}

void Shady::CameraArcball::adjust_angle(float theta_adder, float phi_adder) {
	theta_add(theta_adder);
	phi_add(phi_adder);
}

void Shady::CameraArcball::snap_left() {
	float size = (PI * 2.0 / theta_snap_amount);
	float temp_cur = (theta / size);
	int cur = within(temp_cur, 0.01, (int)temp_cur) ? temp_cur : temp_cur + 0.1;
	theta = (cur + 1) * size;
	fix();
}

void Shady::CameraArcball::snap_right() {
	float size = (PI * 2.0 / theta_snap_amount);
	float temp_cur = (theta / size);
	int cur = within(temp_cur, 0.01, (int) temp_cur) ? temp_cur : temp_cur + 0.1;
	theta = (cur - 1) * size;
	fix();
}

void Shady::CameraArcball::snap_up() {
	float size = (PI / 2.0 / phi_snap_amount);
	float temp_cur = (phi / size);
	int cur = within(temp_cur, 0.01, (int) temp_cur) ? temp_cur : temp_cur + 0.1;
	if(cur >= phi_snap_amount) return;
	phi = (cur + 1) * size;
	fix();
}

void Shady::CameraArcball::snap_down() {
	float size = (PI / 2.0 / phi_snap_amount);
	float temp_cur = (phi / size);
	int cur = within(temp_cur, 0.01, (int) temp_cur) ? temp_cur : temp_cur + 0.1;
	if(cur <= 0) return;
	phi = (cur - 1) * size;
	fix();
}

void Shady::CameraArcball::update(double cursor_x, double cursor_y, double scroll_y) {

	double cur_dif[2];
	if(last_x < 0) last_x = cursor_x;
	if(last_y < 0) last_y = cursor_y;
	if(cursor_x >= 0) { cur_dif[0] = last_x - cursor_x; last_x = cursor_x; } else { cur_dif[0] = 0; last_x = -1; }
	if(cursor_x >= 0) { cur_dif[1] = last_y - cursor_y; last_y = cursor_y; } else { cur_dif[1] = 0; last_y = -1; }

	adjust_angle(2 * PI * float(-cur_dif[0]), PI * float(-cur_dif[1]));

	radius_add(-scroll_y);

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

Shady::CameraArcball Shady::CameraArcball::operator+=(const glm::vec3& add) {
	position_move_facing(add);
	return *this;
}

Shady::CameraArcball Shady::CameraArcball::operator-=(const glm::vec3& add) {
	position_move_facing(-add);
	return *this;
}

ENGINE_API std::ostream & Shady::operator<<(std::ostream & output, const CameraArcball& cam) {
	output << "X: " << cam.position[0] << ", Y: " << cam.position[1] << ", Z: " << cam.position[2] << "\n";
	output << "Radius: " << cam.radius << ", Theta: " << cam.theta << ", Phi: " << cam.phi << "\n";
	return output;
}
