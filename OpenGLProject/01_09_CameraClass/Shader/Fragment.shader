/*片元着色器**/
#version 330 core

in vec2 TexCoord;
uniform float num;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 texture1Col = texture(texture1, TexCoord);
    vec4 texture2Col = texture(texture2, vec2(TexCoord.x, 1 - TexCoord.y));
    FragColor = mix(texture1Col, texture2Col, num);
}