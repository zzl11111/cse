#pragma once

#ifdef _WIN32
#include "GL/glew.h"
#endif

#include <GLFW/glfw3.h>

#include "stb_image.h"


class Texture
{
public:
	Texture(const char* path);
	Texture(const char* path, int w, int h);
	~Texture();
	void bindTexture();

protected:
	void generateTexture(const char* path);

private:
	int width, height, nrChannels;
	unsigned char* image;
	GLuint texture;
};

