/*光照正方体片段着色器**/
#version 330 core
//材质颜色
struct Material 
{
    vec3 ambient;   //环境光照颜色
    vec3 diffuse;   //漫反射光照颜色
    vec3 specular;  //镜面反射光照颜色
    float shininess;//反光度
}; 
//光照强度
struct Light 
{
    vec3 position;//灯光位置
    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射强度
    vec3 specular;
};

in vec3 worldPos;//世界坐标
in vec3 Normal; //模型空间下的法线

out vec4 FragColor;

uniform vec3 viewPos;   //观察者位置
uniform Material material;
uniform Light light;

void main()
{
    //环境光
    // float ambientStrength = 0.1f;//环境光光照强度
    vec3 ambient = light.ambient * material.ambient;
    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    //高光反射
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}