#pragma once
#include "../vendor/glad/glad.h"

class Texture {
public:
    Texture(int width, int height, int level, GLint internalFormat, GLenum format, GLenum type, const void* data);
    ~Texture();

    operator GLint() const { return tex; }

    void use(GLint textureUnit);

private:
    GLuint tex;
};