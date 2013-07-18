////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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


#ifndef SFML_OPENGL_ES

namespace
{
    // Retrieve the maximum number of texture units available
    GLint getMaxTextureUnits()
    {
        GLint maxUnits = 0;
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
            if (size > 0)
            {
                file.seekg(0, std::ios_base::beg);
                buffer.resize(static_cast<std::size_t>(size));
                file.read(&buffer[0], size);
            }
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
        bool success = true;
        sf::Int64 size = stream.getSize();
        if (size > 0)
        {
            buffer.resize(static_cast<std::size_t>(size));
            stream.seek(0);
            sf::Int64 read = stream.read(&buffer[0], size);
            success = (read == size);
        }
        buffer.push_back('\0');
        return success;
    }
    
    // Get pass through vertex shader code when using geometry shader without vertex shader
    std::string getDefaultVertexShaderCode()
    {
        std::string vertexShader = 
            "void main()\n\
            {\n\
                // transform the vertex position\n\
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
                \n\
                // transform the texture coordinates\n\
                gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n\
                \n\
                // forward the vertex color\n\
                gl_FrontColor = gl_Color;\n\
            }";
        return vertexShader;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_shaderProgram (0),
m_currentTexture(-1),
m_textures      (),
m_params        ()
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
    bool result = false;
    switch(type)
    {
    case Vertex:
        result = compile(&shader[0], NULL, NULL);
        break;
        
    case Geometry:
        result = compile(NULL, &shader[0], NULL);
        break;
        
    case Fragment:
        result = compile(NULL, NULL, &shader[0]);
        break;
    }
    
