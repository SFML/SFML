////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Transform.hpp>

#include <SFML/Window/GlResource.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Exception.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Utils.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <utility>
#include <vector>

namespace
{
constexpr std::string_view defaultVertexShader = R"(
#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 sf_Vertex;
attribute vec4 sf_Color;
attribute vec4 sf_MultiTexCoord0;

uniform mat4 sf_ModelViewMatrix;
uniform mat4 sf_ProjectionMatrix;
uniform mat4 sf_ModelViewProjectionMatrix;
uniform mat4 sf_TextureMatrix;

varying vec4 sf_FrontColor;
varying vec4 sf_TexCoord0;

void main()
{
    gl_Position = sf_ModelViewProjectionMatrix * sf_Vertex;
    sf_FrontColor = sf_Color;
    sf_TexCoord0 = sf_TextureMatrix * sf_MultiTexCoord0;
}
)";

constexpr std::string_view defaultFragmentShader = R"(
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D sf_Texture;
uniform bool sf_TextureEnabled;

varying vec4 sf_FrontColor;
varying vec4 sf_TexCoord0;

void main()
{
    vec4 color = sf_FrontColor;
    if (sf_TextureEnabled)
        color *= texture2D(sf_Texture, sf_TexCoord0.xy);
    gl_FragColor = color;
}
)";

bool allowsDefaultStage(std::string_view source)
{
    const std::size_t directive = source.find("#version");
    if (directive == std::string_view::npos)
        return true;

    std::size_t position = directive + std::string_view("#version").size();
    while ((position < source.size()) && ((source[position] == ' ') || (source[position] == '\t')))
        ++position;

    unsigned int version = 0;
    while ((position < source.size()) && (source[position] >= '0') && (source[position] <= '9'))
    {
        version = version * 10 + static_cast<unsigned int>(source[position] - '0');
        ++position;
    }

#ifdef SFML_OPENGL_ES
    return version == 100;
#else
    return version == 110;
#endif
}

// Retrieve the maximum number of texture units available
std::size_t getMaxTextureUnits()
{
    GLint maxUnits = 0;
    glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits));
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
std::vector<float> flatten(const sf::Vector2f* vectorArray, std::size_t length)
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
std::vector<float> flatten(const sf::Vector3f* vectorArray, std::size_t length)
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
std::vector<float> flatten(const sf::Glsl::Vec4* vectorArray, std::size_t length)
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
struct Shader::UniformBinder
{
    ////////////////////////////////////////////////////////////
    /// \brief Constructor: set up state before uniform is set
    ///
    ////////////////////////////////////////////////////////////
    UniformBinder(Shader& shader, const std::string& name) : currentProgram(static_cast<GLuint>(shader.m_shaderProgram))
    {
        if (currentProgram)
        {
            // Enable program object
            glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &savedProgram));
            if (currentProgram != static_cast<GLuint>(savedProgram))
                glCheck(glUseProgram(currentProgram));

            // Store uniform location for further use outside constructor
            location = shader.getUniformLocation(name);
        }
    }

    ////////////////////////////////////////////////////////////
    /// \brief Destructor: restore state after uniform is set
    ///
    ////////////////////////////////////////////////////////////
    ~UniformBinder()
    {
        // Disable program object
        if (currentProgram && (currentProgram != static_cast<GLuint>(savedProgram)))
            glCheck(glUseProgram(static_cast<GLuint>(savedProgram)));
    }

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    UniformBinder(const UniformBinder&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    UniformBinder& operator=(const UniformBinder&) = delete;

    TransientContextLock lock;             //!< Lock to keep context active while uniform is bound
    GLint                savedProgram{};   //!< Handle to the previously active program object
    GLuint               currentProgram{}; //!< Handle to the program object of the modified sf::Shader instance
    GLint                location{-1};     //!< Uniform location, used by the surrounding sf::Shader code
};


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& filename, Type type)
{
    if (!loadFromFile(filename, type))
        throw Exception("Failed to load shader from file");
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename)
{
    if (!loadFromFile(vertexShaderFilename, fragmentShaderFilename))
        throw Exception("Failed to load shader from files");
}


////////////////////////////////////////////////////////////
Shader::Shader(const std::filesystem::path& vertexShaderFilename,
               const std::filesystem::path& geometryShaderFilename,
               const std::filesystem::path& fragmentShaderFilename)
{
    if (!loadFromFile(vertexShaderFilename, geometryShaderFilename, fragmentShaderFilename))
        throw Exception("Failed to load shader from files");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view shader, Type type)
{
    if (!loadFromMemory(shader, type))
        throw Exception("Failed to load shader from memory");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view vertexShader, std::string_view fragmentShader)
{
    if (!loadFromMemory(vertexShader, fragmentShader))
        throw Exception("Failed to load shader from memory");
}


