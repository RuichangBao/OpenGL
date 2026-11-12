/*片段着色器**/
#version 330 core
out vec4 FragColor;


//切线空间下的计算
in VS_OUT {
    vec3 FragPos;           //世界空间位置
    vec2 TexCoords;
    vec3 TangentLightPos;   //切线空间光源位置
    vec3 TangentViewPos;    //切线空间视角方向
    vec3 TangentFragPos;    //切线空间物体位置
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // //高度采样
    // float height = texture(depthMap, texCoords).r;
    // //高度越高 影响越大 高度越低，影响越小  
    // return texCoords - viewDir.xy * (height * heightScale);        

    // // 深度层的数量
    // const float numLayers = 10;
    // // 计算每一层的尺寸
    // float layerDepth = 1.0 / numLayers;
    // // 当前层的深度
    // float currentLayerDepth = 0.0;
    // // 每层调整纹理坐标所需的数值（基于向量 P）
    // vec2 P = viewDir.xy * heightScale; 
    // vec2 deltaTexCoords = P / numLayers;

    // // 获取初始值
    // vec2 currentTexCoords = texCoords;
    // float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
  
    // while(currentLayerDepth < currentDepthMapValue)
    // {
    //     // 沿 P 方向移动纹理坐标
    //     currentTexCoords -= deltaTexCoords;
    //     // 获取当前纹理坐标处的深度图值
    //     currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
    //     // 获取下一层的深度
    //     currentLayerDepth += layerDepth;  
    // }

    // return currentTexCoords;


    // 深度层的数量
    const float numLayers = 10;
    // 计算每一层的尺寸
    float layerDepth = 1.0 / numLayers;
    // 当前层的深度
    float currentLayerDepth = 0.0;
    // 每层调整纹理坐标所需的数值（基于向量 P）
    vec2 P = viewDir.xy * heightScale; 
    vec2 deltaTexCoords = P / numLayers;

    // 获取初始值
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
  
    while(currentLayerDepth < currentDepthMapValue)
    {
        // 沿 P 方向移动纹理坐标
        currentTexCoords -= deltaTexCoords;
        // 获取当前纹理坐标处的深度图值
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // 获取下一层的深度
        currentLayerDepth += layerDepth;  
    }
    // 在碰撞发生前获取纹理坐标（逆向操作）
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{       
    // 使用偏移纹理坐标技术结合视差映射
    // 视角方向
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
   
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
       discard;

    // 法线贴图法线信息
    vec3 normal = texture(normalMap, texCoords).rgb;
    //
    normal = normalize(normal * 2.0 - 1.0);  // 切线空间法线
   
    // 漫反射颜色
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // 环境光
    vec3 ambient = 0.1 * color;
    // 漫反射

    // 切线空间下的计算
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // 高光
    // 切线空间下的计算
    // vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}