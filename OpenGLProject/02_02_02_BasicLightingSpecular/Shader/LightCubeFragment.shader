/*光照正方体片段着色器**/
#version 330 core

in vec3 worldPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;//物体颜色
uniform vec3 lightColor;//灯光颜色
uniform vec3 lightPos;  //灯光位置
uniform vec3 viewPos;   //观察者位置

void main()
{
    //环境光
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //高光反射
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);  
}