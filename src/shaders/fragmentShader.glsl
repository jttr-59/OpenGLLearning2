#version 330 core

struct Material {
    float shininess;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in vec2 TexCoords;

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

#define NR_DIR_LIGHTS 1
uniform DirLight dirLights[NR_DIR_LIGHTS];

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform Material material;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLights[0], norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.01), material.shininess);

    vec3 ambient  = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.01), material.shininess);

    float distance = length(light.position - FragPos);
    float attuenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    ambient *= attuenuation;
    diffuse *= attuenuation;
    specular *= attuenuation;
    return (ambient + diffuse + specular);

}

// OLD LIGHTING CODE
/*
void main()
{

    vec3 ambient  = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.01), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    float distance = length(light.position - FragPos);
    float attuenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    ambient *= attuenuation;
    diffuse *= attuenuation;
    specular *= attuenuation;


    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
*/