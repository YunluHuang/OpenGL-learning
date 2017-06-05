#version 410 core

out vec4 fColor;

in vec4 myVertex;
in vec3 myNormal;
in vec4 shadowCoord;

uniform int lightAmount;
uniform vec4 lightPositions[10];
uniform vec3 lightColors[10];

uniform mat4 lightSpaceMatrix;
uniform sampler2DShadow depthMap;

uniform mat4 view;
uniform mat4 model;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

vec3 lightPosition = vec3(-4.0f, 2.0f, 3.0f);

//float calcVisibility() {
//    
//    // perform perspective divide
//    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
//    
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(depthMap, projCoords.xy).r;
//    
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    
//    // calculate bias (based on depth map resolution and slope)
//    vec3 normal = normalize(myNormal);
//    vec4 lightSpace = modelView * vec4(lightPosition, 1.0f);
//    vec3 lightDir = normalize(lightSpace.xyz - vec3(modelView * myVertex));
//    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
//    
//    // check whether current frag pos is in shadow
//    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
//    // PCF
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
//    for (int x = -1; x <= 1; ++x) {
//        for (int y = -1; y <= 1; ++y) {
//            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
//            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
//        }
//    }
//    shadow /= 9.0;
//    
//    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
//    if (projCoords.z > 1.0)
//        shadow = 0.0;
//    
//    return shadow;
//}

void main() {
    
    mat4 modelView = view * model;
    
    // Setup the final color first
    vec3 finalColor = ambient;
    
    // Calculate the transformed position
    vec3 vPos = vec3(modelView * myVertex);
    
    vec3 normal = normalize(vec3(transpose(inverse(modelView)) * vec4(myNormal, 0.0)));
    
//    for (int i = 0; i < lightAmount; i++) {
    
        // Get Light Color
        vec3 lightColor = vec3(1, 1, 1);
        vec4 lightSpace = modelView * vec4(lightPosition, 1.0f);
        
        // Calculate in and out direction
        bool isDirectLight = lightSpace.w == 0;
        vec3 lightPos = lightSpace.xyz;
    vec3 outDir = isDirectLight ? normalize(lightPos) : normalize(lightPos - vPos);
    
    float bias = max(0.05 * (1.0 - dot(normal, outDir)), 0.005);
    float visibility = texture(depthMap, vec3(shadowCoord.xy, (shadowCoord.z - bias)) / shadowCoord.w);
    
        vec3 inDir = normalize(-vPos);
        
        // Calculate diffuse and specular color
        vec3 halfAngle = normalize(inDir + outDir);
        vec3 diffuseColor = diffuse * max(dot(outDir, normal), 0);
        vec3 specularColor = specular * pow(max(dot(halfAngle, normal), 0), shininess);
        
        finalColor += lightColor * (diffuseColor + specularColor) * visibility;
//    }
    
//    visibility = 0.2;
    fColor = vec4(finalColor, 1.0);
}
