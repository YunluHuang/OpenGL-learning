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

vec2 poissonDisk[16] = vec2[] (
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543, 0.27676845 ),
    vec2( 0.97484398, 0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ), 
    vec2( 0.79197514, 0.19090188 ), 
    vec2( -0.24188840, 0.99706507 ), 
    vec2( -0.81409955, 0.91437590 ), 
    vec2( 0.19984126, 0.78641367 ), 
    vec2( 0.14383161, -0.14100790 ) 
);

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
        for (int j = 0; j < 9; j++) {
            vec2 off = poissonDisk[j];
            float closestDepth = 25.0f * texture(ptlgtMaps[i], diff + 0.001 / invsq * u * off.x + 0.001 / invsq * v * off.y).r;
            if (currentDepth > closestDepth) {
                shadow += 1.0f;
            }
        }
        shadow /= 9.0f;
        float visibility = 1 - shadow;
        
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
