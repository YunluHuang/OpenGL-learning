#version 410 core

out vec4 fColor;

uniform int lightAmount;
uniform vec4 lightPositions[10];
uniform vec3 lightColors[10];

void main() {
    fColor = vec4(0.0, 0.0, 1.0, 1.0);
}

//in vec3 myNormal;
//in vec4 myVertex;
//
//// You will certainly need this matrix for your lighting calculations
//uniform mat4 modelview;
//
//// This first defined output of type vec4 will be the fragment color
//out vec4 fragColor;
//
//uniform vec3 color;
//
//uniform vec4 lightposn[numLights]; // positions of lights
//uniform vec3 lightcolor[numLights]; // colors of lights
//uniform int numused;               // number of lights used
//
//// Now, set the material parameters.
//// I use ambient, diffuse, specular, shininess.
//// But, the ambient is just additive and doesn't multiply the lights.
//
//uniform vec3 ambient;
//uniform vec3 diffuse;
//uniform vec3 specular;
////uniform vec4 emission;
//uniform float shininess;
//
//void main (void)
//{
//    vec4 finalcolor;
//    
//    vec3 sum = vec3(0.0, 0.0, 0.0);
//    vec3 eyepos = vec3(0.0, 0.0, 0.0);
//    vec3 vPos = (modelview * myVertex).xyz / (modelview * myVertex).w;
//    vec3 normal = normalize((transpose(inverse(modelview)) * vec4(myNormal, 0)).xyz);
//    int num = 0;
//    for (int i = 0; i < numused; i++) {
//        vec3 lightdir;
//        if (lightposn[i].w != 0) {
//            vec3 lightpos= lightposn[i].xyz / lightposn[i].w;
//            lightdir = normalize(lightpos - mypos);
//        } else {
//            vec3 lightpos = lightposn[i].xyz;
//            lightdir = normalize(lightpos);
//        }
//        vec3 eyedir = normalize(eyepos - mypos);
//        vec3 halfangle = normalize(lightdir + eyedir);
//        vec4 tmp = diffuse * max(dot(normal, lightdir), 0)
//        + specular * pow(max(dot(normal, halfangle),0), shininess);
//        sum = sum + lightcolor[i] * tmp;
//        num++;
//    }
//    vec4 I = ambient + emission + sum;
//    // Color all pixels black for now, remove this in your implementation!
//    finalcolor = I;
//    
//    fragColor = finalcolor;
//}
