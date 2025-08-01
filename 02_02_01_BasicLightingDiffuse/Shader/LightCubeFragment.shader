/*光照正方体片段着色器**/
#version 330 core

in vec3 worldPos;
// in vec3 ModelPos;//模型空间坐标
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;//物体颜色
uniform vec3 lightColor;//灯光颜色
uniform vec3 lightPos;  //灯光位置

void main()
{
    //环境光
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);  
}