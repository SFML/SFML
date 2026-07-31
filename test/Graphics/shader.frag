#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;
uniform float blink_alpha;

varying vec4 sf_FrontColor;

void main()
{
    vec4 pixel = sf_FrontColor;
    pixel.a = blink_alpha;
    gl_FragColor = pixel;
}
