#include "shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertex_path, const char *fragment_path) {
    std::string vertex_code_str;
    std::string fragment_code_str;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.open(vertex_path);
    fragment_shader_file.open(fragment_path);

    if (!vertex_shader_file.is_open() && 
        !fragment_shader_file.is_open()) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    std::stringstream vertex_shader_stream;
    std::stringstream fragment_shader_stream;

    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();

    vertex_shader_file.close();
    fragment_shader_file.close();

    vertex_code_str = vertex_shader_stream.str();
    fragment_code_str = fragment_shader_stream.str();

    const char* vertex_shader_code = vertex_code_str.c_str();
    const char* fragment_shader_code = fragment_code_str.c_str();

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
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
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
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
    // unsigned int shader_program_id_;
    shader_program_id_ = glCreateProgram();
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

