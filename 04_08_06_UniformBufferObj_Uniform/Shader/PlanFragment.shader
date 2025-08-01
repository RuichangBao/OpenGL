
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    // gl_FragDepth = 0.005625;
    FragColor = texture(texture1, TexCoords);
}