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

GLuint Shader::getAttribute(const std::string& name) {
    // If value is already cached, return it
    if (attribs.find(name) != attribs.end()) {
        return attribs[name];
    }

    // Search for value and error out if not found
    GLint attrib = glGetAttribLocation(prog, name.c_str());
    if (attrib < 0) { flog::warn("Could not find attribute '{}'", name); }

    // Save and return attribute
    attribs[name] = attrib;
    return attrib;
}

GLuint Shader::getUniform(const std::string& name) {
    // If value is already cached, return it
    if (uniforms.find(name) != uniforms.end()) {
        return uniforms[name];
    }

    // Search for value and error out if not found
    GLint uniform = glGetUniformLocation(prog, name.c_str());
    if (uniform < 0) { flog::warn("Could not find uniform '{}'", name); }

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