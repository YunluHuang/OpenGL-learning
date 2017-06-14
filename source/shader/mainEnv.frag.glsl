#version 410 core

out vec4 fColor;

in vec4 myVertex;
in vec3 myNormal;

in vec4 shadowCoords[5];
uniform int dirlgtAmount;
uniform vec3 dirlgtDirections[5];
uniform vec3 dirlgtColors[5];
uniform mat4 dirlgtMatrices[5];
uniform sampler2D dirlgtMaps[5];

uniform int ptlgtAmount;
uniform vec3 ptlgtPositions[5];
uniform vec3 ptlgtColors[5];
uniform samplerCube ptlgtMaps[5];

uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 model;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform samplerCube envMap;
uniform samplerCube irrMap;

uniform int width;
uniform int height;
uniform sampler2D ssao;

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
        vec3 shadowPos = vec3(shadowCoords[i].xy, (shadowCoords[i].z) / shadowCoords[i].w);
        float visibility = 1; // texture(dirlgtMaps[i], shadowPos);
        
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
        vec3 u = cross(normal, vec3(1, 0, 0));
        u = normalize(u == vec3(0, 0, 0) ? cross(normal, vec3(0, 1, 0)) : u);
        vec3 v = normalize(cross(normal, u));
        float currentDepth = length(diff);
        float invsq = 1 / (currentDepth * currentDepth);
        float shadow = 0.0f;
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                float closestDepth = 25.0f * texture(ptlgtMaps[i], diff + 0.001 / invsq * u * j + 0.001 / invsq * v * k).r;
                if (currentDepth > closestDepth) {
                    shadow += 1.0f;
                }
            }
        }
        shadow /= 9.0f;
        float visibility = 1.0f;
        
        float dist = sqrt(dot(diff, diff));
        float brightness = 8 / (dist * dist);
        
        // Add all these together
        finalSpecular += brightness * visibility * lightColor * specularColor;
        finalDiffuse += brightness * visibility * lightColor * diffuseColor;
    }
    
    vec3 env = vec3(0, 0, 0);
    vec3 eyedir = normalize(-vPos);
    vec3 envdir = vec3(inverse(view) * vec4(2 * normal * dot(eyedir, normal) - eyedir, 0.0f));
    vec3 specularEnv = specular * texture(envMap, envdir).xyz;
    vec3 diffuseEnv = diffuse * texture(irrMap, envdir).xyz;
    env = specularEnv + diffuseEnv;
    
    vec2 ssaoCoord = vec2(gl_FragCoord.x / width, gl_FragCoord.y / height);
    float ao = texture(ssao, ssaoCoord).r;
    
    fColor = vec4(ambient + finalSpecular + finalDiffuse + env, 1.0) * ao;
}
