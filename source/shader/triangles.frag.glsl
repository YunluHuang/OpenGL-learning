#version 410 core

out vec4 fColor;

in vec4 myVertex;
in vec3 myNormal;

uniform int lightAmount;
uniform vec4 lightPositions[10];
uniform vec3 lightColors[10];

uniform mat4 modelView;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main() {
    
    vec3 finalColor = ambient;
    
    vec3 vPos = vec3(modelView * myVertex);
    vec3 normal = normalize(vec3(transpose(inverse(modelView)) * vec4(myNormal, 0.0)));
    
    for (int i = 0; i < lightAmount; i++) {
        
        // Get Light Color
        vec3 lightColor = lightColors[i];
        
        // Calculate in and out direction
        bool isDirectLight = lightPositions[i].w == 0;
        vec3 lightPos = lightPositions[i].xyz;
        vec3 outDir = isDirectLight ? -lightPos : normalize(lightPos - vPos);
        
        outDir = normalize(vec3(transpose(inverse(modelView)) * vec4(outDir, 0.0)));
        
        vec3 inDir = normalize(-vPos);
        
        // Calculate diffuse and specular color
        vec3 halfAngle = normalize(inDir + outDir);
        vec3 diffuseColor = diffuse * max(dot(outDir, normal), 0);
        vec3 specularColor = specular * pow(max(dot(halfAngle, normal), 0), shininess);
        
        finalColor += lightColor * (diffuseColor + specularColor);
    }
    
    fColor = vec4(finalColor, 1.0);
}

