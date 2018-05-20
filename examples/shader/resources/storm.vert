uniform vec2 storm_position;
uniform float storm_total_radius;
uniform float storm_inner_radius;

attribute vec2 positionAttribute;
attribute vec4 colorAttribute;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

varying vec4 frontColor;

void main()
{
    vec4 vertex = modelViewMatrix * vec4(positionAttribute, 0.0, 1.0);
    vec2 offset = vertex.xy - storm_position;
    float len = length(offset);
    if (len < storm_total_radius)
    {
        float push_distance = storm_inner_radius + len / storm_total_radius * (storm_total_radius - storm_inner_radius);
        vertex.xy = storm_position + normalize(offset) * push_distance;
    }

    gl_Position = projectionMatrix * vertex;
    frontColor = colorAttribute;
}
