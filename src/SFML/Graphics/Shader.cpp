////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/ShaderImplNull.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Err.hpp>
#include <fstream>
#include <vector>

#if !defined(SFML_OPENGL_ES)

#include <SFML/Graphics/OpenGL/GL1/ShaderImplDefault.hpp>

#endif


namespace
{
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
}


namespace sf
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_impl(NULL)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
    {
#if !defined(SFML_OPENGL_ES)

        m_impl = new priv::ShaderImplDefault;

#else

        m_impl = new priv::ShaderImplNull;

#endif
    }
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    delete m_impl;
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
        return m_impl->compile(&shader[0], NULL, NULL);
    else if (type == Geometry)
        return m_impl->compile(NULL, &shader[0], NULL);
    else
        return m_impl->compile(NULL, NULL, &shader[0]);
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
    return m_impl->compile(&vertexShader[0], NULL, &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& geometryShaderFilename, const std::string& fragmentShaderFilename)
{
    // Read the vertex shader file
    std::vector<char> vertexShader;
    if (!getFileContents(vertexShaderFilename, vertexShader))
    {
        err() << "Failed to open vertex shader file \"" << vertexShaderFilename << "\"" << std::endl;
        return false;
    }

    // Read the geometry shader file
    std::vector<char> geometryShader;
    if (!getFileContents(geometryShaderFilename, geometryShader))
    {
        err() << "Failed to open geometry shader file \"" << geometryShaderFilename << "\"" << std::endl;
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
    return m_impl->compile(&vertexShader[0], &geometryShader[0], &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
    // Compile the shader program
    if (type == Vertex)
        return m_impl->compile(shader.c_str(), NULL, NULL);
    else if (type == Geometry)
        return m_impl->compile(NULL, shader.c_str(), NULL);
    else
        return m_impl->compile(NULL, NULL, shader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Compile the shader program
    return m_impl->compile(vertexShader.c_str(), NULL, fragmentShader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
    // Compile the shader program
    return m_impl->compile(vertexShader.c_str(), geometryShader.c_str(), fragmentShader.c_str());
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
        return m_impl->compile(&shader[0], NULL, NULL);
    else if (type == Geometry)
        return m_impl->compile(NULL, &shader[0], NULL);
    else
        return m_impl->compile(NULL, NULL, &shader[0]);
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
    return m_impl->compile(&vertexShader[0], NULL, &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& geometryShaderStream, InputStream& fragmentShaderStream)
{
    // Read the vertex shader code from the stream
    std::vector<char> vertexShader;
    if (!getStreamContents(vertexShaderStream, vertexShader))
    {
        err() << "Failed to read vertex shader from stream" << std::endl;
        return false;
    }

    // Read the geometry shader code from the stream
    std::vector<char> geometryShader;
    if (!getStreamContents(geometryShaderStream, geometryShader))
    {
        err() << "Failed to read geometry shader from stream" << std::endl;
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
    return m_impl->compile(&vertexShader[0], &geometryShader[0], &fragmentShader[0]);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, float x)
{
    m_impl->setUniform(name, x);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Vec2& v)
{
    m_impl->setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Vec3& v)
{
    m_impl->setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Vec4& v)
{
    m_impl->setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, int x)
{
    m_impl->setUniform(name, x);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Ivec2& v)
{
    m_impl->setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Ivec3& v)
{
    m_impl->setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Ivec4& v)
{
    m_impl->setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, bool x)
{
    setUniform(name, static_cast<int>(x));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Bvec2& v)
{
    setUniform(name, Glsl::Ivec2(v));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Bvec3& v)
{
    setUniform(name, Glsl::Ivec3(v));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Bvec4& v)
{
    setUniform(name, Glsl::Ivec4(v));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Mat3& matrix)
{
    m_impl->setUniform(name, matrix);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Mat4& matrix)
{
    m_impl->setUniform(name, matrix);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Texture& texture)
{
    m_impl->setUniform(name, texture);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, CurrentTextureType)
{
    m_impl->setUniform(name, CurrentTexture);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const float* scalarArray, std::size_t length)
{
    m_impl->setUniformArray(name, scalarArray, length);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length)
{
    m_impl->setUniformArray(name, vectorArray, length);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length)
{
    m_impl->setUniformArray(name, vectorArray, length);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length)
{
    m_impl->setUniformArray(name, vectorArray, length);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length)
{
    m_impl->setUniformArray(name, matrixArray, length);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length)
{
    m_impl->setUniformArray(name, matrixArray, length);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x)
{
    setUniform(name, x);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y)
{
    setUniform(name, Glsl::Vec2(x, y));
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z)
{
    setUniform(name, Glsl::Vec3(x, y, z));
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
    setUniform(name, Glsl::Vec4(x, y, z, w));
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector2f& v)
{
    setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3f& v)
{
    setUniform(name, v);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Color& color)
{
    setUniform(name, Glsl::Vec4(color));
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Transform& transform)
{
    setUniform(name, Glsl::Mat4(transform));
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Texture& texture)
{
    setUniform(name, texture);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, CurrentTextureType)
{
    setUniform(name, CurrentTexture);
}


////////////////////////////////////////////////////////////
unsigned int Shader::getNativeHandle() const
{
    return m_impl->getNativeHandle();
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
    {
#if !defined(SFML_OPENGL_ES)

        priv::ShaderImplDefault::bind(shader ? static_cast<const priv::ShaderImplDefault*>(shader->m_impl) : 0);

#else

        priv::ShaderImplNull::bind(shader ? static_cast<const priv::ShaderImplNull*>(shader->m_impl) : 0);

#endif
    }
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
    {
#if !defined(SFML_OPENGL_ES)

        return priv::ShaderImplDefault::isAvailable();

#else

        return priv::ShaderImplNull::isAvailable();

#endif
    }

    return false;
}


////////////////////////////////////////////////////////////
bool Shader::isGeometryAvailable()
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
    {
#if !defined(SFML_OPENGL_ES)

        return priv::ShaderImplDefault::isGeometryAvailable();

#else

        return priv::ShaderImplNull::isGeometryAvailable();

#endif
    }

    return false;
}

} // namespace sf
