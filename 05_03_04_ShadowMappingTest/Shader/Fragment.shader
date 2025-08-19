/*片段着色器**/
#version 330 core

in vec2 TexCoord;

uniform sampler2D ourTexture;
out vec4 FragColor;

void main()
{           
    vec3 color = texture(ourTexture, TexCoord).xyz;
    FragColor = vec4(color, 1.0);
}
