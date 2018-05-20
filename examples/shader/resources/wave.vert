uniform float wave_phase;
uniform vec2 wave_amplitude;

attribute vec2 positionAttribute;
attribute vec4 colorAttribute;
attribute vec2 texCoordAttribute;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;

varying vec4 texCoord;
varying vec4 frontColor;

void main()
{
    vec4 vertex = vec4(positionAttribute, 0.0, 1.0);
    vertex.x += cos(positionAttribute.y * 0.02 + wave_phase * 3.8) * wave_amplitude.x
              + sin(positionAttribute.y * 0.02 + wave_phase * 6.3) * wave_amplitude.x * 0.3;
    vertex.y += sin(positionAttribute.x * 0.02 + wave_phase * 2.4) * wave_amplitude.y
              + cos(positionAttribute.x * 0.02 + wave_phase * 5.2) * wave_amplitude.y * 0.3;

    gl_Position = modelViewProjectionMatrix * vertex;
    texCoord = textureMatrix * vec4(texCoordAttribute, 0.0, 1.0);
    frontColor = colorAttribute;
}
