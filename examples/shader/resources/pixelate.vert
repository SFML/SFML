attribute vec2 positionAttribute;
attribute vec4 colorAttribute;
attribute vec2 texCoordAttribute;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;

varying vec4 texCoord;
varying vec4 frontColor;

void main()
{
    gl_Position = modelViewProjectionMatrix * vec4(positionAttribute, 0.0, 1.0);
    texCoord = textureMatrix * vec4(texCoordAttribute, 0.0, 1.0);
    frontColor = colorAttribute;
}
