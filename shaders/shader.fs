#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

float LinearizeDepth(float depth);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize( viewPos - FragPos ); // Points from fragment to viewer

    vec3 result = vec3(0.0);
    // Effect of direction light
    //result += CalcDirLight(dirLight, norm, viewDir);

    // Effect of point lights
    for(int i=0; i<NR_POINT_LIGHTS; i++)
      	result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    // Spot Light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
    //FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / 100.0), 1.0);
}

float LinearizeDepth(float depth)
{
    // Result ranges from near to far
    float near = 0.1;
    float far  = 100.0;

    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse
    float diff = max(dot(normal, lightDir) ,0);
    // Specular
    vec3 reflectDir = reflect (-lightDir, normal);
    float spec = pow( max( dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float attenuation = 1.0;
    vec3 lightDir = normalize(light.position - fragPos); // Points from fragment to light
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular
    vec3 reflectDir = reflect (-lightDir, normal);
    float spec = pow( max( dot(viewDir, reflectDir), 0.0), material.shininess);
    // Determine Attenuation
    float distance = length(light.position - fragPos);
    attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
    // Attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}