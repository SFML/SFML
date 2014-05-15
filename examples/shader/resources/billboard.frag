#version 150

uniform sampler2D tex;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    // Read and apply a color from the texture
    FragColor = texture2D(tex, TexCoord);
}
