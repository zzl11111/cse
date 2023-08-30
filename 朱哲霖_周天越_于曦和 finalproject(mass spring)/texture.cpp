#include "Texture.h"



Texture::Texture(const char* path) : width(100), height(100)
{
	generateTexture(path);
}

Texture::Texture(const char* path, int w, int h) : width(w), height(h)
{
	generateTexture(path);
}

Texture::~Texture()
{
}

void Texture::generateTexture(const char* path) {

	// Load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = stbi_load(path, &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindTexture() {
	glBindTexture(GL_TEXTURE_2D, texture);
}
