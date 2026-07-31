uniform float wave_phase;
uniform vec2 wave_amplitude;

attribute vec4 sf_Vertex;
attribute vec4 sf_Color;
attribute vec4 sf_MultiTexCoord0;

uniform mat4 sf_ModelViewProjectionMatrix;
uniform mat4 sf_TextureMatrix;

varying vec4 sf_FrontColor;
varying vec4 sf_TexCoord0;

void main()
{
    vec4 vertex = sf_Vertex;
    vertex.x += cos(sf_Vertex.y * 0.02 + wave_phase * 3.8) * wave_amplitude.x
              + sin(sf_Vertex.y * 0.02 + wave_phase * 6.3) * wave_amplitude.x * 0.3;
    vertex.y += sin(sf_Vertex.x * 0.02 + wave_phase * 2.4) * wave_amplitude.y
              + cos(sf_Vertex.x * 0.02 + wave_phase * 5.2) * wave_amplitude.y * 0.3;

    gl_Position = sf_ModelViewProjectionMatrix * vertex;
    sf_TexCoord0 = sf_TextureMatrix * sf_MultiTexCoord0;
    sf_FrontColor = sf_Color;
}
