#version 120

uniform sampler2D dest;
uniform sampler2D src;

uniform float windowWidth;
uniform float windowHeight;
varying vec2 texCoordVarying;

void main()
{

    
    vec4 color1= 1.0 * texture2D(dest, texCoordVarying);
    vec4 color2= 1.0 * texture2D(src, texCoordVarying);

    gl_FragColor =color2;//vec4(color2.rgb+color1.rgb,1.0);
}