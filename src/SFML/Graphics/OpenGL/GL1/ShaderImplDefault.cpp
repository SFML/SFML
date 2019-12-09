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
#include <SFML/Graphics/OpenGL/GL1/ShaderImplDefault.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/OpenGL/GLCheck.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <vector>


#if defined(SFML_SYSTEM_MACOS) || defined(SFML_SYSTEM_IOS)

    #define castToGlHandle(x) reinterpret_cast<GLEXT_GLhandle>(static_cast<ptrdiff_t>(x))
    #define castFromGlHandle(x) static_cast<unsigned int>(reinterpret_cast<ptrdiff_t>(x))

#else

    #define castToGlHandle(x) (x)
    #define castFromGlHandle(x) (x)

#endif

namespace
{
    sf::Mutex maxTextureUnitsMutex;
    sf::Mutex isAvailableMutex;

    GLint checkMaxTextureUnits()
    {
        GLint maxUnits = 0;
        glCheck(glGetIntegerv(GLEXT_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits));

        return maxUnits;
    }

    // Retrieve the maximum number of texture units available
    GLint getMaxTextureUnits()
    {
        // TODO: Remove this lock when it becomes unnecessary in C++11
        sf::Lock lock(maxTextureUnitsMutex);

        static GLint maxUnits = checkMaxTextureUnits();

        return maxUnits;
    }

    // Transforms an array of 2D vectors into a contiguous array of scalars
    template <typename T>
    std::vector<T> flatten(const sf::Vector2<T>* vectorArray, std::size_t length)
    {
        const std::size_t vectorSize = 2;

        std::vector<T> contiguous(vectorSize * length);
        for (std::size_t i = 0; i < length; ++i)
        {
            contiguous[vectorSize * i]     = vectorArray[i].x;
            contiguous[vectorSize * i + 1] = vectorArray[i].y;
        }

        return contiguous;
    }

    // Transforms an array of 3D vectors into a contiguous array of scalars
    template <typename T>
    std::vector<T> flatten(const sf::Vector3<T>* vectorArray, std::size_t length)
    {
        const std::size_t vectorSize = 3;

        std::vector<T> contiguous(vectorSize * length);
        for (std::size_t i = 0; i < length; ++i)
        {
            contiguous[vectorSize * i]     = vectorArray[i].x;
            contiguous[vectorSize * i + 1] = vectorArray[i].y;
            contiguous[vectorSize * i + 2] = vectorArray[i].z;
        }

        return contiguous;
    }

