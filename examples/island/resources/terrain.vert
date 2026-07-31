attribute vec4 sf_Vertex;
attribute vec4 sf_Color;
attribute vec4 sf_MultiTexCoord0;

uniform mat4 sf_ModelViewProjectionMatrix;

varying vec4 sf_FrontColor;
varying vec3 normal;

void main()
{
    gl_Position = sf_ModelViewProjectionMatrix * sf_Vertex;
    sf_FrontColor = sf_Color;
    normal = vec3(sf_MultiTexCoord0.xy, 1.0);
}
