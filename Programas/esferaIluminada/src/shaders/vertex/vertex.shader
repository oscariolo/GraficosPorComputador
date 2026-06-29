#version 330 core
//the position variable is specified to have its index at 0
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model = mat4(1.0);
//lighting
uniform float ambientStrenght = 1;
uniform vec3 lightSource = vec3(0.0f);
uniform vec3 lightColor = vec3(1.0f);
uniform vec3 cameraPos = vec3(1.0f);

out vec3 ourColor;

float specularStrength = 4;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);

    //World space

    vec3 FragPos = vec3(model * vec4(aPos, 1.0));

    //asumir es una esfera, la normal es desde el centro 0,0 hasta el punto
    vec3 normalSphere = normalize(mat3(model) * aPos);

    vec3 Normal = normalize(mat3(model) * aNormal);

    //ambiental

    vec3 ambientLight = ambientStrenght * lightColor;

    //difusa
    vec3 lightDir = normalize(lightSource - FragPos);
    float diff = max(dot(normalSphere, lightDir),0.0);
    vec3 diffuse = diff * lightColor;
    
    //specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,normalSphere);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  


    //phong
    vec3 result = (ambientLight + diffuse + specular) * aColor;

    ourColor = result;

}