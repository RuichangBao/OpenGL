/*聚光灯片段着色器**/
#version 330 core

//材质颜色
struct Material {
    sampler2D diffuse;//漫反射贴图
    sampler2D specular;  //镜面反射光照颜色
    float shininess;//反光度
};

//方向光
struct DirLight {
    vec3 direction;//光源的方向向量，由光源指向物体
    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//高光反射强度
};

//点光源
struct PointLight {
    vec3 position;//点光源位置
    //光照衰减
    float constant; //常数项
    float linear;   //一次项
    float quadratic;//二次项
    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//高光强度
};

//聚光灯
struct SpotLight{
    vec3 position; //点光源位置
    vec3 direction; //灯光方向 由灯光指向物体
    float cutOff;   //内圆锥光圈一半的正弦值
    float outerCutOff;//外圆锥光圈一半的正弦值
    //光照强度
    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//高光强度
    //光照衰减
    float constant; //常数项
    float linear;   //一次项
    float quadratic;//二次项
};

#define NR_POINT_LIGHTS 4

in vec3 worldPos;//世界坐标
in vec3 Normal; //模型空间下的法线
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos; //观察者位置
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//平行光计算
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//点光源计算
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  //聚光灯计算

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - worldPos);
    // 平型光
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, worldPos, viewDir);    
    // 聚光灯
    result += CalcSpotLight(spotLight, norm, worldPos, viewDir);    

    FragColor = vec4(result, 1.0);
}

// 计算方向光颜色
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);//光源位置，由物体指向光源
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 高光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并计算
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

//点光源计算
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 高光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //合并计算
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

//聚光灯计算
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    //高光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //内外光圈计算
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //合并计算
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}