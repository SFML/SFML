////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/OpenGL/ShaderImplDefault.hpp>

#include <SFML/Window/GlResource.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Exception.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Utils.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <array>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <utility>
#include <vector>

#include <cstdint>

#ifndef SFML_OPENGL_ES

#if defined(SFML_SYSTEM_MACOS) || defined(SFML_SYSTEM_IOS)

#define castToGlHandle(x)   reinterpret_cast<GLEXT_GLhandle>(std::ptrdiff_t{x})
#define castFromGlHandle(x) static_cast<unsigned int>(reinterpret_cast<std::ptrdiff_t>(x))

#else

#define castToGlHandle(x)   (x)
#define castFromGlHandle(x) (x)

#endif

namespace
{
[[maybe_unused]]
// Retrieve the maximum number of texture units available
std::size_t getMaxTextureUnits()
{
    static const GLint maxUnits = []
    {
        GLint value = 0;
        glCheck(glGetIntegerv(GLEXT_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &value));

        return value;
    }();

    return static_cast<std::size_t>(maxUnits);
}

// Read the contents of a file into an array of char
bool getFileContents(const std::filesystem::path& filename, std::vector<char>& buffer)
{
    if (auto file = std::ifstream(filename, std::ios_base::binary))
    {
        file.seekg(0, std::ios_base::end);
        const std::ifstream::pos_type size = file.tellg();
        if (size > 0)
        {
            file.seekg(0, std::ios_base::beg);
            buffer.resize(static_cast<std::size_t>(size));
            file.read(buffer.data(), static_cast<std::streamsize>(size));
        }
        buffer.push_back('\0');
        return true;
    }

    return false;
}

// Read the contents of a stream into an array of char
bool getStreamContents(sf::InputStream& stream, std::vector<char>& buffer)
{
    bool                success = false;
    const std::optional size    = stream.getSize();
    if (size > std::size_t{0})
    {
        buffer.resize(*size);

        if (!stream.seek(0).has_value())
        {
            sf::err() << "Failed to seek shader stream" << std::endl;
            return false;
        }

        const std::optional read = stream.read(buffer.data(), *size);
        success                  = (read == size);
    }
    buffer.push_back('\0');
    return success;
}

// Transforms an array of 2D vectors into a contiguous array of scalars
[[maybe_unused]] std::vector<float> flatten(const sf::Vector2f* vectorArray, std::size_t length)
{
    const std::size_t vectorSize = 2;

    std::vector<float> contiguous(vectorSize * length);
    for (std::size_t i = 0; i < length; ++i)
    {
        contiguous[vectorSize * i]     = vectorArray[i].x;
        contiguous[vectorSize * i + 1] = vectorArray[i].y;
    }

    return contiguous;
}

// Transforms an array of 3D vectors into a contiguous array of scalars
[[maybe_unused]] std::vector<float> flatten(const sf::Vector3f* vectorArray, std::size_t length)
{
    const std::size_t vectorSize = 3;

    std::vector<float> contiguous(vectorSize * length);
    for (std::size_t i = 0; i < length; ++i)
    {
        contiguous[vectorSize * i]     = vectorArray[i].x;
        contiguous[vectorSize * i + 1] = vectorArray[i].y;
        contiguous[vectorSize * i + 2] = vectorArray[i].z;
    }

    return contiguous;
}

// Transforms an array of 4D vectors into a contiguous array of scalars
[[maybe_unused]] std::vector<float> flatten(const sf::Glsl::Vec4* vectorArray, std::size_t length)
{
    const std::size_t vectorSize = 4;

    std::vector<float> contiguous(vectorSize * length);
    for (std::size_t i = 0; i < length; ++i)
    {
        contiguous[vectorSize * i]     = vectorArray[i].x;
        contiguous[vectorSize * i + 1] = vectorArray[i].y;
        contiguous[vectorSize * i + 2] = vectorArray[i].z;
        contiguous[vectorSize * i + 3] = vectorArray[i].w;
    }

    return contiguous;
}
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
Shader::Shader() : m_impl(std::make_unique<priv::ShaderImplDefault>())
{
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& filename, Type type)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromFile(filename, type))
        throw Exception("Failed to load shader from file");
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromFile(vertexShaderFilename, fragmentShaderFilename))
        throw Exception("Failed to load shader from files");
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& vertexShaderFilename,
               const std::filesystem::path& geometryShaderFilename,
               const std::filesystem::path& fragmentShaderFilename)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromFile(vertexShaderFilename, geometryShaderFilename, fragmentShaderFilename))
        throw Exception("Failed to load shader from files");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view shader, Type type)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromMemory(shader, type))
        throw Exception("Failed to load shader from memory");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view vertexShader, std::string_view fragmentShader)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromMemory(vertexShader, fragmentShader))
        throw Exception("Failed to load shader from memory");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromMemory(vertexShader, geometryShader, fragmentShader))
        throw Exception("Failed to load shader from memory");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& stream, Type type)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromStream(stream, type))
        throw Exception("Failed to load shader from stream");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromStream(vertexShaderStream, fragmentShaderStream))
        throw Exception("Failed to load shader from streams");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& vertexShaderStream, InputStream& geometryShaderStream, InputStream& fragmentShaderStream)
{
    m_impl = std::make_unique<priv::ShaderImplDefault>();
    if (!loadFromStream(vertexShaderStream, geometryShaderStream, fragmentShaderStream))
        throw Exception("Failed to load shader from streams");
}


