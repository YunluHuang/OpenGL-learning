#version 410 core

out vec4 fColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
    fColor = texture(skybox, texCoords);
   // fColor = vec4(1, 1, 1, 1);
}
