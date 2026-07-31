#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;
uniform float blur_radius;

varying vec4 sf_FrontColor;
varying vec4 sf_TexCoord0;

void main()
{
    vec2 offx = vec2(blur_radius, 0.0);
    vec2 offy = vec2(0.0, blur_radius);

    vec4 pixel = texture2D(texture, sf_TexCoord0.xy)               * 4.0 +
                 texture2D(texture, sf_TexCoord0.xy - offx)        * 2.0 +
                 texture2D(texture, sf_TexCoord0.xy + offx)        * 2.0 +
                 texture2D(texture, sf_TexCoord0.xy - offy)        * 2.0 +
                 texture2D(texture, sf_TexCoord0.xy + offy)        * 2.0 +
                 texture2D(texture, sf_TexCoord0.xy - offx - offy) * 1.0 +
                 texture2D(texture, sf_TexCoord0.xy - offx + offy) * 1.0 +
                 texture2D(texture, sf_TexCoord0.xy + offx - offy) * 1.0 +
                 texture2D(texture, sf_TexCoord0.xy + offx + offy) * 1.0;

    gl_FragColor = sf_FrontColor * (pixel / 16.0);
}
