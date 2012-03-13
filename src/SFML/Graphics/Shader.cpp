////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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
    GLint getMaxTextureUnits()
    {
        GLint maxUnits;
        glCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
        return maxUnits;
    }

    // Read the contents of a file into an array of char
    bool getFileContents(const std::string& filename, std::vector<char>& buffer)
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
    bool getStreamContents(sf::InputStream& stream, std::vector<char>& buffer)
    {
        sf::Int64 size = stream.getSize();
        buffer.resize(static_cast<std::size_t>(size));
        sf::Int64 read = stream.read(&buffer[0], size);
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
    ensureGlContext();

    // Destroy effect program
    if (m_shaderProgram)
        glCheck(glDeleteObjectARB(m_shaderProgram));
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& filename, Type type)
{
    // Read the file
    std::vector<char> shader;
    if (!getFileContents(filename, shader))
    {
        err() << "Failed to open shader file \"" << filename << "\"" << std::endl;
        return false;
    }

    // Compile the shader program
    if (type == Vertex)
        return compile(&shader[0], NULL);
    else
        return compile(NULL, &shader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    // Read the vertex shader file
    std::vector<char> vertexShader;
    if (!getFileContents(vertexShaderFilename, vertexShader))
    {
        err() << "Failed to open vertex shader file \"" << vertexShaderFilename << "\"" << std::endl;
        return false;
    }

    // Read the fragment shader file
    std::vector<char> fragmentShader;
    if (!getFileContents(fragmentShaderFilename, fragmentShader))
    {
        err() << "Failed to open fragment shader file \"" << fragmentShaderFilename << "\"" << std::endl;
        return false;
    }

    // Compile the shader program
    return compile(&vertexShader[0], &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
    // Compile the shader program
    if (type == Vertex)
        return compile(shader.c_str(), NULL);
    else
        return compile(NULL, shader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Compile the shader program
    return compile(vertexShader.c_str(), fragmentShader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& stream, Type type)
{
    // Read the shader code from the stream
    std::vector<char> shader;
    if (!getStreamContents(stream, shader))
    {
        err() << "Failed to read shader from stream" << std::endl;
        return false;
    }

    // Compile the shader program
    if (type == Vertex)
        return compile(&shader[0], NULL);
    else
        return compile(NULL, &shader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
    // Read the vertex shader code from the stream
    std::vector<char> vertexShader;
    if (!getStreamContents(vertexShaderStream, vertexShader))
    {
        err() << "Failed to read vertex shader from stream" << std::endl;
        return false;
    }

    // Read the fragment shader code from the stream
    std::vector<char> fragmentShader;
    if (!getStreamContents(fragmentShaderStream, fragmentShader))
    {
        err() << "Failed to read fragment shader from stream" << std::endl;
        return false;
    }

    // Compile the shader program
    return compile(&vertexShader[0], &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location != -1)
            glCheck(glUniform1fARB(location, x));
        else
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        glCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location != -1)
            glCheck(glUniform2fARB(location, x, y));
        else
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        glCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location != -1)
            glCheck(glUniform3fARB(location, x, y, z));
        else
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        glCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location != -1)
            glCheck(glUniform4fARB(location, x, y, z, w));
        else
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        glCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector2f& v)
{
    setParameter(name, v.x, v.y);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3f& v)
{
    setParameter(name, v.x, v.y, v.z);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Color& color)
{
    setParameter(name, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const sf::Transform& transform)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable program
        GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location != -1)
            glCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.getMatrix()));
        else
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        // Disable program
        glCheck(glUseProgramObjectARB(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Texture& texture)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Find the location of the variable in the shader
        int location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (location == -1)
        {
            err() << "Texture \"" << name << "\" not found in shader" << std::endl;
            return;
        }

        // Store the location -> texture mapping
        TextureTable::iterator it = m_textures.find(location);
        if (it == m_textures.end())
        {
            // New entry, make sure there are enough texture units
            static const GLint maxUnits = getMaxTextureUnits();
            if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
            {
                err() << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
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
void Shader::setParameter(const std::string& name, CurrentTextureType)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Find the location of the variable in the shader
        m_currentTexture = glGetUniformLocationARB(m_shaderProgram, name.c_str());
        if (m_currentTexture == -1)
            err() << "Texture \"" << name << "\" not found in shader" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void Shader::bind() const
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable the program
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Bind the textures
        bindTextures();

        // Bind the current texture
        if (m_currentTexture != -1)
            glCheck(glUniform1iARB(m_currentTexture, 0));
    }
}


////////////////////////////////////////////////////////////
void Shader::unbind() const
{
    ensureGlContext();

    glCheck(glUseProgramObjectARB(0));
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    ensureGlContext();

    // Make sure that GLEW is initialized
    priv::ensureGlewInit();

    return GLEW_ARB_shading_language_100 &&
           GLEW_ARB_shader_objects       &&
           GLEW_ARB_vertex_shader        &&
           GLEW_ARB_fragment_shader;
}


////////////////////////////////////////////////////////////
bool Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    ensureGlContext();

    // First make sure that we can use shaders
    if (!isAvailable())
    {
        err() << "Failed to create a shader: your system doesn't support shaders "
              << "(you should test Shader::IsAvailable() before trying to use the Shader class)" << std::endl;
        return false;
    }

    // Destroy the shader if it was already created
    if (m_shaderProgram)
        glCheck(glDeleteObjectARB(m_shaderProgram));

    // Create the program
    m_shaderProgram = glCreateProgramObjectARB();

    // Create the vertex shader if needed
    if (vertexShaderCode)
    {
        // Create and compile the shader
        GLhandleARB vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
        glCheck(glShaderSourceARB(vertexShader, 1, &vertexShaderCode, NULL));
        glCheck(glCompileShaderARB(vertexShader));

        // Check the compile log
        GLint success;
        glCheck(glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(glGetInfoLogARB(vertexShader, sizeof(log), 0, log));
            err() << "Failed to compile vertex shader:" << std::endl
                  << log << std::endl;
            glCheck(glDeleteObjectARB(vertexShader));
            glCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(glAttachObjectARB(m_shaderProgram, vertexShader));
        glCheck(glDeleteObjectARB(vertexShader));
    }

    // Create the fragment shader if needed
    if (fragmentShaderCode)
    {
        // Create and compile the shader
        GLhandleARB fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
        glCheck(glShaderSourceARB(fragmentShader, 1, &fragmentShaderCode, NULL));
        glCheck(glCompileShaderARB(fragmentShader));

        // Check the compile log
        GLint success;
        glCheck(glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(glGetInfoLogARB(fragmentShader, sizeof(log), 0, log));
            err() << "Failed to compile fragment shader:" << std::endl
                  << log << std::endl;
            glCheck(glDeleteObjectARB(fragmentShader));
            glCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(glAttachObjectARB(m_shaderProgram, fragmentShader));
        glCheck(glDeleteObjectARB(fragmentShader));
    }

    // Link the program
    glCheck(glLinkProgramARB(m_shaderProgram));

    // Check the link log
    GLint success;
    glCheck(glGetObjectParameterivARB(m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        glCheck(glGetInfoLogARB(m_shaderProgram, sizeof(log), 0, log));
        err() << "Failed to link shader:" << std::endl
              << log << std::endl;
        glCheck(glDeleteObjectARB(m_shaderProgram));
        m_shaderProgram = 0;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
void Shader::bindTextures() const
{
    TextureTable::const_iterator it = m_textures.begin();
    for (std::size_t i = 0; i < m_textures.size(); ++i)
    {
        GLint index = static_cast<GLsizei>(i + 1);
        glCheck(glUniform1iARB(it->first, index));
        glCheck(glActiveTextureARB(GL_TEXTURE0_ARB + index));
        it->second->bind();
        ++it;
    }

    // Make sure that the texture unit which is left active is the number 0
    glCheck(glActiveTextureARB(GL_TEXTURE0_ARB));
}

} // namespace sf
