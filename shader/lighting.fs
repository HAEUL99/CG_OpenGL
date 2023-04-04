#version 330 core
in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;

//눈의 위치
uniform vec3 viewPos;


struct PointLight {
    vec3 position;
    //vec3 attenuation;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//uniform Light light;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading


    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    vec3 distPoly = vec3(1.0, distance, distance*distance);

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main() {
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - position);
    vec3 result = vec3(0.0);
    // phase 1: Directional lighting
    //vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < 2; i++)
    {
        result += CalcPointLight(pointLights[i], norm, position, viewDir);  
    }  
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    fragColor = vec4(result, 1.0);
}