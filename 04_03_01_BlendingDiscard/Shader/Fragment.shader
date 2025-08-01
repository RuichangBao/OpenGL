#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    vec4 textCol = texture(texture1, TexCoords);
    if(textCol.a < 0.1)
        discard;
    FragColor = textCol;
}