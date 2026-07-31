#version 150

in vec4 sf_Vertex;
uniform mat4 sf_ModelViewProjectionMatrix;

void main()
{
    // Transform the vertex position
    gl_Position = sf_ModelViewProjectionMatrix * sf_Vertex;
}
