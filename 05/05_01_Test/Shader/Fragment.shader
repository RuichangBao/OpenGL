/*片段着色器**/
#version 330 core

in vec4 FragColor;
out vec4 _fragColor;
void main()
{
   _fragColor = FragColor;
}