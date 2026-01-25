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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/ShaderImpl.hpp>

#include <SFML/Window/GlResource.hpp>

#include <string>
#include <unordered_map>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Default OpenGL implementation of shaders
///
////////////////////////////////////////////////////////////
class ShaderImplDefault : public ShaderImpl
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
    ~ShaderImplDefault() override;

    ////////////////////////////////////////////////////////////
    /// \brief Compile the shader(s) and create the program
    ///
    /// \param vertexShaderCode   Source code of the vertex shader
    /// \param geometryShaderCode Source code of the geometry shader
    /// \param fragmentShaderCode Source code of the fragment shader
    ///
    /// \return `true` on success, `false` if any error happened
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool compile(std::string_view vertexShaderCode,
                               std::string_view geometryShaderCode,
                               std::string_view fragmentShaderCode) override;

    ////////////////////////////////////////////////////////////
    /// \brief Set a uniform
    ///
    /// \param name     Name of the uniform variable
    /// \param x        Value to set
    ///
    ////////////////////////////////////////////////////////////
    void setUniform(const std::string& name, float x) override;
    void setUniform(const std::string& name, int x) override;
    void setUniform(const std::string& name, bool x) override;
    void setUniform(const std::string& name, const Glsl::Vec2& v) override;
    void setUniform(const std::string& name, const Glsl::Vec3& v) override;
    void setUniform(const std::string& name, const Glsl::Vec4& v) override;
    void setUniform(const std::string& name, const Glsl::Ivec2& v) override;
    void setUniform(const std::string& name, const Glsl::Ivec3& v) override;
    void setUniform(const std::string& name, const Glsl::Ivec4& v) override;
    void setUniform(const std::string& name, const Glsl::Bvec2& v) override;
    void setUniform(const std::string& name, const Glsl::Bvec3& v) override;
    void setUniform(const std::string& name, const Glsl::Bvec4& v) override;
    void setUniform(const std::string& name, const Glsl::Mat3& matrix) override;
    void setUniform(const std::string& name, const Glsl::Mat4& matrix) override;
    void setUniform(const std::string& name, const Texture& texture) override;
    void setUniform(const std::string& name, Shader::CurrentTextureType) override;
    void setUniformArray(const std::string& name, const float* scalarArray, std::size_t length) override;
    void setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length) override;
    void setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length) override;
    void setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length) override;
    void setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length) override;
    void setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length) override;

    ////////////////////////////////////////////////////////////
    /// \brief Bind all the textures used by the shader
    ///
    ////////////////////////////////////////////////////////////
    void bindTextures() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying native handle of the shader
    ///
    /// \return Native handle of the shader or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned int getNativeHandle() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Check if shaders are available on the system
    ///
    /// \return True if shaders are available, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Check if geometry shaders are available on the system
    ///
    /// \return True if geometry shaders are available, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool isGeometryAvailable();

private:
    friend class ShaderImpl;

    ////////////////////////////////////////////////////////////
    /// \brief Get the location of a uniform variable by its name
    ///
    /// \param name Name of the uniform variable to find
    ///
    /// \return Location of the uniform variable in the shader
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] int getUniformLocation(const std::string& name);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    using TextureTable = std::unordered_map<int, const Texture*>;
    using UniformTable = std::unordered_map<std::string, int>;

    unsigned int m_shaderProgram{}; //!< Internal-only OpenGL identifier for the program
    int          m_currentTexture{-1}; //!< Location of the special "current texture" uniform
    TextureTable m_textures;           //!< Texture variables locations, and their assigned textures
    UniformTable m_uniforms;           //!< Locations of the uniform variables
};

} // namespace sf::priv
