#include "shader.h"
#include "flog.h"
#include <stdexcept>

Shader::Shader(const std::string& vertSource, const std::string& fragSource) {
    // Create and compile vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char* vss = vertSource.c_str();
    int vsl = vertSource.size();
    glShaderSource(vs, 1, &vss, &vsl);
    glCompileShader(vs);
    checkShader(vs);

    // Create and compile fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fss = fragSource.c_str();
    int fsl = fragSource.size();
    glShaderSource(fs, 1, &fss, &fsl);
    glCompileShader(fs);
    checkShader(fs);

    // Link program
    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    checkProgram(prog);

    // Get attributes
    pos = glGetAttribLocation(prog, "pos");
    if (pos < 0) {
        flog::error("Could not find position attribute");
        throw std::runtime_error("Could not find position attribute");
    }

    // Delete individual shaders
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    glDeleteProgram(prog);
}

void Shader::use() {
    glUseProgram(prog);
}

GLint Shader::getPosAttribute() {
    return pos;
}

void Shader::checkShader(GLint shader) {
    GLint success;
    char cLogs[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, cLogs);
        flog::error("Shader compilation failed: {0}", cLogs);
        throw std::runtime_error("Could not compile shader");
    }
}

void Shader::checkProgram(GLint program) {
    GLint success;
    char cLogs[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, cLogs);
        flog::error("Program linking failed: {0}", cLogs);
        throw std::runtime_error("Could not link shader");
    }
}