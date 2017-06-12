#version 410 core

in vec4 fragPos;

uniform vec3 lightPos;

void main() {
    
    float lightDistance = length(fragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / 25.0f;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}
