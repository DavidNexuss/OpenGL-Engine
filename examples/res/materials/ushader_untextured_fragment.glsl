#version 330 core

uniform vec3 uLightColor[6];
uniform vec3 uLightPosition[6];
uniform vec3 uViewPos;
uniform int uLightCount;
uniform samplerCube uSkybox;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float specularStrength;

uniform float reflectionStrength;

in vec3 fragPos;
in vec3 normal;
in vec3 color;

out vec3 fragColor;

vec3 lightPass(vec3 N,int i)
{
    vec3 L = normalize(uLightPosition[i] - fragPos);
    vec3 V = normalize(uViewPos - fragPos);
    
    //diffuse   
    float d = max(dot(L,N),0.0);

    vec3 reflectDir = reflect(-L,N);
    float s = pow(max(dot(V,reflectDir),0.0),specularStrength);

    return (s * ks + d * (kd + color) + ka) * uLightColor[i];
}

vec3 reflectPass(vec3 N)
{
    vec3 V = normalize(uViewPos - fragPos);
    vec3 R = reflect(V,N);
    return texture(uSkybox,-R).xyz;
}
void main()
{
    fragColor = vec3(0.0);
    vec3 N = normalize(normal);
    for(int i = 0; i < uLightCount; i++) {
        fragColor += lightPass(N,i);
    }

    fragColor = fragColor * (1.0 - reflectionStrength) + reflectPass(N) * reflectionStrength;
}