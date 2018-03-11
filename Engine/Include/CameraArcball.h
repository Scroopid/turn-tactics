#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define PI 3.1415

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <GLFW\glfw3.h>

namespace Shady {
	class ENGINE_API CameraArcball {
	private:
		GLuint matrix_id; //this is the thing that keeps track of camera's matrix id stuff and is required
		float radius;//this represents the radius of the arcball(how far from the look-at-point we are looking at we are)
		float theta;//this is the X position of the arcball(horizontal angle from the look-at-point)
		float phi;//this is the y position of the arcball(vertical angle from the look-at-point)
		glm::vec3 position { 0,0,0 };//the point in space we are looking at (look-at-point)
		glm::vec3 up { 0,1,0 };//the directon of the top of the camera( {0,1,0} is normal camera, {0,-1,0} would make the camera upside down)
		glm::vec3 last { 0,0,0 };//this is where the calculations for the cameras position are stored (updates when update is called)
		double last_x = 0;//last mouse X position IF CAMERA IS MOVING otherwise its -1.
		double last_y = 0;//last mouse Y position IF CAMERA IS MOVING otherwise its -1.
		short theta_snap_amount;//amount of snapping positions for theta.
		short phi_snap_amount;//amount of snapping positions for phi.
		float snap_speed;
		bool fix();
	public:
		const double default_move_speed = 0.05;
		/**
		this class does cool arcball camera math for you.

		@param max Matrix Id
		@param theta_snaps amoutn of snap points for theta
		@param phi_snaps amount of snap points for phi
		*/
		CameraArcball(GLuint, short = 16, short = 8);
		glm::vec3 get_position();
		/**
		gets the vector of the direction the camera is facing
		*/
		glm::vec3 get_facing_vector();
		glm::vec3 get_facing_vector_XZ();
		/**
		Moves the look-at-point adding the given values

		@param add a glm::vec3 to add to the current look-at-point
		*/
		void position_move(glm::vec3 add);
		void position_move_facing(glm::vec3 add);
		/**
		sets the look-at-point with the given values

		@param values a glm::vec3 point to set the look-at-point to.
		*/
		void position_set(glm::vec3 values);
		/**
		Adds to the Radius (distance to look-at-point).

		@param add the amount of opengl units to move away.
		*/
		void radius_add(float add);
		/**
		Sets the Radius (distance to look-at-point).

		@param amount of opengl units to be from the look-at-point.
		*/
		void radius_set(float amount);
		/**
		Adds to the Theta value changing the arcball view poition horizaltal

		@param add amount to add to Theda (horizaltal angle)
		*/
		void theta_add(float add);
		/**
		sets the Theta value changing the arcball view poition horizaltal

		@param anlge sets Theta (horizaltal angle) in radians
		*/
		void theta_set(float angle);
		/**
		Adds to the Phi value changing the arcball view poition vertical

		@param add amount to add to Phi (vertical angle)
		*/
		void phi_add(float add);
		/**
		sets the Phi value changing the arcball view poition vertical

		@param anlge sets Phi (vertical angle) in radians
		*/
		void phi_set(float angle);
		/**
		Adds to the Theta and Phi values changing the arcball view poition

		@param theta_adder amount to add to Theda (horizaltal angle)
		@param phi_adder amount to add to Phi (vertical angle)
		*/
		void adjust_angle(float theta_adder, float phi_adder);
		/**
		goes to the next left snap point around the look-at-point on theta
		*/
		void snap_left();
		/**
		goes to the next right snap point around the look-at-point on theta
		*/
		void snap_right();
		/**
		goes to the next upper snap point around the look-at-point on phi
		*/
		void snap_up();
		/**
		goes to the next downward snap point around the look-at-point on phi
		*/
		void snap_down();
		/**
		Updates camera position based on Radius, Phi, and Theta.

		@param cursor_x the UNV postion of the of the X coordinate of the cursor (0-1) -1 tells it not not update Theta 
		@param cursor_y the UNV postion of the of the Y coordinate of the cursor (0-1) -1 tells it not not update Phi
		@param scroll_y raw change of Y axis of the mouse scroll wheel, changes distance from loot at point.
		*/
		void update(double cursor_x = -1.0, double cursor_y = -1.0, double scroll_y = 0.0);
		/**
		Updates camera position based on Radius, Phi, and Theta. using the input from the window used for testing.
		*/
		void update(GLFWwindow* window);
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		/**
		See position_move(glm::vec3 add)
		*/
		CameraArcball operator+=(const glm::vec3&);
		/**
		See position_move(glm::vec3 add) but this subtracts instead of adding.
		*/
		CameraArcball operator-=(const glm::vec3&);
		/**
		outputs the important debug values.
		*/
		friend ENGINE_API std::ostream &operator<<(std::ostream& output, const CameraArcball& cam);
	};
}

/*
BASIC SETUP:

#define move_speed 0.05
Shady::CameraArcball cam = NULL;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.radius_add(-yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		cam.snap_right();
	}else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		cam.snap_left();
	} else if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
		cam.snap_up();
	} else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cam.snap_down();
	}
}

...

GLuint MatrixID = glGetUniformLocation(programID, "MVP");

...

glfwSetScrollCallback(window, scroll_callback);
glfwSetKeyCallback(window, key_callback);
cam = Shady::CameraArcball(MatrixID);
cam.update();

LOOP{
	double temp_x, temp_y;
	glfwGetCursorPos(window, &temp_x, &temp_y);

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if(state == GLFW_PRESS)
		cam.update(temp_x / 1024, temp_y / 768, 0);
	else
		cam.update(-1, -1, 0);

	glm::vec3 temp {0,0,0};
	state = glfwGetKey(window, GLFW_KEY_E);
	if(state == GLFW_PRESS)
		temp[1] -= move_speed;
	state = glfwGetKey(window, GLFW_KEY_Q);
	if(state == GLFW_PRESS)
		temp[1] += move_speed;
	state = glfwGetKey(window, GLFW_KEY_W);
	if(state == GLFW_PRESS)
		temp[0] -= move_speed;
	state = glfwGetKey(window, GLFW_KEY_S);
	if(state == GLFW_PRESS)
		temp[0] += move_speed;
	state = glfwGetKey(window, GLFW_KEY_A);
	if(state == GLFW_PRESS)
		temp[2] += move_speed;
	state = glfwGetKey(window, GLFW_KEY_D);
	if(state == GLFW_PRESS)
		temp[2] -= move_speed;

	cam += temp;
}

*/