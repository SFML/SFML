#version 150

in vec2 positionAttribute;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    // Transform the vertex position
    gl_Position = modelViewProjectionMatrix * vec4(positionAttribute, 0.0, 1.0);
}
