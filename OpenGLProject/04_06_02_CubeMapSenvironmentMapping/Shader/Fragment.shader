#version 330 core
out vec4 FragColor;

in vec3 Normal;     //法线
in vec3 Position;   //世界位置

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    //反射
    // vec3 I = normalize(Position - cameraPos);   //观察方向
    // vec3 R = reflect(I, normalize(Normal));     //反射方向
    // FragColor = vec4(texture(skybox, R).rgb, 1.0);//输出颜色

    //折射
    float ratio = 1.00 / 1.52;//折射率
    vec3 I = normalize(Position - cameraPos);       //观察方向
    vec3 R = refract(I, normalize(Normal), ratio);  //折射方向
    FragColor = vec4(texture(skybox, R).rgb, 1.0);  //输出颜色
    
}