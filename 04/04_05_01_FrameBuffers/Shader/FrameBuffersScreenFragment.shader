#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// void main()
// {    
//     //正常效果
//     // vec4 textCol = texture(screenTexture, TexCoords);
//     // FragColor = textCol;

//     //取反效果
//     // vec3 textCol = texture(screenTexture, TexCoords).xyz;
//     // FragColor = new vec4(1- textCol, 1);

//     //灰度效果
//     // vec4 textCol = texture(screenTexture, TexCoords);
//     // float average = (textCol.r + textCol.g + textCol.b) / 3.0;
//     // FragColor = vec4(average, average, average, 1.0);

//     //加权灰度效果
//     vec4 textCol = texture(screenTexture, TexCoords);
//     float average = 0.2126 * textCol.r + 0.7152 * textCol.g + 0.0722 * textCol.b;
//     FragColor = vec4(average, average, average, 1.0);
// }

//卷积效果
const float offset = 1.0 / 300.0;  
void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );
    //正常锐化
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );

    //模糊效果
    // float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16  
    // );

    //更加模糊效果(自己写着玩的)
    // float kernel[9] = float[](
    //     1.0 / 14, 2.0 / 14, 1.0 / 14,
    //     2.0 / 14, 2.0 / 14, 2.0 / 14,
    //     1.0 / 14, 2.0 / 14, 1.0 / 14  
    // );

    //边缘检测
    float kernel[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}