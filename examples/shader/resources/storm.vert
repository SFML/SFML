uniform vec2 storm_position;
uniform float storm_total_radius;
uniform float storm_inner_radius;

attribute vec4 sf_Vertex;
attribute vec4 sf_Color;
attribute vec4 sf_MultiTexCoord0;

uniform mat4 sf_ModelViewMatrix;
uniform mat4 sf_ProjectionMatrix;
uniform mat4 sf_TextureMatrix;

varying vec4 sf_FrontColor;
varying vec4 sf_TexCoord0;

void main()
{
    vec4 vertex = sf_ModelViewMatrix * sf_Vertex;
    vec2 offset = vertex.xy - storm_position;
    float len = length(offset);
    if (len < storm_total_radius)
    {
        float push_distance = storm_inner_radius + len / storm_total_radius * (storm_total_radius - storm_inner_radius);
        vertex.xy = storm_position + normalize(offset) * push_distance;
    }

    gl_Position = sf_ProjectionMatrix * vertex;
    sf_TexCoord0 = sf_TextureMatrix * sf_MultiTexCoord0;
    sf_FrontColor = sf_Color;
}
