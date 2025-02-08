#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>

class Shader {
public:
    unsigned int shader_program_id_;
    const std::string shader_location_ = "../src/shaders/";

    Shader(std::string vertex_path, std::string fragment_path);

    void use();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);

    virtual ~Shader();
private:
    void source(std::string &shader_source, std::string shader_name);
    void process(unsigned int &shader_program, const char* vertex_shader, const char* fragment_shader);
};

#endif
