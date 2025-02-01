#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

void framebuffer_size_callback(GLFWwindow* window, int with, int height) {
    glViewport(0, 0, with, height);
}

void preccess_input_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, true);
    std::cout << "input callback!" << std::endl;
    printf("key: %d | scancode: %d | action: %d | mode: %d\n", key,  scancode, action, mods);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create OpenGL context and make it the current
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glew
    if(glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // set viewport
    glViewport(0, 0, 800, 600);

    // set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetKeyCallback(window, preccess_input_callback);
    
    // OpenGL Objects 
    // Objects are manage following a pattern similar to this:
    // 1. Declare an `unsigned int ref`
    // 2. Associate an `unsigned int ref` to a new object type
    // 3. Feed object with the data
    // For tipical objects:
    //   unsigned int ref (ref) -> GenObject (gen) -> BindObject (bind) -> ObjectData (feed)
    //   RGBF => Ref -> Gen -> Bind -> Feed
    // For shader objects:
    //   unsigned int ref (ref) -> CreateShader (create) -> ShaderSource (feed) -> CompileShader (compile)
    //   RCFC => Ref -> Create -> Feed - Compile
    // For shader program objects:
    //   unsigned int ref (ref) -> CreateProgram (create) -> AttachShader x2 (attach) -> LinkProgram (link)
    //   RCAL => Ref -> Create -> Attach -> Link

    // create, source and compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // validate shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // create, source and compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // validate shader compilation
    // int success;
    // char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders to a shader program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // validate shader program linkage
    // int success;
    // char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << infoLog << std::endl;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // gen and bind Vertex Array Object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // gen and bind Vertex Buffer Object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set location and data format of vertex attributes (vertices[] linkage to shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // unbind objects
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set render loop
    while (!glfwWindowShouldClose(window)) {
        // process inputs
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shader program, bind VAO and set primitive
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

