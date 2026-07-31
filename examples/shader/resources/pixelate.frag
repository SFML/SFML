#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;
uniform float pixel_threshold;

varying vec4 sf_FrontColor;
varying vec4 sf_TexCoord0;

void main()
{
    float factor = 1.0 / (pixel_threshold + 0.001);
    vec2 pos = floor(sf_TexCoord0.xy * factor + 0.5) / factor;
    gl_FragColor = texture2D(texture, pos) * sf_FrontColor;
}
