#version 330 core
out vec4 FragColor;

in vec3 attrColor;

uniform vec4 uniColor;

void main() {
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    // FragColor = uniColor;
    FragColor = vec4(attrColor, 1.0f);
}
