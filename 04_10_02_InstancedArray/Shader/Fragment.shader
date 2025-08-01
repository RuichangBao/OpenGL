/*片段着色器**/
#version 330 core

in vec3 fColor;

out vec4 FragColor;

void main()
{
   FragColor = vec4(fColor, 1);
}