#include "texture.h"

Texture::Texture(int width, int height, int level, GLint internalFormat, GLenum format, GLenum type, const void* data) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // TODO: Parameters
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    glDeleteTextures(1, &tex);
}

void Texture::use(GLint textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, tex);
}