////////////////////////////////////////////////////////////
Shader::~Shader() = default;


////////////////////////////////////////////////////////////
Shader::Shader(Shader&& source) noexcept = default;


////////////////////////////////////////////////////////////
Shader& Shader::operator=(Shader&& right) noexcept = default;

////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::filesystem::path& filename, Type type)
{
    // Read the file
    std::vector<char> shader;
    if (!getFileContents(filename, shader))
    {
        err() << "Failed to open shader file\n" << formatDebugPathInfo(filename) << std::endl;
        return false;
    }

    // Compile the shader program
    if (type == Type::Vertex)
        return m_impl->compile(shader.data(), {}, {});

    if (type == Type::Geometry)
        return m_impl->compile({}, shader.data(), {});

    return m_impl->compile({}, {}, shader.data());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::filesystem::path& vertexShaderFilename,
                          const std::filesystem::path& fragmentShaderFilename)
{
    // Read the vertex shader file
    std::vector<char> vertexShader;
    if (!getFileContents(vertexShaderFilename, vertexShader))
    {
        err() << "Failed to open vertex shader file\n" << formatDebugPathInfo(vertexShaderFilename) << std::endl;
        return false;
    }

    // Read the fragment shader file
    std::vector<char> fragmentShader;
    if (!getFileContents(fragmentShaderFilename, fragmentShader))
    {
        err() << "Failed to open fragment shader file\n" << formatDebugPathInfo(fragmentShaderFilename) << std::endl;
        return false;
    }

    // Compile the shader program
    return m_impl->compile(vertexShader.data(), {}, fragmentShader.data());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::filesystem::path& vertexShaderFilename,
                          const std::filesystem::path& geometryShaderFilename,
                          const std::filesystem::path& fragmentShaderFilename)
{
    // Read the vertex shader file
    std::vector<char> vertexShader;
    if (!getFileContents(vertexShaderFilename, vertexShader))
    {
        err() << "Failed to open vertex shader file\n" << formatDebugPathInfo(vertexShaderFilename) << std::endl;
        return false;
    }

    // Read the geometry shader file
    std::vector<char> geometryShader;
    if (!getFileContents(geometryShaderFilename, geometryShader))
    {
        err() << "Failed to open geometry shader file\n" << formatDebugPathInfo(geometryShaderFilename) << std::endl;
        return false;
    }

    // Read the fragment shader file
    std::vector<char> fragmentShader;
    if (!getFileContents(fragmentShaderFilename, fragmentShader))
    {
        err() << "Failed to open fragment shader file\n" << formatDebugPathInfo(fragmentShaderFilename) << std::endl;
        return false;
    }

    // Compile the shader program
    return m_impl->compile(vertexShader.data(), geometryShader.data(), fragmentShader.data());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view shader, Type type)
{
    // Compile the shader program
    if (type == Type::Vertex)
        return m_impl->compile(shader, {}, {});

    if (type == Type::Geometry)
        return m_impl->compile({}, shader, {});

    return m_impl->compile({}, {}, shader);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view vertexShader, std::string_view fragmentShader)
{
    // Compile the shader program
    return m_impl->compile(vertexShader, {}, fragmentShader);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
    // Compile the shader program
    return m_impl->compile(vertexShader, geometryShader, fragmentShader);
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
    if (type == Type::Vertex)
        return m_impl->compile(shader.data(), {}, {});

    if (type == Type::Geometry)
        return m_impl->compile({}, shader.data(), {});

    return m_impl->compile({}, {}, shader.data());
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
    return m_impl->compile(vertexShader.data(), {}, fragmentShader.data());
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
    return m_impl->compile(vertexShader.data(), geometryShader.data(), fragmentShader.data());
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, float x)
{
    m_impl->setUniform(name, x);
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, Glsl::Vec2 v)
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
void Shader::setUniform(const std::string& name, Glsl::Ivec2 v)
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
void Shader::setUniform(const std::string& name, Glsl::Bvec2 v)
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
unsigned int Shader::getNativeHandle() const
{
    return m_impl->getNativeHandle();
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
    priv::ShaderImpl::bind(shader ? shader->m_impl.get() : nullptr);
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    return priv::ShaderImplDefault::isAvailable();
}


