
#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;

uniform sampler2D texture1;

in VertexData {
    vec2 texCoord;
} vData;
void main()
{    
    FragColor = texture(texture1, vData.texCoord);
}