varying vec3 normal;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
    normal = vec3(gl_MultiTexCoord0.xy, 1.0);
}
