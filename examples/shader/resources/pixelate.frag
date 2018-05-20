uniform sampler2D texture;
uniform float pixel_threshold;

varying vec4 texCoord;
varying vec4 frontColor;

void main()
{
    float factor = 1.0 / (pixel_threshold + 0.001);
    vec2 pos = floor(texCoord.xy * factor + 0.5) / factor;
    gl_FragColor = texture2D(texture, pos) * frontColor;
}