    return result;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename)
{
    // Read the vertex shader file if given
    std::vector<char> vertexShader;
    bool useVertexShader = !vertexShaderFilename.empty();
    if (useVertexShader)
    {
        if (!getFileContents(vertexShaderFilename, vertexShader))
        {
            err() << "Failed to open vertex shader file \"" << vertexShaderFilename << "\"" << std::endl;
            return false;
        }
    }
    
    // Read the geometry shader file if given
    std::vector<char> geometryShader;
    bool useGeometryShader = !geometryShaderFilename.empty();
    if (useGeometryShader)
    {
        if (!getFileContents(geometryShaderFilename, geometryShader))
        {
            err() << "Failed to open geometry shader file \"" << geometryShaderFilename << "\"" << std::endl;
            return false;
        }
    }

    // Read the fragment shader file if given
    std::vector<char> fragmentShader;
    bool useFragmentShader = !fragmentShaderFilename.empty();
    if (useFragmentShader)
    {
        if (!getFileContents(fragmentShaderFilename, fragmentShader))
        {
            err() << "Failed to open fragment shader file \"" << fragmentShaderFilename << "\"" << std::endl;
            return false;
        }
    }

    // Compile the shader program
    return compile(useVertexShader ? &vertexShader[0] : NULL,
                   useGeometryShader ? &geometryShader[0] : NULL,
                   useFragmentShader ? &fragmentShader[0] : NULL);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
    // Compile the shader program
    bool result = false;
    switch(type)
    {
    case Vertex:
        result = compile(shader.c_str(), NULL, NULL);
        break;
        
    case Geometry:
        result = compile(NULL, shader.c_str(), NULL);
        break;
        
    case Fragment:
        result = compile(NULL, NULL, shader.c_str());
        break;
    }
    
    return result;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
    // Check which shaders are given
    bool useVertexShader = !vertexShader.empty();
    bool useGeometryShader = !geometryShader.empty();
    bool useFragmentShader = !fragmentShader.empty();

    // Compile the shader program
    return compile(useVertexShader ? vertexShader.c_str() : NULL,
                   useGeometryShader ? geometryShader.c_str() : NULL,
                   useFragmentShader ? fragmentShader.c_str() : NULL);
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
    bool result = false;
    switch(type)
    {
    case Vertex:
        result = compile(&shader[0], NULL, NULL);
        break;
        
    case Geometry:
        result = compile(NULL, &shader[0], NULL);
        break;
        
    case Fragment:
        result = compile(NULL, NULL, &shader[0]);
        break;
    }
    
    return result;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& geometryShaderStream, InputStream& fragmentShaderStream)
{    
    // Read the vertex shader code from the stream if given
    std::vector<char> vertexShader;
    bool useVertexShader = vertexShaderStream.getSize() > 0;
    if (useVertexShader)
    {
        if (!getStreamContents(vertexShaderStream, vertexShader))
        {
            err() << "Failed to read vertex shader from stream" << std::endl;
            return false;
        }
    }
    
    // Read the geometry shader code from the stream if given
    std::vector<char> geometryShader;
    bool useGeometryShader = geometryShaderStream.getSize() > 0;
    if (useGeometryShader)
    {
        if (!getStreamContents(geometryShaderStream, geometryShader))
        {
            err() << "Failed to read geometry shader from stream" << std::endl;
            return false;
        }
    }

    // Read the fragment shader code from the stream if given
    std::vector<char> fragmentShader;
    bool useFragmentShader = fragmentShaderStream.getSize() > 0;
    if (useFragmentShader)
    {
        if (!getStreamContents(fragmentShaderStream, fragmentShader))
        {
            err() << "Failed to read fragment shader from stream" << std::endl;
            return false;
        }
    }

    // Compile the shader program
    return compile(useVertexShader ? &vertexShader[0] : NULL,
                   useGeometryShader ? &geometryShader[0] : NULL,
                   useFragmentShader ? &fragmentShader[0] : NULL);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Enable program
        GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform1fARB(location, x));
        }

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
        GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform2fARB(location, x, y));
        }

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
        GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform3fARB(location, x, y, z));
        }

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
        GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform4fARB(location, x, y, z, w));
        }

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
        GLhandleARB program = glCheck(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
        glCheck(glUseProgramObjectARB(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.getMatrix()));
        }

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
        int location = getParamLocation(name);
        if (location != -1)
        {
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
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, CurrentTextureType)
{
    if (m_shaderProgram)
    {
        ensureGlContext();

        // Find the location of the variable in the shader
        m_currentTexture = getParamLocation(name);
    }
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
    ensureGlContext();

    if (shader && shader->m_shaderProgram)
    {
        // Enable the program
        glCheck(glUseProgramObjectARB(shader->m_shaderProgram));

        // Bind the textures
        shader->bindTextures();

        // Bind the current texture
        if (shader->m_currentTexture != -1)
            glCheck(glUniform1iARB(shader->m_currentTexture, 0));
    }
    else
    {
        // Bind no shader
        glCheck(glUseProgramObjectARB(0));
    }
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable(int shaderTypes)
{
    ensureGlContext();


    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();
   
    bool availableTypes = GLEW_ARB_shading_language_100 &&
                          GLEW_ARB_shader_objects; 
                                  
    if(shaderTypes & Vertex) 
        availableTypes = availableTypes && GLEW_ARB_vertex_shader;
        
    if(shaderTypes & Geometry) 
        availableTypes = availableTypes && GL_ARB_geometry_shader4;
        
    if(shaderTypes & Fragment) 
        availableTypes = availableTypes && GLEW_ARB_fragment_shader;
        
    return availableTypes;
}


////////////////////////////////////////////////////////////
bool Shader::compile(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode)
{
    ensureGlContext();

    // First make sure that we can use shaders
    int shadersToUse =     (vertexShaderCode ? Vertex : 0)      |
                        (geometryShaderCode ? Geometry : 0)  |
                        (fragmentShaderCode ? Fragment : 0);
    if (!isAvailable(shadersToUse))
    {
        err() << "Failed to create a shader: your system doesn't support shaders "
              << "(you should test Shader::isAvailable(shaderTypes) before trying to use the Shader class)" << std::endl;
        return false;
    }

    // Destroy the shader if it was already created
    if (m_shaderProgram)
        glCheck(glDeleteObjectARB(m_shaderProgram));

    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_params.clear();

    // Create the program
    m_shaderProgram = glCheck(glCreateProgramObjectARB());

    // Create the vertex shader if needed
    if (vertexShaderCode == NULL && geometryShaderCode)
        vertexShaderCode = getDefaultVertexShaderCode().c_str();
    if (vertexShaderCode)
    {
        // Create and compile the shader
        GLhandleARB vertexShader = glCheck(glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB));
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
    
    // Create the geometry shader if needed
    if (geometryShaderCode)
    {
        // Create and compile the shader
        GLhandleARB geometryShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
        glCheck(glShaderSourceARB(geometryShader, 1, &geometryShaderCode, NULL));
        glCheck(glCompileShaderARB(geometryShader));

        // Check the compile log
        GLint success;
        glCheck(glGetObjectParameterivARB(geometryShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(glGetInfoLogARB(geometryShader, sizeof(log), 0, log));
            err() << "Failed to compile geometry shader:" << std::endl
                  << log << std::endl;
            glCheck(glDeleteObjectARB(geometryShader));
            glCheck(glDeleteObjectARB(m_shaderProgram));
            m_shaderProgram = 0;
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(glAttachObjectARB(m_shaderProgram, geometryShader));
        glCheck(glDeleteObjectARB(geometryShader));
    }

    // Create the fragment shader if needed
    if (fragmentShaderCode)
    {
        // Create and compile the shader
        GLhandleARB fragmentShader = glCheck(glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB));
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

    // Force an OpenGL flush, so that the shader will appear updated
    // in all contexts immediately (solves problems in multi-threaded apps)
    glCheck(glFlush());

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
        Texture::bind(it->second);
        ++it;
    }

    // Make sure that the texture unit which is left active is the number 0
    glCheck(glActiveTextureARB(GL_TEXTURE0_ARB));
}


////////////////////////////////////////////////////////////
int Shader::getParamLocation(const std::string& name)
{
    // Check the cache
    ParamTable::const_iterator it = m_params.find(name);
    if (it != m_params.end())
    {
        // Already in cache, return it
        return it->second;
    }
    else
    {
        // Not in cache, request the location from OpenGL
        int location = glCheck(glGetUniformLocationARB(m_shaderProgram, name.c_str()));
        if (location != -1)
        {
            // Location found: add it to the cache
            m_params.insert(std::make_pair(name, location));
        }
        else
        {
            // Error: location not found
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;
        }

        return location;
    }
}

} // namespace sf

#else // SFML_OPENGL_ES

// OpenGL ES 1 does't support GLSL shaders at all, we have to provide an empty implementation

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
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& filename, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& stream, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
    return false;
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector2f& v)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3f& v)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Color& color)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const sf::Transform& transform)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Texture& texture)
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, CurrentTextureType)
{
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    return false;
}


////////////////////////////////////////////////////////////
void Shader::bindTextures() const
{
}

} // namespace sf

#endif // SFML_OPENGL_ES
