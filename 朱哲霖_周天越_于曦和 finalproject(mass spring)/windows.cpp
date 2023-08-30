#include "windows.h"
Windows::Windows(GLFWwindow * &window, unsigned int width, unsigned int height, const char* title) 
{
	if (!glfwInit()) {
		std::cerr << "glfw init failure" << std::endl;
		exit(-1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//define the version
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cerr << "glfw window create error" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwSetWindowPos(window, 250, 400);
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(-1);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


}
void Windows::InitFrame(glm::vec3 clear_color) {
	//INIT THE FRAME
	glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

}