uniform sampler2D texture;
uniform float edge_threshold;

varying vec4 texCoord;
varying vec4 frontColor;

void main()
{
    const float offset = 1.0 / 512.0;
    vec2 offx = vec2(offset, 0.0);
    vec2 offy = vec2(0.0, offset);

    vec4 hEdge = texture2D(texture, texCoord.xy - offy)        * -2.0 +
                 texture2D(texture, texCoord.xy + offy)        *  2.0 +
                 texture2D(texture, texCoord.xy - offx - offy) * -1.0 +
                 texture2D(texture, texCoord.xy - offx + offy) *  1.0 +
                 texture2D(texture, texCoord.xy + offx - offy) * -1.0 +
                 texture2D(texture, texCoord.xy + offx + offy) *  1.0;

    vec4 vEdge = texture2D(texture, texCoord.xy - offx)        *  2.0 +
                 texture2D(texture, texCoord.xy + offx)        * -2.0 +
                 texture2D(texture, texCoord.xy - offx - offy) *  1.0 +
                 texture2D(texture, texCoord.xy - offx + offy) * -1.0 +
                 texture2D(texture, texCoord.xy + offx - offy) *  1.0 +
                 texture2D(texture, texCoord.xy + offx + offy) * -1.0;

    vec3 result = sqrt(hEdge.rgb * hEdge.rgb + vEdge.rgb * vEdge.rgb);
    float edge = length(result);
    vec4 pixel = frontColor * texture2D(texture, texCoord.xy);
    if (edge > (edge_threshold * 8.0))
        pixel.rgb = vec3(0.0, 0.0, 0.0);
    else
        pixel.a = edge_threshold;
    gl_FragColor = pixel;
}