    // Transforms an array of 4D vectors into a contiguous array of scalars
    template <typename T>
    std::vector<T> flatten(const sf::priv::Vector4<T>* vectorArray, std::size_t length)
    {
        const std::size_t vectorSize = 4;

        std::vector<T> contiguous(vectorSize * length);
        for (std::size_t i = 0; i < length; ++i)
        {
            contiguous[vectorSize * i]     = vectorArray[i].x;
            contiguous[vectorSize * i + 1] = vectorArray[i].y;
            contiguous[vectorSize * i + 2] = vectorArray[i].z;
            contiguous[vectorSize * i + 3] = vectorArray[i].w;
        }

        return contiguous;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
struct ShaderImplDefault::UniformBinder : private sf::NonCopyable
{
    ////////////////////////////////////////////////////////////
    /// \brief Constructor: set up state before uniform is set
    ///
    ////////////////////////////////////////////////////////////
    UniformBinder(ShaderImplDefault& shader, const std::string& name) :
    savedProgram(0),
    currentProgram(castToGlHandle(shader.m_shaderProgram)),
    location(-1)
    {
        if (currentProgram)
        {
            // Enable program object
            glCheck(savedProgram = GLEXT_glGetHandle(GLEXT_GL_PROGRAM_OBJECT));
            if (currentProgram != savedProgram)
                glCheck(GLEXT_glUseProgramObject(currentProgram));

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
        if (currentProgram && (currentProgram != savedProgram))
            glCheck(GLEXT_glUseProgramObject(savedProgram));
    }

    TransientContextLock lock;           ///< Lock to keep context active while uniform is bound
    GLEXT_GLhandle       savedProgram;   ///< Handle to the previously active program object
    GLEXT_GLhandle       currentProgram; ///< Handle to the program object of the modified sf::Shader instance
    GLint                location;       ///< Uniform location, used by the surrounding sf::Shader code
};


////////////////////////////////////////////////////////////
ShaderImplDefault::ShaderImplDefault() :
m_shaderProgram (0),
m_currentTexture(-1),
m_textures      (),
m_uniforms      ()
{
}


////////////////////////////////////////////////////////////
ShaderImplDefault::~ShaderImplDefault()
{
    TransientContextLock lock;

    // Destroy effect program
    if (m_shaderProgram)
        glCheck(GLEXT_glDeleteObject(castToGlHandle(m_shaderProgram)));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, float x)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform1f(binder.location, x));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Vec2& v)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform2f(binder.location, v.x, v.y));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Vec3& v)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform3f(binder.location, v.x, v.y, v.z));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Vec4& v)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform4f(binder.location, v.x, v.y, v.z, v.w));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, int x)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform1i(binder.location, x));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Ivec2& v)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform2i(binder.location, v.x, v.y));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Ivec3& v)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform3i(binder.location, v.x, v.y, v.z));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Ivec4& v)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform4i(binder.location, v.x, v.y, v.z, v.w));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Mat3& matrix)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniformMatrix3fv(binder.location, 1, GL_FALSE, matrix.array));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Glsl::Mat4& matrix)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniformMatrix4fv(binder.location, 1, GL_FALSE, matrix.array));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniform(const std::string& name, const Texture& texture)
{
    if (m_shaderProgram)
    {
        TransientContextLock lock;

        // Find the location of the variable in the shader
        int location = getUniformLocation(name);
        if (location != -1)
        {
            // Store the location -> texture mapping
            TextureTable::iterator it = m_textures.find(location);
            if (it == m_textures.end())
            {
                // New entry, make sure there are enough texture units
                GLint maxUnits = getMaxTextureUnits();
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
void ShaderImplDefault::setUniform(const std::string& name, Shader::CurrentTextureType)
{
    if (m_shaderProgram)
    {
        TransientContextLock lock;

        // Find the location of the variable in the shader
        m_currentTexture = getUniformLocation(name);
    }
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniformArray(const std::string& name, const float* scalarArray, std::size_t length)
{
    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform1fv(binder.location, static_cast<GLsizei>(length), scalarArray));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length)
{
    std::vector<float> contiguous = flatten(vectorArray, length);

    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform2fv(binder.location, static_cast<GLsizei>(length), &contiguous[0]));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length)
{
    std::vector<float> contiguous = flatten(vectorArray, length);

    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform3fv(binder.location, static_cast<GLsizei>(length), &contiguous[0]));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length)
{
    std::vector<float> contiguous = flatten(vectorArray, length);

    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniform4fv(binder.location, static_cast<GLsizei>(length), &contiguous[0]));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length)
{
    const std::size_t matrixSize = 3 * 3;

    std::vector<float> contiguous(matrixSize * length);
    for (std::size_t i = 0; i < length; ++i)
        priv::copyMatrix(matrixArray[i].array, matrixSize, &contiguous[matrixSize * i]);

    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniformMatrix3fv(binder.location, static_cast<GLsizei>(length), GL_FALSE, &contiguous[0]));
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length)
{
    const std::size_t matrixSize = 4 * 4;

    std::vector<float> contiguous(matrixSize * length);
    for (std::size_t i = 0; i < length; ++i)
        priv::copyMatrix(matrixArray[i].array, matrixSize, &contiguous[matrixSize * i]);

    UniformBinder binder(*this, name);
    if (binder.location != -1)
        glCheck(GLEXT_glUniformMatrix4fv(binder.location, static_cast<GLsizei>(length), GL_FALSE, &contiguous[0]));
}


