/*默认正方体片段着色器**/
#version 330 core

in vec2 gTexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffusel;

void main()
{
   FragColor = texture(texture_diffusel, gTexCoords);
}