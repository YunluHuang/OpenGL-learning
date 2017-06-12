#version 410 core

out vec4 fCol;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
    fCol = texture(skybox, TexCoords);
}
