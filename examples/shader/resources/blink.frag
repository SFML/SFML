uniform float blink_alpha;

varying vec4 frontColor;

void main()
{
    vec4 pixel = frontColor;
    pixel.a = blink_alpha;
    gl_FragColor = pixel;
}
