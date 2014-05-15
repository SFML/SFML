#version 150

// The render target's resolution (used for scaling)
uniform vec2 resolution;

// The billboards' size
uniform vec2 size;

// Input is the passed point cloud
layout (points) in;

// The output will consist of triangle strips with four vertices each
layout (triangle_strip, max_vertices = 4) out;

// Output texture coordinates
out vec2 TexCoord;

// Main entry point
void main()
{
    // Caculate the half width/height of the billboards
    float halfWidth = size.x / 2;
    float halfHeight = size.y / 2;

    // Scale the size based on resolution (1 would be full width/height)
    halfWidth = halfWidth / resolution;
    halfHeight = halfHeight / resolution;
    
    // Iterate over all vertices
    for (int i = 0; i < gl_in.length(); i++)
    {
        // Retrieve the passed vertex position
        vec4 pos = gl_in[i].gl_Position;
        
        // Bottom left vertex
        gl_Position.x = pos.x - halfWidth;
        gl_Position.y = pos.y - halfHeight;
        TexCoord = vec2(1, 1);
        EmitVertex();
        
        // Bottom right vertex
        gl_Position.x = pos.x + halfWidth;
        gl_Position.y = pos.y - halfHeight;
        TexCoord = vec2(0, 1);
        EmitVertex();
        
        // Top left vertex
        gl_Position.x = pos.x - halfWidth;
        gl_Position.y = pos.y + halfHeight;
        TexCoord = vec2(1, 0);
        EmitVertex();
        
        // Top right vertex
        gl_Position.x = pos.x + halfWidth;
        gl_Position.y = pos.y + halfHeight;
        TexCoord = vec2(0, 0);
        EmitVertex();
        
        // And finalize the primitive
        EndPrimitive();
    }
}
