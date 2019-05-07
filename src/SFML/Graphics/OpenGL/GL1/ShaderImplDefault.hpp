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

#ifndef SFML_SHADER_IMPL_DEFAULT_HPP
#define SFML_SHADER_IMPL_DEFAULT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/ShaderImpl.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Window/GlResource.hpp>
#include <map>
#include <string>


namespace sf
{
class Texture;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Default specialization of ShaderImpl
///
////////////////////////////////////////////////////////////
class ShaderImplDefault : public ShaderImpl, GlResource
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ShaderImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ShaderImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Bind a shader for rendering
    ///
    /// \param shader Shader to bind, can be null to use no shader
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const ShaderImplDefault* shader);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports shaders
    ///
    /// \return True if shaders are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports geometry shaders
    ///
    /// \return True if geometry shaders are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isGeometryAvailable();

private:

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p float uniform
    ///
    /// \param name Name of the uniform variable in GLSL
    /// \param x    Value of the float scalar
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, float x);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p vec2 uniform
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param vector Value of the vec2 vector
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Vec2& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p vec3 uniform
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param vector Value of the vec3 vector
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Vec3& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p vec4 uniform
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param vector Value of the vec4 vector
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Vec4& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p int uniform
    ///
    /// \param name Name of the uniform variable in GLSL
    /// \param x    Value of the int scalar
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, int x);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p ivec2 uniform
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param vector Value of the ivec2 vector
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Ivec2& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p ivec3 uniform
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param vector Value of the ivec3 vector
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Ivec3& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p ivec4 uniform
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param vector Value of the ivec4 vector
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Ivec4& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p mat3 matrix
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param matrix Value of the mat3 matrix
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Mat3& matrix);

    ////////////////////////////////////////////////////////////
    /// \brief Specify value for \p mat4 matrix
    ///
    /// \param name   Name of the uniform variable in GLSL
    /// \param matrix Value of the mat4 matrix
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Glsl::Mat4& matrix);

    ////////////////////////////////////////////////////////////
    /// \brief Specify a texture as \p sampler2D uniform
    ///
    /// \param name    Name of the texture in the shader
    /// \param texture Texture to assign
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, const Texture& texture);

    ////////////////////////////////////////////////////////////
    /// \brief Specify current texture as \p sampler2D uniform
    ///
    /// \param name Name of the texture in the shader
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, Shader::CurrentTextureType);

    ////////////////////////////////////////////////////////////
    /// \brief Specify values for \p float[] array uniform
    ///
    /// \param name        Name of the uniform variable in GLSL
    /// \param scalarArray pointer to array of \p float values
    /// \param length      Number of elements in the array
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniformArray(const std::string& name, const float* scalarArray, std::size_t length);

    ////////////////////////////////////////////////////////////
    /// \brief Specify values for \p vec2[] array uniform
    ///
    /// \param name        Name of the uniform variable in GLSL
    /// \param vectorArray pointer to array of \p vec2 values
    /// \param length      Number of elements in the array
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length);

    ////////////////////////////////////////////////////////////
    /// \brief Specify values for \p vec3[] array uniform
    ///
    /// \param name        Name of the uniform variable in GLSL
    /// \param vectorArray pointer to array of \p vec3 values
    /// \param length      Number of elements in the array
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length);

    ////////////////////////////////////////////////////////////
    /// \brief Specify values for \p vec4[] array uniform
    ///
    /// \param name        Name of the uniform variable in GLSL
    /// \param vectorArray pointer to array of \p vec4 values
    /// \param length      Number of elements in the array
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length);

    ////////////////////////////////////////////////////////////
    /// \brief Specify values for \p mat3[] array uniform
    ///
    /// \param name        Name of the uniform variable in GLSL
    /// \param matrixArray pointer to array of \p mat3 values
    /// \param length      Number of elements in the array
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length);

    ////////////////////////////////////////////////////////////
    /// \brief Specify values for \p mat4[] array uniform
    ///
    /// \param name        Name of the uniform variable in GLSL
    /// \param matrixArray pointer to array of \p mat4 values
    /// \param length      Number of elements in the array
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length);

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying OpenGL handle of the shader.
    ///
    /// \return OpenGL handle of the shader or 0 if not yet loaded
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getNativeHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Compile the shader(s) and create the program
    ///
    /// \param vertexShaderCode   Source code of the vertex shader
    /// \param geometryShaderCode Source code of the geometry shader
    /// \param fragmentShaderCode Source code of the fragment shader
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool compile(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode);

    ////////////////////////////////////////////////////////////
    /// \brief Bind all the textures used by the shader
    ///
    /// This function each texture to a different unit, and
    /// updates the corresponding variables in the shader accordingly.
    ///
    ////////////////////////////////////////////////////////////
    void bindTextures() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the location ID of a shader uniform
    ///
    /// \param name Name of the uniform variable to search
    ///
    /// \return Location ID of the uniform, or -1 if not found
    ///
    ////////////////////////////////////////////////////////////
    int getUniformLocation(const std::string& name);

    ////////////////////////////////////////////////////////////
    /// \brief RAII object to save and restore the program
    ///        binding while uniforms are being set
    ///
    /// Implementation is private in the .cpp file.
    ///
    ////////////////////////////////////////////////////////////
    struct UniformBinder;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<int, const Texture*> TextureTable;
    typedef std::map<std::string, int> UniformTable;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int m_shaderProgram;  ///< OpenGL identifier for the program
    int          m_currentTexture; ///< Location of the current texture in the shader
    TextureTable m_textures;       ///< Texture variables in the shader, mapped to their location
    UniformTable m_uniforms;       ///< Parameters location cache
};

} // namespace priv

} // namespace sf


#endif // SFML_SHADER_IMPL_DEFAULT_HPP
