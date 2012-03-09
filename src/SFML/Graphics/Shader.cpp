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
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Err.hpp>
#include <fstream>
#include <vector>


namespace
{
    // Retrieve the maximum number of texture units available
    GLint GetMaxTextureUnits()
    {
        GLint maxUnits;
        GLCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
        return maxUnits;
    }

    // Read the contents of a file into an array of char
    bool GetFileContents(const std::string& filename, std::vector<char>& buffer)
    {
        std::ifstream file(filename.c_str(), std::ios_base::binary);
        if (file)
        {
            file.seekg(0, std::ios_base::end);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios_base::beg);
            buffer.resize(size);
            file.read(&buffer[0], size);
            buffer.push_back('\0');
            return true;
        }
        else
        {
            return false;
        }
    }

    // Read the contents of a stream into an array of char
    bool GetStreamContents(sf::InputStream& stream, std::vector<char>& buffer)
    {
        sf::Int64 size = stream.GetSize();
        buffer.resize(static_cast<std::size_t>(size));
        sf::Int64 read = stream.Read(&buffer[0], size);
        buffer.push_back('\0');
        return read == size;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_shaderProgram (0),
m_currentTexture(-1)
{
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    EnsureGlContext();

    // Destroy effect program
    if (m_shaderProgram)
        GLCheck(glDeleteObjectARB(m_shaderProgram));
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromFile(const std::string& filename, Type type)
{
    // Read the file
    std::vector<char> shader;
    if (!GetFileContents(filename, shader))
    {
        Err() << "Failed to open shader file \"" << filename << "\"" << std::endl;
        return false;
    }

    // Compile the shader program
    if (type == Vertex)
        return Compile(&shader[0], NULL);
    else
        return Compile(NULL, &shader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    // Read the vertex shader file
    std::vector<char> vertexShader;
    if (!GetFileContents(vertexShaderFilename, vertexShader))
    {
        Err() << "Failed to open vertex shader file \"" << vertexShaderFilename << "\"" << std::endl;
        return false;
    }

    // Read the fragment shader file
    std::vector<char> fragmentShader;
    if (!GetFileContents(fragmentShaderFilename, fragmentShader))
    {
        Err() << "Failed to open fragment shader file \"" << fragmentShaderFilename << "\"" << std::endl;
        return false;
    }

    // Compile the shader program
    return Compile(&vertexShader[0], &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromMemory(const std::string& shader, Type type)
{
    // Compile the shader program
    if (type == Vertex)
        return Compile(shader.c_str(), NULL);
    else
        return Compile(NULL, shader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Compile the shader program
    return Compile(vertexShader.c_str(), fragmentShader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromStream(InputStream& stream, Type type)
{
    // Read the shader code from the stream
    std::vector<char> shader;
    if (!GetStreamContents(stream, shader))
    {
        Err() << "Failed to read shader from stream" << std::endl;
        return false;
    }

    // Compile the shader program
    if (type == Vertex)
        return Compile(&shader[0], NULL);
    else
        return Compile(NULL, &shader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::LoadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
    // Read the vertex shader code from the stream
    std::vector<char> vertexShader;
    if (!GetStreamContents(vertexShaderStream, vertexShader))
    {
        Err() << "Failed to read vertex shader from stream" << std::endl;
        return false;
    }

    // Read the fragment shader code from the stream
    std::vector<char> fragmentShader;
    if (!GetStreamContents(fragmentShaderStream, fragmentShader))
    {
        Err() << "Failed to read fragment shader from stream" << std::endl;
        return false;
    }

    // Compile the shader program
    return Compile(&vertexShader[0], &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, float x)
{
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
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
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
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
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
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
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
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
void Shader::SetParameter(const std::string& name, const Color& color)
{
    SetParameter(name, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, const sf::Transform& transform)
{
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        GLCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location != -1)
            GLCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.GetMatrix()));
        else
            Err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        GLCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, const Texture& texture)
{
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Find the location of the variable in the shader
        int location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location == -1)
        {
            Err() << "Texture \"" << name << "\" not found in shader" << std::endl;
            return;
        }

        // Store the location -> texture mapping
        TextureTable::iterator it = m_textures.find(location);
        if (it == m_textures.end())
        {
            // New entry, make sure there are enough texture units
            static const GLint maxUnits = GetMaxTextureUnits();
            if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
            {
                Err() << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
                return;
            }

            m_textures[location] = &texture;
        }
        else
        {
            // Location already used, just replace the texture
            it->second = &texture;
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::SetParameter(const std::string& name, CurrentTextureType)
{
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Find the location of the variable in the shader
        m_currentTexture = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (m_currentTexture == -1)
            Err() << "Texture \"" << name << "\" not found in shader" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void Shader::Bind() const
{
    if (m_shaderProgram)
    {
        EnsureGlContext();

        // Enable the program
        GLCheck(glUseProgramObjectARB(m_shaderProgram));

        // Bind the textures
        BindTextures();

        // Bind the current texture
        if (m_currentTexture != -1)
            GLCheck(glUniform1iARB(m_currentTexture, 0));
    }
}


////////////////////////////////////////////////////////////
void Shader::Unbind() const
{
    EnsureGlContext();

    GLCheck(glUseProgramObjectARB(0));
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
bool Shader::Compile(const char* vertexShaderCode, const char* fragmentShaderCode)
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
    if (m_shaderProgram)
        GLCheck(glDeleteObjectARB(m_shaderProgram));

    // Create the program
    m_shaderProgram = glCreateProgramObjectARB();

    // Create the vertex shader if needed
    if (vertexShaderCode)
    {
        // Create and compile the shader
        GLhandleARB vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
        GLCheck(glShaderSourceARB(vertexShader, 1, &vertexShaderCode, NULL));
        GLCheck(glCompileShaderARB(vertexShader));

        // Check the compile log
        GLint success;
        GLCheck(glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            GLCheck(glGetInfoLogARB(vertexShader, sizeof(log), 0, log));
            Err() << "Failed to compile vertex shader:" << std::endl
                  << log << std::endl;
            GLCheck(glDeleteObjectARB(vertexShader));
            GLCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        GLCheck(glAttachObjectARB(m_shaderProgram, vertexShader));
        GLCheck(glDeleteObjectARB(vertexShader));
    }

    // Create the fragment shader if needed
    if (fragmentShaderCode)
    {
        // Create and compile the shader
        GLhandleARB fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
        GLCheck(glShaderSourceARB(fragmentShader, 1, &fragmentShaderCode, NULL));
        GLCheck(glCompileShaderARB(fragmentShader));

        // Check the compile log
        GLint success;
        GLCheck(glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            GLCheck(glGetInfoLogARB(fragmentShader, sizeof(log), 0, log));
            Err() << "Failed to compile fragment shader:" << std::endl
                  << log << std::endl;
            GLCheck(glDeleteObjectARB(fragmentShader));
            GLCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        GLCheck(glAttachObjectARB(m_shaderProgram, fragmentShader));
        GLCheck(glDeleteObjectARB(fragmentShader));
    }

    // Link the program
    GLCheck(glLinkProgramARB(m_shaderProgram));

    // Check the link log
    GLint success;
    GLCheck(glGetObjectParameterivARB(m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        GLCheck(glGetInfoLogARB(m_shaderProgram, sizeof(log), 0, log));
        Err() << "Failed to link shader:" << std::endl
              << log << std::endl;
        GLCheck(glDeleteObjectARB(m_shaderProgram));
        m_shaderProgram = 0;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
void Shader::BindTextures() const
{
    TextureTable::const_iterator it = m_textures.begin();
    for (std::size_t i = 0; i < m_textures.size(); ++i)
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

} // namespace sf
