#pragma once
#include "../vendor/glad/glad.h"
#include <string>
#include <map>

class Shader {
public:
    Shader(const std::string& vertSource, const std::string& fragSource);
    ~Shader();

    operator GLint() const { return prog; }
    GLuint getAttribute(const std::string& name);
    GLuint getUniform(const std::string& name);
    
    void use();
    
private:
    static void checkShader(GLint shader);
    static void checkProgram(GLint program);

    GLint prog = -1;
    std::map<std::string, GLuint> attribs;
    std::map<std::string, GLuint> uniforms;
};