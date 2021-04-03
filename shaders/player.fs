#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform Material material;
uniform sampler2D playerTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int isTail;

void main()
{
    float ambientStrength = 0.3;
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = vec3(1.0);
    vec3 headColor = vec3(0.5, 0.8, 0.0);
    vec3 tailColor = vec3(0.8, 0.0, 0.0);

    if (isTail != 0){ // red for tail
        objectColor = tailColor;
    } else { // green for head
        objectColor = headColor;
    }
    // Ambient lighting
    vec3 ambient = lightColor * material.ambient;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize( lightPos - FragPos ); // Points from fragment to light
    float diff = max(dot(norm, lightDir) ,0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize( viewPos - FragPos ); // Points from fragment to viewer
    vec3 reflectDir = reflect (-lightDir, norm); // -lightDir points from light to fragment
    float spec = pow( max( dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}