uniform sampler2D texture;
uniform float blur_radius;

varying vec4 texCoord;
varying vec4 frontColor;

void main()
{
    vec2 offx = vec2(blur_radius, 0.0);
    vec2 offy = vec2(0.0, blur_radius);

    vec4 pixel = texture2D(texture, texCoord.xy)               * 4.0 +
                 texture2D(texture, texCoord.xy - offx)        * 2.0 +
                 texture2D(texture, texCoord.xy + offx)        * 2.0 +
                 texture2D(texture, texCoord.xy - offy)        * 2.0 +
                 texture2D(texture, texCoord.xy + offy)        * 2.0 +
                 texture2D(texture, texCoord.xy - offx - offy) * 1.0 +
                 texture2D(texture, texCoord.xy - offx + offy) * 1.0 +
                 texture2D(texture, texCoord.xy + offx - offy) * 1.0 +
                 texture2D(texture, texCoord.xy + offx + offy) * 1.0;

    gl_FragColor =  frontColor * (pixel / 16.0);
}
