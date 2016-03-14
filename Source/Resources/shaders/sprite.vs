attribute vec4 a_vertex;
varying vec2 v_texCoord;

uniform mat4 model;
uniform mat4 projection;
uniform vec3 spriteColor;

void main()
{
    v_texCoord = a_vertex.zw;
    gl_Position = projection * model * vec4(a_vertex.xy, 0.0, 1.0);
}


