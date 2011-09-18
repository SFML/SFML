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
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Err.hpp>
#include <fstream>
#include <vector>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Retrieve the maximum number of texture units available
    GLint GetMaxTextureUnits()
    {
        GLint maxUnits;
        GLCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
        return maxUnits;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Shader::Shader() :
myShaderProgram (0),
myCurrentTexture(-1)
{

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
    EnsureGlContext();

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
        Err() << "Failed to open shader file \"" << filename << "\"" << std::endl;
        return false;
    }

    // Read the shader code from the file
    myFragmentShader.clear();
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
bool Shader::LoadFromStream(InputStream& stream)
{
    // Read the shader code from the stream
    std::vector<char> buffer(static_cast<std::size_t>(stream.GetSize()));
    Int64 read = stream.Read(&buffer[0], buffer.size());
    myFragmentShader.assign(&buffer[0], &buffer[0] + read);

    // Create the shaders and the program
    return CompileProgram();
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x)
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform1fARB(location, x));
        else
            Err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x, float y)
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform2fARB(location, x, y));
        else
            Err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x, float y, float z)
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform3fARB(location, x, y, z));
        else
            Err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x, float y, float z, float w)
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniform4fARB(location, x, y, z, w));
        else
            Err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(program));
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
void Shader::SetTexture(const std::string& name, const Texture& texture)
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Find the location of the variable in the shader
        int location = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (location == -1)
        {
            Err() << "Texture \"" << name << "\" not found in shader" << std::endl;
            return;
        }

        // Store the location -> texture mapping
        TextureTable::iterator it = myTextures.find(location);
        if (it == myTextures.end())
        {
            // New entry, make sure there are enough texture units
            static const GLint maxUnits = GetMaxTextureUnits();
            if (myTextures.size() + 1 >= static_cast<std::size_t>(maxUnits))
            {
                Err() << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
                return;
            }

            myTextures[location] = &texture;
        }
        else
        {
            // Location already used, just replace the texture
            it->second = &texture;
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::SetCurrentTexture(const std::string& name)
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Find the location of the variable in the shader
        myCurrentTexture = glGetUniformLocationARB(myShaderProgram, name.c_str());
        if (myCurrentTexture == -1)
            Err() << "Texture \"" << name << "\" not found in shader" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void Shader::Bind() const
{
    if (myShaderProgram)
    {
        EnsureGlContext();

        // Enable the program
        GLCheck(glUseProgramObjectARB(myShaderProgram));

        // Bind the textures
        BindTextures();

        // Bind the current texture
        if (myCurrentTexture != -1)
            GLCheck(glUniform1iARB(myCurrentTexture, 0));
    }
}


////////////////////////////////////////////////////////////
void Shader::Unbind() const
{
    EnsureGlContext();

    GLCheck(glUseProgramObjectARB(0));
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
    EnsureGlContext();

    // Make sure that GLEW is initialized
    priv::EnsureGlewInit();

    return GLEW_ARB_shading_language_100 &&
           GLEW_ARB_shader_objects       &&
           GLEW_ARB_vertex_shader        &&
           GLEW_ARB_fragment_shader;
}


////////////////////////////////////////////////////////////
bool Shader::CompileProgram()
{
    EnsureGlContext();

    // First make sure that we can use shaders
    if (!IsAvailable())
    {
        Err() << "Failed to create a shader: your system doesn't support shaders "
              << "(you should test Shader::IsAvailable() before trying to use the Shader class)" << std::endl;
        return false;
    }

    // Destroy the shader if it was already created
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
        Err() << "Failed to compile shader:" << std::endl
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
        Err() << "Failed to compile shader:" << std::endl
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
        Err() << "Failed to link shader:" << std::endl
              << log << std::endl;
        GLCheck(glDeleteObjectARB(myShaderProgram));
        myShaderProgram = 0;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
void Shader::BindTextures() const
{
    TextureTable::const_iterator it = myTextures.begin();
    for (std::size_t i = 0; i < myTextures.size(); ++i)
    {
        GLint index = static_cast<GLsizei>(i + 1);
        GLCheck(glUniform1iARB(it->first, index));
        GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB + index));
        it->second->Bind();
        ++it;
    }

    // Make sure that the texture unit which is left active is the number 0
    GLCheck(glActiveTextureARB(GL_TEXTURE0_ARB));
}


////////////////////////////////////////////////////////////
void Shader::Use() const
{
    BindTextures();
}

} // namespace sf
