/*片段着色器**/
#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

//阴影计算
float ShadowCalculation(vec3 fragPos)
{
    // 得到片段位置和光源位置之间的向量
    vec3 fragToLight = fragPos - lightPos;
    // 从深度图中提取片段到光向量
    float closestDepth = texture(depthMap, fragToLight).r;
    // 它目前在[0,1]之间的线性范围内，让我们将其重新转换回原始深度值
    closestDepth *= far_plane;
    // 将当前的线性深度作为片段和光源位置之间的长度
    float currentDepth = length(fragToLight);
    // 阴影测试
    float bias = 0.05; // 我们使用了更大的偏差，因为深度现在在[near_plane， far_plane]范围内
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
    // 可视化深度立方体图
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // 环境光
    vec3 ambient = 0.3 * lightColor;
    // 漫反射
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);//兰伯特光照
    vec3 diffuse = diff * lightColor;
    // 高光反射
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // 计算阴影
    float shadow = ShadowCalculation(fs_in.FragPos);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}