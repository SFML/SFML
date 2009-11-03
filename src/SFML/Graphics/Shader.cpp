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
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


namespace sf
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
const Image Shader::CurrentTexture;

////////////////////////////////////////////////////////////
Shader::Shader() :
myShaderProgram (0),
myCurrentTexture(-1)
{
    // Make sure that GLEW is initialized
    priv::EnsureGlewInit();
}


////////////////////////////////////////////////////////////
Shader::Shader(const Shader& copy) :
myShaderProgram (0),
myCurrentTexture(copy.myCurrentTexture),
myTextures      (copy.myTextures),
myFragmentShader(copy.myFragmentShader)
{
    // Create the shaders and the program
    if (copy.myShaderProgram)
        CompileProgram();
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    // Destroy effect program
    if (myShaderProgram)
        GLCheck(glDeleteObjectARB(myShaderProgram));
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromFile(const std::string& filename)
{
    // Open the file
    std::ifstream file(filename.c_str());
    if (!file)
    {
        std::cerr << "Failed to open shader file \"" << filename << "\"" << std::endl;
        return false;
    }

    // Read the shader code from the file
    std::string line;
    while (std::getline(file, line))
        myFragmentShader += line + "\n";

    // Create the shaders and the program
    return CompileProgram();
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromMemory(const std::string& shader)
{
    // Save the shader code
    myFragmentShader = shader;

    // Create the shaders and the program
    return CompileProgram();
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x)
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
            std::cerr << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x, float y)
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
            std::cerr << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x, float y, float z)
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
            std::cerr << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x, float y, float z, float w)
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
            std::cerr << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, const Vector2f& v)
{
    SetParameter(name, v.x, v.y);
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, const Vector3f& v)
{
    SetParameter(name, v.x, v.y, v.z);
}


////////////////////////////////////////////////////////////
void Shader::SetTexture(const std::string& name, const Image& texture)
{
    // Check if there is a texture unit available
    GLint maxUnits;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
    if (myTextures.size() + 1 >= static_cast<std::size_t>(maxUnits))
    {
        std::cerr << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
        return;
    }

    // Make sure the given name is a valid variable in the effect
    int location = glGetUniformLocationARB(myShaderProgram, name.c_str());
    if (location == -1)
    {
        std::cerr << "Texture \"" << name << "\" not found in Shader" << std::endl;
        return;
    }

    // Store the texture for later use
    if ((texture.GetWidth() > 0) && (texture.GetHeight() > 0))
    {
        myTextures[location] = &texture;
    }
    else
    {
        // An invalid size means that texture is Shader::CurrentTexture
        myCurrentTexture = location;
    }
}


////////////////////////////////////////////////////////////
void Shader::Bind() const
{
    // Make sure that we have a valid program
    if (!myShaderProgram)
        return;

    // Enable the program
    GLCheck(glUseProgramObjectARB(myShaderProgram));

    // Bind the textures
    TextureTable::const_iterator it = myTextures.begin();
    for (std::size_t i = 0; i < myTextures.size(); ++i)
    {
        GLint index = i + 1;
        GLCheck(glUniform1iARB(it->first, index));
        GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB + index));
        it->second->Bind();
        it++;
    }

    // Make sure that the texture unit which is left active is the number 0
    GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB));

    // Bind the current texture
    if (myCurrentTexture != -1)
        GLCheck(glUniform1iARB(myCurrentTexture, 0));
}


////////////////////////////////////////////////////////////
void Shader::Unbind() const
{
    // First make sure that the program is currently bound
    GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
    if (!myShaderProgram || (myShaderProgram != program))
        return;

    // Unbind the program
    GLCheck(glUseProgramObjectARB(0));

    // Disable texture units
    for (std::size_t i = 0; i < myTextures.size(); ++i)
    {
        GLint index = i + 1;
        GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB + index));
        GLCheck(glBindTexture(GL_TEXTURE_2D, 0));
    }

    // Make sure that the texture unit which is left active is the number 0
    GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB));
}


////////////////////////////////////////////////////////////
Shader& Shader::operator =(const Shader& right)
{
    Shader temp(right);

    std::swap(myShaderProgram,  temp.myShaderProgram);
    std::swap(myCurrentTexture, temp.myCurrentTexture);
    std::swap(myTextures,       temp.myTextures);
    std::swap(myFragmentShader, temp.myFragmentShader);

    return *this;
}


////////////////////////////////////////////////////////////
bool Shader::IsAvailable()
{
    // Make sure that GLEW is initialized
    priv::EnsureGlewInit();

    return glewIsSupported("GL_ARB_shading_language_100") != 0 &&
           glewIsSupported("GL_ARB_shader_objects")       != 0 &&
           glewIsSupported("GL_ARB_vertex_shader")        != 0 &&
           glewIsSupported("GL_ARB_fragment_shader")      != 0;
}


////////////////////////////////////////////////////////////
bool Shader::CompileProgram()
{
    // First make sure that we can use shaders
    if (!IsAvailable())
    {
        std::cerr << "Failed to create a shader: your system doesn't support shaders "
                  << "(you should test Shader::IsAvailable() before trying to use the Shader class)" << std::endl;
        return false;
    }

    // Destroy effect program if it was already created
    if (myShaderProgram)
        GLCheck(glDeleteObjectARB(myShaderProgram));

    // Define the vertex shader source (we provide it directly as it doesn't have to change)
    static const char* vertexSrc =
        "void main()"
        "{"
        "    gl_TexCoord[0] = gl_MultiTexCoord0;"
        "    gl_FrontColor = gl_Color;"
        "    gl_Position = ftransform();"
        "}";

    // Create the program
    myShaderProgram = glCreateProgramObjectARB();

    // Create the shaders
    GLhandleARB vertexShader   = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLhandleARB fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    // Compile them
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
        std::cerr << "Failed to compile shader:" << std::endl
                  << log << std::endl;
        GLCheck(glDeleteObjectARB(vertexShader));
        GLCheck(glDeleteObjectARB(fragmentShader));
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return false;
    }
    GLCheck(glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        GLCheck(glGetInfoLogARB(fragmentShader, sizeof(log), 0, log));
        std::cerr << "Failed to compile shader:" << std::endl
                  << log << std::endl;
        GLCheck(glDeleteObjectARB(vertexShader));
        GLCheck(glDeleteObjectARB(fragmentShader));
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return false;
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
        std::cerr << "Failed to link shader:" << std::endl
                  << log << std::endl;
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return false;
    }

    return true;
}

} // namespace sf
