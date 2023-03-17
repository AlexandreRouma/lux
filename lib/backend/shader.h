#pragma once
#include "../vendor/glad/glad.h"
#include <string>
#include <map>

class Shader {
public:
    Shader(const char* vertSource, const char* fragSource);
    ~Shader();

    operator GLint() const { return prog; }
    GLuint attrib(const char* name);
    GLuint uniform(const char* name);
    
    void use();
    
private:
    static void checkShader(GLint shader);
    static void checkProgram(GLint program);

    GLint prog = -1;
    std::map<std::string, GLuint> attribs;
    std::map<std::string, GLuint> uniforms;
};