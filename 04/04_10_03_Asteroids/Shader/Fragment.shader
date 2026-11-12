/*片段着色器**/
#version 330 core

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main()
{
   FragColor = texture(texture_diffuse1, TexCoords);
}