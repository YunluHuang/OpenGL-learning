#version 410 core

out vec4 fColor;

in vec4 myVertex;
in vec3 myNormal;

in vec4 shadowCoords[5];
uniform int dirlgtAmount;
uniform vec3 dirlgtDirections[5];
uniform vec3 dirlgtColors[5];
uniform mat4 dirlgtMatrices[5];
uniform sampler2DShadow dirlgtMaps[5];

uniform int ptlgtAmount;
uniform vec3 ptlgtPositions[5];
uniform vec3 ptlgtColors[5];
uniform samplerCube ptlgtMaps[5];

uniform mat4 view;
uniform mat4 model;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main() {
    
    mat4 modelView = view * model;
    
    // Setup the final color first
    vec3 finalSpecular = vec3(0, 0, 0);
    vec3 finalDiffuse = vec3(0, 0, 0);
    
    // Calculate the transformed position
    vec4 _vPos = modelView * myVertex;
    vec3 vPos = _vPos.xyz / _vPos.w;
    
    vec4 _oriPos = model * myVertex;
    vec3 oriPos = _oriPos.xyz / _oriPos.w;
    
    //
    vec3 normal = normalize(vec3(transpose(inverse(modelView)) * vec4(myNormal, 0.0)));
    
    // First calculate all the direct light
    for (int i = 0; i < dirlgtAmount; i++) {
    
        // Get Light Color
        vec3 lightColor = dirlgtColors[i];
        vec4 lightSpace = modelView * vec4(dirlgtDirections[i], 0.0f);
        
        // Calculate in and out direction
        vec3 lightPos = lightSpace.xyz;
        vec3 inDir = normalize(-vPos);
        vec3 outDir = normalize(lightPos);
        
        // Calculate the shadow position with
        vec3 shadowPos = vec3(shadowCoords[i].xy, (shadowCoords[i].z - 0.005) / shadowCoords[i].w);
        float visibility = 1.0f; // texture(dirlgtMaps[i], shadowPos);
        
        // Calculate diffuse and specular color
        vec3 halfAngle = normalize(inDir + outDir);
        vec3 diffuseColor = diffuse * max(dot(outDir, normal), 0);
        vec3 specularColor = specular * pow(max(dot(halfAngle, normal), 0), shininess);
        
        // Add all these together
        finalSpecular += visibility * lightColor * specularColor;
        finalDiffuse += visibility * lightColor * diffuseColor;
    }
    
    // Then calculate all the point light
    for (int i = 0; i < ptlgtAmount; i++) {
        
        // Cache light color and light space
        vec3 lightColor = ptlgtColors[i];
        vec4 lightSpace = view * vec4(ptlgtPositions[i], 1.0f);
        
        // Calculate
        vec3 lightPos = lightSpace.xyz / lightSpace.w;
        vec3 inDir = normalize(-vPos);
        vec3 outDir = normalize(lightPos - vPos);
        
        vec3 halfAngle = normalize(inDir + outDir);
        vec3 diffuseColor = diffuse * max(dot(outDir, normal), 0);
        vec3 specularColor = specular * pow(max(dot(halfAngle, normal), 0), shininess);
        
        vec3 diff = oriPos - ptlgtPositions[i];
        float closestDepth = 25.0f * texture(ptlgtMaps[i], diff).r;
        float currentDepth = length(diff);
        float visibility = currentDepth > closestDepth ? 0.0f : 1.0f;
        
        float dist = sqrt(dot(diff, diff));
        float brightness = 8 / (dist * dist);
        
        // Add all these together
        finalSpecular += brightness * 1 * lightColor * specularColor;
        finalDiffuse += brightness * 1 * lightColor * diffuseColor;
    }

    fColor = vec4(ambient + finalSpecular + finalDiffuse, 1.0);
}