////////////////////////////////////////////////////////////
Shader::Shader(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
    if (!loadFromMemory(vertexShader, geometryShader, fragmentShader))
        throw Exception("Failed to load shader from memory");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& stream, Type type)
{
    if (!loadFromStream(stream, type))
        throw Exception("Failed to load shader from stream");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
    if (!loadFromStream(vertexShaderStream, fragmentShaderStream))
        throw Exception("Failed to load shader from streams");
}


////////////////////////////////////////////////////////////
Shader::Shader(InputStream& vertexShaderStream, InputStream& geometryShaderStream, InputStream& fragmentShaderStream)
{
    if (!loadFromStream(vertexShaderStream, geometryShaderStream, fragmentShaderStream))
        throw Exception("Failed to load shader from streams");
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    const TransientContextLock lock;

    // Destroy effect program
    if (m_shaderProgram)
        glCheck(glDeleteProgram(m_shaderProgram));
}

////////////////////////////////////////////////////////////
Shader::Shader(Shader&& source) noexcept :
    m_shaderProgram(std::exchange(source.m_shaderProgram, 0u)),
    m_currentTexture(std::exchange(source.m_currentTexture, -1)),
    m_modelViewMatrix(std::exchange(source.m_modelViewMatrix, -1)),
    m_projectionMatrix(std::exchange(source.m_projectionMatrix, -1)),
    m_modelViewProjectionMatrix(std::exchange(source.m_modelViewProjectionMatrix, -1)),
    m_textureMatrix(std::exchange(source.m_textureMatrix, -1)),
    m_textureEnabled(std::exchange(source.m_textureEnabled, -1)),
    m_defaultTexture(std::exchange(source.m_defaultTexture, -1)),
    m_textures(std::move(source.m_textures)),
    m_uniforms(std::move(source.m_uniforms))
{
}

////////////////////////////////////////////////////////////
Shader& Shader::operator=(Shader&& right) noexcept
{
    // Make sure we aren't moving ourselves.
    if (&right == this)
    {
        return *this;
    }

    if (m_shaderProgram)
    {
        // Destroy effect program
        const TransientContextLock lock;
        glCheck(glDeleteProgram(m_shaderProgram));
    }

    // Move the contents of right.
    m_shaderProgram             = std::exchange(right.m_shaderProgram, 0u);
    m_currentTexture            = std::exchange(right.m_currentTexture, -1);
    m_modelViewMatrix           = std::exchange(right.m_modelViewMatrix, -1);
    m_projectionMatrix          = std::exchange(right.m_projectionMatrix, -1);
    m_modelViewProjectionMatrix = std::exchange(right.m_modelViewProjectionMatrix, -1);
    m_textureMatrix             = std::exchange(right.m_textureMatrix, -1);
    m_textureEnabled            = std::exchange(right.m_textureEnabled, -1);
    m_defaultTexture            = std::exchange(right.m_defaultTexture, -1);
    m_textures                  = std::move(right.m_textures);
    m_uniforms                  = std::move(right.m_uniforms);
    return *this;
}

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
        return compile(shader.data(), {}, {});

    if (type == Type::Geometry)
        return compile({}, shader.data(), {});

    return compile({}, {}, shader.data());
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
    return compile(vertexShader.data(), {}, fragmentShader.data());
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
    return compile(vertexShader.data(), geometryShader.data(), fragmentShader.data());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view shader, Type type)
{
    // Compile the shader program
    if (type == Type::Vertex)
        return compile(shader, {}, {});

    if (type == Type::Geometry)
        return compile({}, shader, {});

    return compile({}, {}, shader);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view vertexShader, std::string_view fragmentShader)
{
    // Compile the shader program
    return compile(vertexShader, {}, fragmentShader);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader)
{
    // Compile the shader program
    return compile(vertexShader, geometryShader, fragmentShader);
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
        return compile(shader.data(), {}, {});

    if (type == Type::Geometry)
        return compile({}, shader.data(), {});

    return compile({}, {}, shader.data());
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
    return compile(vertexShader.data(), {}, fragmentShader.data());
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
    return compile(vertexShader.data(), geometryShader.data(), fragmentShader.data());
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, float x)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform1f(binder.location, x));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, Glsl::Vec2 v)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform2f(binder.location, v.x, v.y));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Vec3& v)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform3f(binder.location, v.x, v.y, v.z));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Vec4& v)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform4f(binder.location, v.x, v.y, v.z, v.w));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, int x)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform1i(binder.location, x));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, Glsl::Ivec2 v)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform2i(binder.location, v.x, v.y));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Ivec3& v)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform3i(binder.location, v.x, v.y, v.z));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Ivec4& v)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform4i(binder.location, v.x, v.y, v.z, v.w));
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
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniformMatrix3fv(binder.location, 1, GL_FALSE, matrix.array.data()));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Glsl::Mat4& matrix)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniformMatrix4fv(binder.location, 1, GL_FALSE, matrix.array.data()));
}


