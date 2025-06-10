//几何着色器
#version 330 core
layout (triangles) in;//输入是三角形
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

in vec2[] vTexCoords;
out vec2 gTexCoords;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
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
    vec3 normal = GetNormal();
    gTexCoords = vTexCoords[0];
    gl_Position = explode(gl_in[0].gl_Position, normal);
    EmitVertex();
    gTexCoords = vTexCoords[1];
    gl_Position = explode(gl_in[1].gl_Position, normal); 
    EmitVertex();
    gTexCoords = vTexCoords[2];
    gl_Position = explode(gl_in[2].gl_Position, normal); 
    EmitVertex();
    EndPrimitive();
}

