#include "shader.h"
#include "flog.h"
#include "string.h"
#include <stdexcept>

Shader::Shader(const char* vertSource, const char* fragSource) {
    // Create and compile vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    int vsl = strlen(vertSource);
    glShaderSource(vs, 1, &vertSource, &vsl);
    glCompileShader(vs);
    checkShader(vs);

    // Create and compile fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    int fsl = strlen(fragSource);
    glShaderSource(fs, 1, &fragSource, &fsl);
    glCompileShader(fs);
    checkShader(fs);

    // Link program
    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    checkProgram(prog);

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

GLuint Shader::attrib(const char* name) {
    // If value is already cached, return it
    if (attribs.find(name) != attribs.end()) {
        return attribs[name];
    }

    // Search for value and error out if not found
    GLint attrib = glGetAttribLocation(prog, name);
    if (attrib < 0) { throw std::runtime_error("Could not find attribute"); }

    // Save and return attribute
    attribs[name] = attrib;
    return attrib;
}

GLuint Shader::uniform(const char* name) {
    // If value is already cached, return it
    if (uniforms.find(name) != uniforms.end()) {
        return uniforms[name];
    }

    // Search for value and error out if not found
    GLint uniform = glGetUniformLocation(prog, name);
    if (uniform < 0) { throw std::runtime_error("Could not find attribute"); }

    // Save and return attribute
    uniforms[name] = uniform;
    return uniform;
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