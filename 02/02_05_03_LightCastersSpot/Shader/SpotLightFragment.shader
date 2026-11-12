/*聚光灯片段着色器**/
#version 330 core
//材质颜色
struct Material {
    sampler2D diffuse;//漫反射贴图
    sampler2D specular;  //镜面反射光照颜色
    float shininess;//反光度
}; 
//光照强度
struct Light {
    vec3 position; //点光源位置
    vec3 direction; //灯光方向 由灯光指向物体
    float cutOff;   //光圈一半的正弦值
    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//高光强度
    //光照衰减
    float constant; //常数项
    float linear;   //一次项
    float quadratic;//二次项
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
    float distance = length(light.position - worldPos);//光照距离
    float attenuation = 1.0 / (light.constant + light.linear * distance +  light.quadratic * (distance * distance));
    //环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 lightDir = normalize(light.position - worldPos);//光源方向，由物体指向光源
    float theta = dot(lightDir, normalize(-light.direction));
    if(theta > light.cutOff) 
    {       
        //漫反射
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));//通过漫反射贴图设置颜色
    
        //高光反射
        vec3 viewDir = normalize(viewPos - worldPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

        FragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0);
    }
    else  // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
    {  
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    }
  }