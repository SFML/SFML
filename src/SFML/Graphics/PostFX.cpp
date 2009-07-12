////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/PostFX.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
PostFX::PostFX() :
myShaderProgram(0)
{
    // Make sure that GLEW is initialized
    EnsureGlewInit();

    // No filtering on frame buffer
    myFrameBuffer.SetSmooth(false);
}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
PostFX::PostFX(const PostFX& copy) :
Drawable        (copy),
myShaderProgram (0),
myTextures      (copy.myTextures),
myFragmentShader(copy.myFragmentShader),
myFrameBuffer   (copy.myFrameBuffer)
{
    // No filtering on frame buffer
    myFrameBuffer.SetSmooth(false);

    // Create the shaders and the program
    if (copy.myShaderProgram)
        CreateProgram();
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
PostFX::~PostFX()
{
    // Destroy effect program
    if (myShaderProgram)
        GLCheck(glDeleteObjectARB(myShaderProgram));
}


////////////////////////////////////////////////////////////
/// Load the effect from a file
////////////////////////////////////////////////////////////
bool PostFX::LoadFromFile(const std::string& filename)
{
    // Open the file
    std::ifstream file(filename.c_str());
    if (!file)
    {
        std::cerr << "Failed to open effect file \"" << filename << "\"" << std::endl;
        return false;
    }

    // Apply the preprocessing pass to the fragment shader code
    myFragmentShader = PreprocessEffect(file);

    // Create the shaders and the program
    CreateProgram();

    return myShaderProgram != 0;
}


////////////////////////////////////////////////////////////
/// Load the effect from a text in memory
////////////////////////////////////////////////////////////
bool PostFX::LoadFromMemory(const std::string& effect)
{
    // Open a stream and copy the effect code
    std::istringstream stream(effect.c_str());

    // Apply the preprocessing pass to the fragment shader code
    myFragmentShader = PreprocessEffect(stream);

    // Create the shaders and the program
    CreateProgram();

    return myShaderProgram != 0;
}


////////////////////////////////////////////////////////////
/// Change a parameter of the effect (1 float)
////////////////////////////////////////////////////////////
void PostFX::SetParameter(const std::string& name, float x)
{
    if (myShaderProgram)
    {
        // Enable program
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform1fARB(location, x));
        else
            std::cerr << "Parameter \"" << name << "\" not found in post-effect" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
/// Change a parameter of the effect (2 floats)
////////////////////////////////////////////////////////////
void PostFX::SetParameter(const std::string& name, float x, float y)
{
    if (myShaderProgram)
    {
        // Enable program
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform2fARB(location, x, y));
        else
            std::cerr << "Parameter \"" << name << "\" not found in post-effect" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
/// Change a parameter of the effect (3 floats)
////////////////////////////////////////////////////////////
void PostFX::SetParameter(const std::string& name, float x, float y, float z)
{
    if (myShaderProgram)
    {
        // Enable program
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform3fARB(location, x, y, z));
        else
            std::cerr << "Parameter \"" << name << "\" not found in post-effect" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
/// Change a parameter of the effect (4 floats)
////////////////////////////////////////////////////////////
void PostFX::SetParameter(const std::string& name, float x, float y, float z, float w)
{
    if (myShaderProgram)
    {
        // Enable program
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform4fARB(location, x, y, z, w));
        else
            std::cerr << "Parameter \"" << name << "\" not found in post-effect" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
/// Set a texture parameter
////////////////////////////////////////////////////////////
void PostFX::SetTexture(const std::string& name, const Image* texture)
{
    // Check that the current texture unit is available
    GLint maxUnits;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
    if (myTextures.size() >= static_cast<std::size_t>(maxUnits))
    {
        std::cerr << "Impossible to use texture \"" << name << "\" for post-effect : all available texture units are used" << std::endl;
        return;
    }

    // Make sure the given name is a valid variable in the effect
    int location = glGetUniformLocationARB(myShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Texture \"" << name << "\" not found in post-effect" << std::endl;
        return;
    }

    // Store the texture for later use
    myTextures[name] = texture ? texture : &myFrameBuffer;
}


////////////////////////////////////////////////////////////
/// Assignment operator
////////////////////////////////////////////////////////////
PostFX& PostFX::operator =(const PostFX& other)
{
    PostFX temp(other);

    std::swap(myShaderProgram,  temp.myShaderProgram);
    std::swap(myTextures,       temp.myTextures);
    std::swap(myFragmentShader, temp.myFragmentShader);
    std::swap(myFrameBuffer,    temp.myFrameBuffer);

    return *this;
}


////////////////////////////////////////////////////////////
/// Tell whether or not the system supports post-effects
////////////////////////////////////////////////////////////
bool PostFX::CanUsePostFX()
{
    // Make sure that GLEW is initialized
    EnsureGlewInit();

    return glewIsSupported("GL_ARB_shading_language_100") != 0 &&
           glewIsSupported("GL_ARB_shader_objects")       != 0 &&
           glewIsSupported("GL_ARB_vertex_shader")        != 0 &&
           glewIsSupported("GL_ARB_fragment_shader")      != 0;
}


////////////////////////////////////////////////////////////
/// /see Drawable::Render
////////////////////////////////////////////////////////////
void PostFX::Render(RenderTarget& target) const
{
    // Check that we have a valid program
    if (!myShaderProgram)
        return;

    // Copy the current framebuffer pixels to our frame buffer texture
    // The ugly cast is temporary until PostFx are rewritten :)
    myFrameBuffer.CopyScreen((RenderWindow&)target);

    // Enable program
    GLCheck(glUseProgramObjectARB(myShaderProgram));

    // Bind textures
    TextureTable::const_iterator it = myTextures.begin();
    for (std::size_t i = 0; i < myTextures.size(); ++i)
    {
        int location = glGetUniformLocationARB(myShaderProgram, it->first.c_str());
        GLCheck(glUniform1iARB(location, static_cast<GLint>(i)));
        GLCheck(glActiveTextureARB(static_cast<GLenum>(GL_TEXTURE0_ARB + i)));
        it->second->Bind();
        it++;
    }

    // Compute the texture coordinates (it may not be (0, 0, 1, 1) if the texture is padded or flipped)
    IntRect frameBufferRect(0, 0, myFrameBuffer.GetWidth(), myFrameBuffer.GetHeight());
    FloatRect texCoords = myFrameBuffer.GetTexCoords(frameBufferRect);

    // Set the projection matrix to the identity so that the screen coordinates are in the range [-1, 1]
    GLCheck(glMatrixMode(GL_PROJECTION));
    GLCheck(glLoadIdentity());

    // Render a fullscreen quad using the effect on our framebuffer
    glBegin(GL_QUADS);
         glTexCoord2f(texCoords.Left,  texCoords.Top);    glVertex2f(-1,  1);
         glTexCoord2f(texCoords.Right, texCoords.Top);    glVertex2f( 1,  1);
         glTexCoord2f(texCoords.Right, texCoords.Bottom); glVertex2f( 1, -1);
         glTexCoord2f(texCoords.Left,  texCoords.Bottom); glVertex2f(-1, -1);
    glEnd();

    // Disable program
    GLCheck(glUseProgramObjectARB(0));

    // Disable texture units
    for (std::size_t i = 0; i < myTextures.size(); ++i)
    {
        GLCheck(glActiveTextureARB(static_cast<GLenum>(GL_TEXTURE0_ARB + i)));
        GLCheck(glBindTexture(GL_TEXTURE_2D, 0));
    }
    GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB));
}


////////////////////////////////////////////////////////////
/// Preprocess a SFML effect file
/// to convert it to a valid GLSL fragment shader
////////////////////////////////////////////////////////////
std::string PostFX::PreprocessEffect(std::istream& file)
{
    // Initialize output string
    std::set<std::string> textures;
    std::string out = "";

    // Variable declarations
    std::string line;
    while (std::getline(file, line) && (line.substr(0, 6) != "effect"))
    {
        // Remove the ending '\r', if any
        if (!line.empty() && (line[line.size() - 1] == '\r'))
            line.erase(line.size() - 1);

        // Skip empty lines
        if (line == "")
            continue;

        // Extract variables type and name and convert them
        std::string type, name;
        std::istringstream iss(line);
        if (!(iss >> type >> name))
        {
            std::cerr << "Post-effect error : invalid declaration (should be \"[type][name]\")" << std::endl
                      << "> " << line << std::endl;
            return "";
        }

        if (type == "texture")
        {
            // Textures need some checking and conversion
            if (textures.find(name) != textures.end())
            {
                std::cerr << "Post-effect error : texture \"" << name << "\" already exists" << std::endl;
                return "";
            }

            out += "uniform sampler2D " + name + ";\n";
            textures.insert(name);
        }
        else
        {
            // Other types are just copied to output with "uniform" prefix
            out += "uniform " + type + " " + name + ";\n";
        }
    }

    // Effect code
    out += "void main()\n";
    while (std::getline(file, line))
    {
        // Replace any texture lookup "T(" by "texture2D(T, "
        for (std::set<std::string>::const_iterator it = textures.begin(); it != textures.end(); ++it)
        {
            std::string::size_type pos = line.find(*it);
            if (pos != std::string::npos)
                line.replace(pos, it->size() + 1, "texture2D(" + *it + ", ");
        }

        // Replace "_in" by "gl_TexCoord[0].xy"
        for (std::string::size_type pos = line.find("_in"); pos != std::string::npos; pos = line.find("_in"))
            line.replace(pos, 3, "gl_TexCoord[0].xy");

        // Replace "_out" by "gl_FragColor"
        for (std::string::size_type pos = line.find("_out"); pos != std::string::npos; pos = line.find("_out"))
            line.replace(pos, 4, "gl_FragColor");

        // Write modified line to output string
        out += line + "\n";
    }

    return out;
}


////////////////////////////////////////////////////////////
/// Create the program and attach the shaders
////////////////////////////////////////////////////////////
void PostFX::CreateProgram()
{
    // Check that we can use post-FX !
    if (!CanUsePostFX())
    {
        std::cerr << "Failed to create a PostFX : your system doesn't support effects" << std::endl;
        return;
    }

    // Destroy effect program if it was already created
    if (myShaderProgram)
        GLCheck(glDeleteObjectARB(myShaderProgram));

    // Define vertex shader source (we provide it directly as it doesn't have to change)
    static const std::string vertexShaderSrc =
        "void main()"
        "{"
        "    gl_TexCoord[0] = gl_MultiTexCoord0;"
        "    gl_Position = ftransform();"
        "}";

    // Create the program
    myShaderProgram = glCreateProgramObjectARB();

    // Create the shaders
    GLhandleARB vertexShader   = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLhandleARB fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    // Compile them
    const char* vertexSrc   = vertexShaderSrc.c_str();
    const char* fragmentSrc = myFragmentShader.c_str();
    GLCheck(glShaderSourceARB(vertexShader,   1, &vertexSrc,   NULL));
    GLCheck(glShaderSourceARB(fragmentShader, 1, &fragmentSrc, NULL));
    GLCheck(glCompileShaderARB(vertexShader));
    GLCheck(glCompileShaderARB(fragmentShader));

    // Check the compile logs
    GLint success;
    GLCheck(glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        GLCheck(glGetInfoLogARB(vertexShader, sizeof(log), 0, log));
        std::cerr << "Failed to compile post-effect :" << std::endl
                  << log << std::endl;
        GLCheck(glDeleteObjectARB(vertexShader));
        GLCheck(glDeleteObjectARB(fragmentShader));
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return;
    }
    GLCheck(glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        GLCheck(glGetInfoLogARB(fragmentShader, sizeof(log), 0, log));
        std::cerr << "Failed to compile post-effect :" << std::endl
                  << log << std::endl;
        GLCheck(glDeleteObjectARB(vertexShader));
        GLCheck(glDeleteObjectARB(fragmentShader));
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return;
    }

    // Attach the shaders to the program
    GLCheck(glAttachObjectARB(myShaderProgram, vertexShader));
    GLCheck(glAttachObjectARB(myShaderProgram, fragmentShader));

    // We can now delete the shaders
    GLCheck(glDeleteObjectARB(vertexShader));
    GLCheck(glDeleteObjectARB(fragmentShader));

    // Link the program
    GLCheck(glLinkProgramARB(myShaderProgram));

    // Get link log
    GLCheck(glGetObjectParameterivARB(myShaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        // Oops... link errors
        char log[1024];
        GLCheck(glGetInfoLogARB(myShaderProgram, sizeof(log), 0, log));
        std::cerr << "Failed to link post-effect :" << std::endl
                  << log << std::endl;
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return;
    }
}

} // namespace sf
