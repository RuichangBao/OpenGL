#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
    if(gl_FrontFacing) 
    {
        FragColor = texture(frontTexture, TexCoords);
        // FragColor = vec4(0.5,0.5,0,1);
    }
    else
    {
        FragColor = texture(backTexture, TexCoords);
        // FragColor = vec4(0,0.5,0.5,1);
    }
    if(FragColor.a < 0.1)
        discard;
}