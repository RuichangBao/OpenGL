#version 330 core

void main()
{
    // 生成三角形网格
    int triID = gl_VertexID / 3;  // 三角形ID
    int vertID = gl_VertexID % 3; // 三角形内顶点ID(0-2)
    
    vec2 pos;
    if(vertID == 0) pos = vec2(-1,-1);
    else if(vertID == 1) pos = vec2(1,-1);
    else pos = vec2(0, 1);
    //缩放
    pos*=vec2(0.1, 0.1);
    //偏移
    pos -=vec2(0.5, 0);
    // 添加基于三角形ID的偏移
    pos.x += triID * 0.2;    
    gl_Position = vec4(pos, 0.0, 1.0);
}
