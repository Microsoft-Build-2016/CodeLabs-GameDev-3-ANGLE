precision mediump float;
varying vec2 v_texCoord;

uniform vec4 color;
uniform sampler2D sprite;

void main()
{
    gl_FragColor = (texture2D(sprite, v_texCoord) * color);
} 