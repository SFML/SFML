#version 150

uniform sampler2D texture;

in vec2 tex_coord;

out vec4 fragColor;

void main()
{
    // Read and apply a color from the texture
    fragColor = texture2D(texture, tex_coord);
}
