#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
// in vec3 color;
in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{   
    gl_Position = position + vec4(-0.2, 0.0, 0.0, 0.0); //左    
    fColor = gs_in[0].color; //
    EmitVertex(); 
    gl_Position = position + vec4(0.0, sqrt(0.12), 0.0, 0.0); //上
    fColor = vec3(1, 1, 1);
    EmitVertex();   
    gl_Position = position + vec4(0.0, -sqrt(0.12), 0.0, 0.0);//下
    fColor = gs_in[0].color; //
    EmitVertex();
    gl_Position = position + vec4(0.2, 0.0, 0.0, 0.0);//右
    fColor = gs_in[0].color; //
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}