attribute vec4 a_vertex;
varying vec2 v_texCoord;

uniform mat4 projection;
uniform vec2 offset;

void main()
{
    float scale = 10.0;
    v_texCoord = a_vertex.zw;
    gl_Position = projection * vec4((a_vertex.xy * scale) + offset, 0.0, 1.0);
}