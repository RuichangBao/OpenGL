#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, vData.texCoord);
}