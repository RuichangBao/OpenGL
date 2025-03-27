//帧缓冲 片段着色器
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{    
    //正常效果
    // vec4 textCol = texture(screenTexture, TexCoords);
    // FragColor = textCol;

    //取反效果
    // vec3 textCol = texture(screenTexture, TexCoords).xyz;
    // FragColor = new vec4(1- textCol, 1);

    //灰度效果
    // vec4 textCol = texture(screenTexture, TexCoords);
    // float average = (textCol.r + textCol.g + textCol.b) / 3.0;
    // FragColor = vec4(average, average, average, 1.0);

    //加权灰度效果
    vec4 textCol = texture(screenTexture, TexCoords);
    float average = 0.2126 * textCol.r + 0.7152 * textCol.g + 0.0722 * textCol.b;
    FragColor = vec4(average, average, average, 1.0);
}