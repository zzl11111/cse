#ifndef WINDOWS_H
#define WINDOWS_H
#include <iostream>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class Windows {
public:
	Windows(GLFWwindow* &window, unsigned int width, unsigned int height, const char* title);
	void InitFrame(glm::vec3 clear_color);
	~Windows() {

	}
};
#endif