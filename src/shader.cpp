#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

Shader::Shader(std::string vertex_name, std::string fragment_name) {
    std::string vertex_source;
    std::string fragment_source;

    source(vertex_source, vertex_name);
    source(fragment_source, fragment_name);
    process(shader_program_id_, vertex_source.c_str(), fragment_source.c_str());
}

void Shader::use() {
    glUseProgram(shader_program_id_);
}

void Shader::setBool(const std::string &name, bool value) {
    int uniform_location = glGetUniformLocation(shader_program_id_, name.c_str());
    glUniform1i(uniform_location, (int)value);
}

void Shader::setInt(const std::string &name, int value) {
    int uniform_location = glGetUniformLocation(shader_program_id_, name.c_str());
    glUniform1i(uniform_location, value);
}

void Shader::setFloat(const std::string &name, float value) {
    int uniform_location = glGetUniformLocation(shader_program_id_, name.c_str());
    glUniform1f(uniform_location, value);
}

Shader::~Shader() {
    std::cout << "Deleting program" << std::endl;
    glDeleteProgram(shader_program_id_);
}

void Shader::source(std::string &shader_source, std::string shader_name) {
    std::ifstream shader_file;

    std::string vertex_path = Shader::shader_location_ + shader_name;
    shader_file.open(vertex_path);

    if (!shader_file.is_open() && !shader_file.is_open()) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    std::stringstream vertex_shader_stream;
    vertex_shader_stream << shader_file.rdbuf();
    shader_file.close();

    shader_source = vertex_shader_stream.str();
}

void Shader::process(unsigned int &shader_program_id, const char *vertex_source, const char *fragment_source) {
    // create, source and compile vertex shader
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    // validate shader compilation
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // create, source and compile fragment shader
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    // validate shader compilation
    // int success;
    // char infoLog[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // link shaders to a shader program object
    // unsigned int shader_program_id;
    shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id_, vertex_shader);
    glAttachShader(shader_program_id_, fragment_shader);
    glLinkProgram(shader_program_id_);

    // validate shader program linkage
    // int success;
    // char info_log[512];
    glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program_id_, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << info_log << std::endl;
    }

    // delete shader objects
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

