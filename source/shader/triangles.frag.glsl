#version 410 core

out vec4 fColor;

in vec4 myVertex;
in vec3 myNormal;
in vec4 shadowCoords[10];

uniform int lightAmount;
uniform vec4 lightPositions[10];
uniform vec3 lightColors[10];

uniform mat4 lightSpaceMatrices[10];
uniform sampler2DShadow depthMaps[10];

uniform mat4 view;
uniform mat4 model;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main() {
    
    mat4 modelView = view * model;
    
    // Setup the final color first
    vec3 finalColor = ambient;
    
    // Calculate the transformed position
    vec3 vPos = vec3(modelView * myVertex);
    
    vec3 normal = normalize(vec3(transpose(inverse(modelView)) * vec4(myNormal, 0.0)));
    
    for (int i = 0; i < lightAmount; i++) {
    
        // Get Light Color
        vec3 lightColor = lightColors[i];
        vec4 lightSpace = modelView * lightPositions[i];
        
        // Calculate in and out direction
        bool isDirectLight = lightSpace.w == 0;
        vec3 lightPos = lightSpace.xyz;
        vec3 outDir = isDirectLight ? normalize(-lightPos) : normalize(lightPos - vPos);
        
        float visibility = texture(depthMaps[i], vec3(shadowCoords[i].xy, (shadowCoords[i].z - 0.005) / shadowCoords[i].w));
    
        vec3 inDir = normalize(-vPos);
        
        // Calculate diffuse and specular color
        vec3 halfAngle = normalize(inDir + outDir);
        vec3 diffuseColor = diffuse * max(dot(outDir, normal), 0);
        vec3 specularColor = specular * pow(max(dot(halfAngle, normal), 0), shininess);
        
        finalColor += lightColor * (diffuseColor + specularColor) * visibility;
    }

    fColor = vec4(finalColor, 1.0);
}
