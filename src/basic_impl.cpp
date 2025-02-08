#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>


const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 attrColor;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        attrColor = aColor;
    }
)";
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 attrColor;

    uniform vec4 uniColor;

    void main() {
        // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        // FragColor = uniColor;
        FragColor = vec4(attrColor, 1.0f);
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
        // triangle
        // positions             // colors
        // -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        //  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        //  0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f

        // square (two tringle without overlaps)
        // positions          // colors
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // quad. 1 - top-right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // quad. 4 - bottom-right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // quad. 3 - bottom-left
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f  // quad. 2 - top-left
    };

    unsigned int indices[] = {
        // triangle
        // 0, 1, 2

        // square
        0, 1, 3,
        1, 2, 3
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

    // gen and bind Element Buffer Object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set location and data format of vertex attributes (vertices[] linkage to shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // set render loop
    while (!glfwWindowShouldClose(window)) {
        // process inputs
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shader program
        glUseProgram(shaderProgram);

        // set uniform in each frame
        // float time_value = glfwGetTime();
        // float green_value = sin(time_value) / 2.0f + 0.5f;
        // int vertex_color_location = glGetUniformLocation(shaderProgram, "uniColor");
        // glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

        // bind VAO and set primitive
        glBindVertexArray(VAO);
        // used to draw without EBO
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // used to draw with EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

