#include <iostream>
#include <algorithm>
#include <vector>

#ifdef _WIN32
#include "GL/glew.h"
#endif
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "windows.h"
#include "shaderprogram.h"
#include "rotator.h"
#include "texture.h"

#include "cloth.h"


#define HEIGHT 1080
#define WIDTH 1920

int main() {

	std::cout << "====== Welcome to ======\n";
	std::cout << "===== Cloth simulation =====\n";
	std::cout << " \n";

	// Our first ingredience is time
	float currentTime=0.0f, deltaTime=0.0f, lastTime = 0.0f;
	GLFWwindow* window = nullptr;
	Windows  myWindow = Windows(window, WIDTH, HEIGHT, "Mass spring-cloth simulation");
	glm::vec3 clear_color = glm::vec3(0.3, 0.35, 0.6);

	// Controls
	MouseRotator rotator;
	rotator.init(window);

	// Create cloth
	Cloth cloth = Cloth(40, 40, 20, 20);

	glUseProgram(0);
	ShaderProgram standard_program("environment.vs", "", "", "", "environment.fs");
	ShaderProgram cloth_program("cloth.vs", "", "", "", "cloth.fs");

	Texture carpet("textures/buliao.jpg");

	GLint texLoc;

	while (!glfwWindowShouldClose(window))
	{// Init frame
		myWindow.InitFrame(clear_color);
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		rotator.poll(window);
//if you press the p it will stop

		if (!glfwGetKey(window, GLFW_KEY_P))
			cloth.updateSimulation(0.016, window);


		// Draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		cloth_program();
		cloth_program.updateCommonUniforms(rotator, WIDTH, HEIGHT, currentTime, clear_color);


		texLoc = glGetUniformLocation(cloth_program, "carpetTexture");
		glUniform1i(texLoc, 0);
		glActiveTexture(GL_TEXTURE0);
		carpet.bindTexture();
		cloth.render(window);

		// Finish frame
		glfwSwapInterval(0);
		glfwSwapBuffers(window);
		glfwPollEvents();


	} 

	glDisableVertexAttribArray(0);

	return 0;
}