////////////////////////////////////////////////////////////
bool Shader::isGeometryAvailable()
{
    return priv::ShaderImplDefault::isGeometryAvailable();
}

} // namespace sf

#else // SFML_OPENGL_ES

// OpenGL ES 1 doesn't support GLSL shaders at all, we have to provide an empty implementation

namespace sf
{
////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& /* filename */, Type /* type */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& /* vertexShaderFilename */,
               const std::filesystem::path& /* fragmentShaderFilename */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& /* vertexShaderFilename */,
               const std::filesystem::path& /* geometryShaderFilename */,
               const std::filesystem::path& /* fragmentShaderFilename */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view /* shader */, Type /* type */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view /* vertexShader */, std::string_view /* fragmentShader */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view /* vertexShader */, std::string_view /* geometryShader */, std::string_view /* fragmentShader */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& /* stream */, Type /* type */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& /* vertexShaderStream */, InputStream& /* fragmentShaderStream */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& /* vertexShaderStream */,
               InputStream& /* geometryShaderStream */,
               InputStream& /* fragmentShaderStream */)
{
    throw Exception("Shaders are not supported with OpenGL ES 1");
}


////////////////////////////////////////////////////////////
Shader::~Shader() = default;


////////////////////////////////////////////////////////////
Shader::Shader(Shader&& source) noexcept = default;


////////////////////////////////////////////////////////////
Shader& Shader::operator=(Shader&& right) noexcept = default;


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::filesystem::path& /* filename */, Type /* type */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::filesystem::path& /* vertexShaderFilename */,
                          const std::filesystem::path& /* fragmentShaderFilename */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::filesystem::path& /* vertexShaderFilename */,
                          const std::filesystem::path& /* geometryShaderFilename */,
                          const std::filesystem::path& /* fragmentShaderFilename */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view /* shader */, Type /* type */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view /* vertexShader */, std::string_view /* fragmentShader */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view /* vertexShader */,
                            std::string_view /* geometryShader */,
                            std::string_view /* fragmentShader */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& /* stream */, Type /* type */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& /* vertexShaderStream */, InputStream& /* fragmentShaderStream */)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& /* vertexShaderStream */,
                            InputStream& /* geometryShaderStream */,
                            InputStream& /* fragmentShaderStream */)
{
    return false;
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, float)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, Glsl::Vec2)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Vec3&)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Vec4&)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, int)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, Glsl::Ivec2)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Ivec3&)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Ivec4&)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, bool)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, Glsl::Bvec2)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Bvec3&)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Bvec4&)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Mat3& /* matrix */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Glsl::Mat4& /* matrix */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, const Texture& /* texture */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& /* name */, CurrentTextureType)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& /* name */, const float* /* scalarArray */, std::size_t /* length */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& /* name */, const Glsl::Vec2* /* vectorArray */, std::size_t /* length */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& /* name */, const Glsl::Vec3* /* vectorArray */, std::size_t /* length */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& /* name */, const Glsl::Vec4* /* vectorArray */, std::size_t /* length */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& /* name */, const Glsl::Mat3* /* matrixArray */, std::size_t /* length */)
{
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& /* name */, const Glsl::Mat4* /* matrixArray */, std::size_t /* length */)
{
}


////////////////////////////////////////////////////////////
unsigned int Shader::getNativeHandle() const
{
    return 0;
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* /* shader */)
{
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::isGeometryAvailable()
{
    return false;
}

} // namespace sf

#endif // SFML_OPENGL_ES
