#version 330 core
layout (lines) in;//输入是线段
layout (line_strip, max_vertices = 2) out;

in vec3[] color;
out vec3 gColor;

void build_house(vec4 position1, vec4 position2)
{ 
    gl_Position = position1;
    gColor = color[0];
    EmitVertex();
    gl_Position = position2; 
    gColor = color[1];
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position, gl_in[1].gl_Position);
}