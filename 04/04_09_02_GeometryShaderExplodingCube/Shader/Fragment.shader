/*默认正方体片段着色器**/
#version 330 core

// in VS_OUT {
//     vec2 TexCoords;
//     vec3 Pos;
// } gs_in;
in vec2 gTexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffusel;

void main()
{
   // FragColor = texture(texture_diffusel, gs_in.TexCoords);
   FragColor = texture(texture_diffusel, gTexCoords);
}