////////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Texture& texture)
{
    if (!m_shaderProgram)
        return;

    const TransientContextLock lock;

    // Find the location of the variable in the shader
    const int location = getUniformLocation(name);
    if (location != -1)
    {
        // Store the location -> texture mapping
        const auto it = m_textures.find(location);
        if (it == m_textures.end())
        {
            // New entry, make sure there are enough texture units
            if (m_textures.size() + 1 >= getMaxTextureUnits())
            {
                err() << "Impossible to use texture " << std::quoted(name)
                      << " for shader: all available texture units are used" << std::endl;
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
void Shader::setUniform(const std::string& name, CurrentTextureType)
{
    if (!m_shaderProgram)
        return;

    const TransientContextLock lock;

    // Find the location of the variable in the shader
    m_currentTexture = getUniformLocation(name);
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const float* scalarArray, std::size_t length)
{
    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform1fv(binder.location, static_cast<GLsizei>(length), scalarArray));
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length)
{
    std::vector<float> contiguous = flatten(vectorArray, length);

    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform2fv(binder.location, static_cast<GLsizei>(length), contiguous.data()));
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length)
{
    std::vector<float> contiguous = flatten(vectorArray, length);

    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform3fv(binder.location, static_cast<GLsizei>(length), contiguous.data()));
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length)
{
    std::vector<float> contiguous = flatten(vectorArray, length);

    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniform4fv(binder.location, static_cast<GLsizei>(length), contiguous.data()));
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length)
{
    static const std::size_t matrixSize = matrixArray[0].array.size();

    std::vector<float> contiguous(matrixSize * length);
    for (std::size_t i = 0; i < length; ++i)
        priv::copyMatrix(matrixArray[i].array.data(), matrixSize, &contiguous[matrixSize * i]);

    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniformMatrix3fv(binder.location, static_cast<GLsizei>(length), GL_FALSE, contiguous.data()));
}


////////////////////////////////////////////////////////////
void Shader::setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length)
{
    static const std::size_t matrixSize = matrixArray[0].array.size();

    std::vector<float> contiguous(matrixSize * length);
    for (std::size_t i = 0; i < length; ++i)
        priv::copyMatrix(matrixArray[i].array.data(), matrixSize, &contiguous[matrixSize * i]);

    const UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(glUniformMatrix4fv(binder.location, static_cast<GLsizei>(length), GL_FALSE, contiguous.data()));
}


////////////////////////////////////////////////////////////
unsigned int Shader::getNativeHandle() const
{
    return m_shaderProgram;
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
    const TransientContextLock lock;

    // Make sure that we can use shaders
    if (!isAvailable())
    {
        err() << "Failed to bind or unbind shader: your system doesn't support shaders "
              << "(you should test Shader::isAvailable() before trying to use the Shader class)" << std::endl;
        return;
    }

    if (shader && shader->m_shaderProgram)
    {
        // Enable the program
        glCheck(glUseProgram(shader->m_shaderProgram));

        // Bind the textures
        shader->bindTextures();

        // Bind the current texture
        if (shader->m_currentTexture != -1)
            glCheck(glUniform1i(shader->m_currentTexture, 0));
    }
    else
    {
        // Bind no shader
        glCheck(glUseProgram(0));
    }
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    const TransientContextLock contextLock;

    // Do not cache this result: a mobile shared context can be rebuilt
    // with a different actual ES version and reload the GL entry points.
    priv::ensureExtensionsInit();

    return glActiveTexture && glCreateShader && glShaderSource && glCompileShader && glGetShaderiv &&
           glGetShaderInfoLog && glDeleteShader && glCreateProgram && glAttachShader && glBindAttribLocation &&
           glLinkProgram && glGetProgramiv && glGetProgramInfoLog && glDeleteProgram && glUseProgram &&
           glGetUniformLocation && glUniform1i && glUniformMatrix4fv;
}


