/*光照正方体片段着色器**/
#version 330 core
//材质颜色
struct Material {
    sampler2D diffuse;//漫反射贴图
    sampler2D specular;  //镜面反射光照颜色
    float shininess;//反光度
}; 
//光照强度
struct Light {
    vec3 position;//灯光位置
    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射强度
    vec3 specular;
};

in vec3 worldPos;//世界坐标
in vec3 Normal; //模型空间下的法线
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform vec3 viewPos;   //观察者位置
uniform Light light;

void main()
{
    //环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - worldPos);//光源方向，由物体指向光源
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));//通过漫反射贴图设置颜色
   
    //高光反射
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
  }