//几何着色器
#version 330 core
layout (triangles) in;//输入是三角形
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵
uniform float time;


in VS_OUT {
    vec2 TexCoords;
    vec3 Pos;
} gs_in[];

out vec2 gTexCoords;

vec3 GetNormal()
{
   vec3 a = vec3(gs_in[1].Pos) - vec3(gs_in[0].Pos);
   vec3 b = vec3(gs_in[2].Pos) - vec3(gs_in[0].Pos);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

void main() 
{
    mat4 mvp = projection * view * model;
    vec3 normal = GetNormal();
    vec4 pos = vec4(gs_in[0].Pos, 1);
    gl_Position = mvp * explode(pos, normal);
    gTexCoords = gs_in[0].TexCoords;
    EmitVertex();

    pos = vec4(gs_in[1].Pos, 1);
    gl_Position = mvp * explode(pos, normal);
    gTexCoords = gs_in[1].TexCoords;
    EmitVertex();

    pos = vec4(gs_in[2].Pos, 1);
    gl_Position = mvp * explode(pos, normal);
    gTexCoords = gs_in[2].TexCoords;
    EmitVertex();
    EndPrimitive();
}
