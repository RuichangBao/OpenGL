/*片段着色器**/
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{
   vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
   // 环境光
   vec3 ambient = 0.05 * color;
   // 漫反射
   vec3 lightDir = normalize(lightPos - fs_in.FragPos);
   vec3 normal = normalize(fs_in.Normal);
   float diff = max(dot(lightDir, normal), 0.0);
   vec3 diffuse = diff * color;
   // 高光
   vec3 viewDir = normalize(viewPos - fs_in.FragPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = 0.0;
   if(blinn)//布林-冯模型
   {
      vec3 halfwayDir = normalize(lightDir + viewDir);  
      spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
   }
   else  //冯模型
   {
      vec3 reflectDir = reflect(-lightDir, normal);
      spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
   }
   vec3 specular = vec3(0.3) * spec; // 假设明亮的白色光的颜色
   FragColor = vec4(ambient + diffuse + specular, 1.0);
}