////////////////////////////////////////////////////////////
unsigned int ShaderImplDefault::getNativeHandle() const
{
    return m_shaderProgram;
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::bind(const ShaderImplDefault* shader)
{
    TransientContextLock lock;

    // Make sure that we can use shaders
    if (!isAvailable())
    {
        err() << "Failed to bind or unbind shader: your system doesn't support shaders "
              << "(you should test ShaderImplDefault::isAvailable() before trying to use the Shader class)" << std::endl;
        return;
    }

    if (shader && shader->m_shaderProgram)
    {
        // Enable the program
        glCheck(GLEXT_glUseProgramObject(castToGlHandle(shader->m_shaderProgram)));

        // Bind the textures
        shader->bindTextures();

        // Bind the current texture
        if (shader->m_currentTexture != -1)
            glCheck(GLEXT_glUniform1i(shader->m_currentTexture, 0));
    }
    else
    {
        // Bind no shader
        glCheck(GLEXT_glUseProgramObject(0));
    }
}


////////////////////////////////////////////////////////////
bool ShaderImplDefault::isAvailable()
{
    Lock lock(isAvailableMutex);

    static bool checked = false;
    static bool available = false;

    if (!checked)
    {
        checked = true;

        TransientContextLock contextLock;

        // Make sure that extensions are initialized
        sf::priv::ensureExtensionsInit();

        available = GLEXT_multitexture         &&
                    GLEXT_shading_language_100 &&
                    GLEXT_shader_objects       &&
                    GLEXT_vertex_shader        &&
                    GLEXT_fragment_shader;
    }

    return available;
}


////////////////////////////////////////////////////////////
bool ShaderImplDefault::isGeometryAvailable()
{
    Lock lock(isAvailableMutex);

    static bool checked = false;
    static bool available = false;

    if (!checked)
    {
        checked = true;

        TransientContextLock contextLock;

        // Make sure that extensions are initialized
        sf::priv::ensureExtensionsInit();

        available = isAvailable() && GLEXT_geometry_shader4;
    }

    return available;
}


////////////////////////////////////////////////////////////
bool ShaderImplDefault::compile(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode)
{
    TransientContextLock lock;

    // First make sure that we can use shaders
    if (!isAvailable())
    {
        err() << "Failed to create a shader: your system doesn't support shaders "
              << "(you should test ShaderImplDefault::isAvailable() before trying to use the Shader class)" << std::endl;
        return false;
    }

    // Make sure we can use geometry shaders
    if (geometryShaderCode && !isGeometryAvailable())
    {
        err() << "Failed to create a shader: your system doesn't support geometry shaders "
              << "(you should test ShaderImplDefault::isGeometryAvailable() before trying to use geometry shaders)" << std::endl;
        return false;
    }

    // Destroy the shader if it was already created
    if (m_shaderProgram)
    {
        glCheck(GLEXT_glDeleteObject(castToGlHandle(m_shaderProgram)));
        m_shaderProgram = 0;
    }

    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_uniforms.clear();

    // Create the program
    GLEXT_GLhandle shaderProgram;
    glCheck(shaderProgram = GLEXT_glCreateProgramObject());

    // Create the vertex shader if needed
    if (vertexShaderCode)
    {
        // Create and compile the shader
        GLEXT_GLhandle vertexShader;
        glCheck(vertexShader = GLEXT_glCreateShaderObject(GLEXT_GL_VERTEX_SHADER));
        glCheck(GLEXT_glShaderSource(vertexShader, 1, &vertexShaderCode, NULL));
        glCheck(GLEXT_glCompileShader(vertexShader));

        // Check the compile log
        GLint success;
        glCheck(GLEXT_glGetObjectParameteriv(vertexShader, GLEXT_GL_OBJECT_COMPILE_STATUS, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(GLEXT_glGetInfoLog(vertexShader, sizeof(log), 0, log));
            err() << "Failed to compile vertex shader:" << std::endl
                  << log << std::endl;
            glCheck(GLEXT_glDeleteObject(vertexShader));
            glCheck(GLEXT_glDeleteObject(shaderProgram));
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(GLEXT_glAttachObject(shaderProgram, vertexShader));
        glCheck(GLEXT_glDeleteObject(vertexShader));
    }

    // Create the geometry shader if needed
    if (geometryShaderCode)
    {
        // Create and compile the shader
        GLEXT_GLhandle geometryShader = GLEXT_glCreateShaderObject(GLEXT_GL_GEOMETRY_SHADER);
        glCheck(GLEXT_glShaderSource(geometryShader, 1, &geometryShaderCode, NULL));
        glCheck(GLEXT_glCompileShader(geometryShader));

        // Check the compile log
        GLint success;
        glCheck(GLEXT_glGetObjectParameteriv(geometryShader, GLEXT_GL_OBJECT_COMPILE_STATUS, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(GLEXT_glGetInfoLog(geometryShader, sizeof(log), 0, log));
            err() << "Failed to compile geometry shader:" << std::endl
                  << log << std::endl;
            glCheck(GLEXT_glDeleteObject(geometryShader));
            glCheck(GLEXT_glDeleteObject(shaderProgram));
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(GLEXT_glAttachObject(shaderProgram, geometryShader));
        glCheck(GLEXT_glDeleteObject(geometryShader));
    }

    // Create the fragment shader if needed
    if (fragmentShaderCode)
    {
        // Create and compile the shader
        GLEXT_GLhandle fragmentShader;
        glCheck(fragmentShader = GLEXT_glCreateShaderObject(GLEXT_GL_FRAGMENT_SHADER));
        glCheck(GLEXT_glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL));
        glCheck(GLEXT_glCompileShader(fragmentShader));

        // Check the compile log
        GLint success;
        glCheck(GLEXT_glGetObjectParameteriv(fragmentShader, GLEXT_GL_OBJECT_COMPILE_STATUS, &success));
        if (success == GL_FALSE)
        {
            char log[1024];
            glCheck(GLEXT_glGetInfoLog(fragmentShader, sizeof(log), 0, log));
            err() << "Failed to compile fragment shader:" << std::endl
                  << log << std::endl;
            glCheck(GLEXT_glDeleteObject(fragmentShader));
            glCheck(GLEXT_glDeleteObject(shaderProgram));
            return false;
        }

        // Attach the shader to the program, and delete it (not needed anymore)
        glCheck(GLEXT_glAttachObject(shaderProgram, fragmentShader));
        glCheck(GLEXT_glDeleteObject(fragmentShader));
    }

    // Link the program
    glCheck(GLEXT_glLinkProgram(shaderProgram));

    // Check the link log
    GLint success;
    glCheck(GLEXT_glGetObjectParameteriv(shaderProgram, GLEXT_GL_OBJECT_LINK_STATUS, &success));
    if (success == GL_FALSE)
    {
        char log[1024];
        glCheck(GLEXT_glGetInfoLog(shaderProgram, sizeof(log), 0, log));
        err() << "Failed to link shader:" << std::endl
              << log << std::endl;
        glCheck(GLEXT_glDeleteObject(shaderProgram));
        return false;
    }

    m_shaderProgram = castFromGlHandle(shaderProgram);

    // Force an OpenGL flush, so that the shader will appear updated
    // in all contexts immediately (solves problems in multi-threaded apps)
    glCheck(glFlush());

    return true;
}


////////////////////////////////////////////////////////////
void ShaderImplDefault::bindTextures() const
{
    TextureTable::const_iterator it = m_textures.begin();
    for (std::size_t i = 0; i < m_textures.size(); ++i)
    {
        GLint index = static_cast<GLsizei>(i + 1);
        glCheck(GLEXT_glUniform1i(it->first, index));
        glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0 + index));
        Texture::bind(it->second);
        ++it;
    }

    // Make sure that the texture unit which is left active is the number 0
    glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0));
}


////////////////////////////////////////////////////////////
int ShaderImplDefault::getUniformLocation(const std::string& name)
{
    // Check the cache
    UniformTable::const_iterator it = m_uniforms.find(name);
    if (it != m_uniforms.end())
    {
        // Already in cache, return it
        return it->second;
    }
    else
    {
        // Not in cache, request the location from OpenGL
        int location = GLEXT_glGetUniformLocation(castToGlHandle(m_shaderProgram), name.c_str());
        m_uniforms.insert(std::make_pair(name, location));

        if (location == -1)
            err() << "Uniform \"" << name << "\" not found in shader" << std::endl;

        return location;
    }
}

} // namespace priv

} // namespace sf
