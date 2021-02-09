#version 330
#define MAX_LIGHTS 128

out vec4 FragColor;

struct DirectLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float cutOffOuter;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

uniform DirectLight directLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];
uniform Material material;
uniform vec3 viewPosition;

uniform int pointLightNo;
uniform int spotLightNo;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

vec3 calculateDirectLightWeight(vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-directLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient  = directLight.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = directLight.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = directLight.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 calculatePointLightSingle(PointLight pl, vec3 normal, vec3 viewDirection, vec3 fragPos) {
    vec3 lightDir = normalize(pl.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(pl.position - fragPos);
    float attenuation = 1.0 / (pl.constant + pl.linear * distance + pl.quadratic * (distance * distance));

    vec3 ambient  = pl.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = pl.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = pl.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculatePointLightsWeight(vec3 normal, vec3 viewDirection, vec3 fragPos) {
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < pointLightNo; i++){
        result += calculatePointLightSingle(pointLights[i], normal, viewDirection, fragPos);
    }

    return result;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 dirWeight = calculateDirectLightWeight(normal, viewDirection);
    vec3 plWeight = calculatePointLightsWeight(normal, viewDirection, FragPos);

    vec3 result = dirWeight + plWeight;
    FragColor = vec4(result, 1.0);
}
