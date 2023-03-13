#pragma once
#include "../vendor/glad/glad.h"
#include <string>

class Shader {
public:
    Shader(const std::string& vertSource, const std::string& fragSource);
    ~Shader();

    operator GLint() const { return prog; }
    
    void use();
    GLint getPosAttribute();

private:
    static void checkShader(GLint shader);
    static void checkProgram(GLint program);

    GLint prog = -1;
    GLint pos = -1;
};