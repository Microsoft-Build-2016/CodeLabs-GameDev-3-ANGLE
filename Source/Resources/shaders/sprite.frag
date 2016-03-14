precision mediump float;
varying vec2 v_texCoord;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    
    gl_FragColor = vec4(spriteColor, 1.0) * texture2D(image, v_texCoord);
}  