////////////////////////////////////////////////////////////
bool Shader::isGeometryAvailable()
{
#ifdef SFML_OPENGL_ES
    return false;
#else
    const TransientContextLock contextLock;
    priv::ensureExtensionsInit();
    return isAvailable() && (GLEXT_geometry_shader4 || SF_GLAD_GL_VERSION_3_2);
#endif
}


////////////////////////////////////////////////////////////
bool Shader::compile(std::string_view vertexShaderCode, std::string_view geometryShaderCode, std::string_view fragmentShaderCode)
{
    const TransientContextLock lock;

    // First make sure that we can use shaders
    if (!isAvailable())
    {
        err() << "Failed to create a shader: your system doesn't support shaders "
              << "(you should test Shader::isAvailable() before trying to use the Shader class)" << std::endl;
        return false;
    }

    if (!geometryShaderCode.empty() && (vertexShaderCode.empty() || fragmentShaderCode.empty()))
    {
        err() << "Failed to create a shader: a geometry stage requires a complete vertex + geometry + fragment "
                 "program"
              << std::endl;
        return false;
    }

    // A single vertex or fragment stage uses SFML's matching baseline
    // stage. Newer language versions must provide both stages explicitly.
    if (geometryShaderCode.empty() && (vertexShaderCode.empty() != fragmentShaderCode.empty()))
    {
        const std::string_view suppliedStage = vertexShaderCode.empty() ? fragmentShaderCode : vertexShaderCode;
        if (!allowsDefaultStage(suppliedStage))
        {
            err() << "Failed to create a shader: shaders newer than GLSL 1.10 / ESSL 1.00 must provide both "
                     "vertex and fragment stages"
                  << std::endl;
            return false;
        }

        if (vertexShaderCode.empty())
            vertexShaderCode = getDefaultVertexShaderSource();
        else
            fragmentShaderCode = getDefaultFragmentShaderSource();
    }

    if (vertexShaderCode.empty() || fragmentShaderCode.empty())
    {
        err() << "Failed to create a shader: both vertex and fragment stages are required" << std::endl;
        return false;
    }

    // Make sure we can use geometry shaders.
    if (!geometryShaderCode.empty() && !isGeometryAvailable())
    {
        err() << "Failed to create a shader: your system doesn't support geometry shaders "
              << "(you should test Shader::isGeometryAvailable() before trying to use geometry shaders)" << std::endl;
        return false;
    }

    // Create the program
    const GLuint shaderProgram = glCheck(glCreateProgram());
    if (!shaderProgram)
    {
        err() << "Failed to create a shader program" << std::endl;
        return false;
    }

    // Helper function for shader creation
    const auto createAndAttachShader =
        [shaderProgram](GLenum shaderType, const char* shaderTypeStr, std::string_view shaderCode)
    {
        // Create and compile the shader
        const GLuint shader = glCheck(glCreateShader(shaderType));
        if (!shader)
        {
            err() << "Failed to create " << shaderTypeStr << " shader" << std::endl;
            glCheck(glDeleteProgram(shaderProgram));
            return false;
        }

        const GLchar* sourceCode       = shaderCode.data();
        const auto    sourceCodeLength = static_cast<GLint>(shaderCode.length());
        glCheck(glShaderSource(shader, 1, &sourceCode, &sourceCodeLength));
        glCheck(glCompileShader(shader));

        // Check the compile log
        GLint success = 0;
        glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (success == GL_FALSE)
        {
            GLint logLength = 0;
            glCheck(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));
            std::vector<char> log(static_cast<std::size_t>(std::max(logLength, 1)));
            glCheck(glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data()));
            err() << "Failed to compile " << shaderTypeStr << " shader:" << '\n' << log.data() << std::endl;
            glCheck(glDeleteShader(shader));
            glCheck(glDeleteProgram(shaderProgram));
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(glAttachShader(shaderProgram, shader));
        glCheck(glDeleteShader(shader));
        return true;
    };

    if (!createAndAttachShader(GL_VERTEX_SHADER, "vertex", vertexShaderCode))
        return false;

#ifndef SFML_OPENGL_ES
    if (!geometryShaderCode.empty())
        if (!createAndAttachShader(GL_GEOMETRY_SHADER, "geometry", geometryShaderCode))
            return false;
