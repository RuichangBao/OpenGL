/*片段着色器**/
#version 330 core
out vec4 FragColor;

//模型空间下的计算
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 ModelLightPos;
    vec3 ModelViewPos;
} fs_in;

//切线空间下的计算
// in VS_OUT {
//     vec3 FragPos;
//     vec2 TexCoords;
//     vec3 TangentLightPos;
//     vec3 TangentViewPos;
//     vec3 TangentFragPos;
// } fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
    // 法线贴图法线信息
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    //
    normal = normalize(normal * 2.0 - 1.0);  // 切线空间法线
   
    // 漫反射颜色
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // 环境光
    vec3 ambient = 0.1 * color;
    // 漫反射
    // 模型空间下的计算
    vec3 lightDir = normalize(fs_in.ModelLightPos - fs_in.FragPos);
    // 切线空间下的计算
    // vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    // 模型空间下的计算
    vec3 viewDir = normalize(fs_in.ModelViewPos - fs_in.FragPos);
    // 切线空间下的计算
    // vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}