#endif

    if (!createAndAttachShader(GL_FRAGMENT_SHADER, "fragment", fragmentShaderCode))
        return false;

    // Attribute locations are part of SFML's shader contract.
    glCheck(glBindAttribLocation(shaderProgram, 0, "sf_Vertex"));
    glCheck(glBindAttribLocation(shaderProgram, 1, "sf_Color"));
    glCheck(glBindAttribLocation(shaderProgram, 2, "sf_MultiTexCoord0"));

    // Link the program
    glCheck(glLinkProgram(shaderProgram));

    // Check the link log
    GLint success = 0;
    glCheck(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glCheck(glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength));
        std::vector<char> log(static_cast<std::size_t>(std::max(logLength, 1)));
        glCheck(glGetProgramInfoLog(shaderProgram, static_cast<GLsizei>(log.size()), nullptr, log.data()));
        err() << "Failed to link shader:" << '\n' << log.data() << std::endl;
        glCheck(glDeleteProgram(shaderProgram));
        return false;
    }

    // Destroy the shader if it was already created
    if (m_shaderProgram)
    {
        glCheck(glDeleteProgram(m_shaderProgram));
        m_shaderProgram = 0;
    }

    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_uniforms.clear();

    m_shaderProgram             = shaderProgram;
    m_modelViewMatrix           = glCheck(glGetUniformLocation(shaderProgram, "sf_ModelViewMatrix"));
    m_projectionMatrix          = glCheck(glGetUniformLocation(shaderProgram, "sf_ProjectionMatrix"));
    m_modelViewProjectionMatrix = glCheck(glGetUniformLocation(shaderProgram, "sf_ModelViewProjectionMatrix"));
    m_textureMatrix             = glCheck(glGetUniformLocation(shaderProgram, "sf_TextureMatrix"));
    m_textureEnabled            = glCheck(glGetUniformLocation(shaderProgram, "sf_TextureEnabled"));
    m_defaultTexture            = glCheck(glGetUniformLocation(shaderProgram, "sf_Texture"));

    // Force an OpenGL flush, so that the shader will appear updated
    // in all contexts immediately (solves problems in multi-threaded apps)
    glCheck(glFlush());

    return true;
}


////////////////////////////////////////////////////////////
std::string_view Shader::getDefaultVertexShaderSource()
{
    return defaultVertexShader;
}


////////////////////////////////////////////////////////////
std::string_view Shader::getDefaultFragmentShaderSource()
{
    return defaultFragmentShader;
}


////////////////////////////////////////////////////////////
void Shader::bindForDraw(const Transform& modelView, const Transform& projection, const float* textureMatrix, bool textureEnabled) const
{
    glCheck(glUseProgram(m_shaderProgram));

    bindTextures();

    if (m_currentTexture != -1)
        glCheck(glUniform1i(m_currentTexture, 0));
    if (m_defaultTexture != -1)
        glCheck(glUniform1i(m_defaultTexture, 0));

    if (m_modelViewMatrix != -1)
        glCheck(glUniformMatrix4fv(m_modelViewMatrix, 1, GL_FALSE, modelView.getMatrix()));
    if (m_projectionMatrix != -1)
        glCheck(glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, projection.getMatrix()));
    if (m_modelViewProjectionMatrix != -1)
    {
        const Transform modelViewProjection = projection * modelView;
        glCheck(glUniformMatrix4fv(m_modelViewProjectionMatrix, 1, GL_FALSE, modelViewProjection.getMatrix()));
    }
    if ((m_textureMatrix != -1) && textureMatrix)
        glCheck(glUniformMatrix4fv(m_textureMatrix, 1, GL_FALSE, textureMatrix));
    if (m_textureEnabled != -1)
        glCheck(glUniform1i(m_textureEnabled, textureEnabled ? GL_TRUE : GL_FALSE));
}


////////////////////////////////////////////////////////////
void Shader::bindTextures() const
{
    auto it = m_textures.begin();
    for (std::size_t i = 0; i < m_textures.size(); ++i)
    {
        const auto index = static_cast<GLsizei>(i + 1);
        glCheck(glUniform1i(it->first, index));
        glCheck(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
        Texture::bind(it->second);
        ++it;
    }

    // Make sure that the texture unit which is left active is the number 0
    glCheck(glActiveTexture(GL_TEXTURE0));
}


////////////////////////////////////////////////////////////
int Shader::getUniformLocation(const std::string& name)
{
    // Check the cache
    if (const auto it = m_uniforms.find(name); it != m_uniforms.end())
    {
        // Already in cache, return it
        return it->second;
    }

    // Not in cache, request the location from OpenGL
    const int location = glGetUniformLocation(m_shaderProgram, name.c_str());
    m_uniforms.try_emplace(name, location);

    if (location == -1)
        err() << "Uniform " << std::quoted(name) << " not found in shader" << std::endl;

    return location;
}

} // namespace sf
