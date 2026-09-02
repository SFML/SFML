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
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/GlResource.hpp>

#include <string>
#include <string_view>
#include <unordered_map>



namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for shader implementations
///
////////////////////////////////////////////////////////////
class ShaderImpl : public GlResource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~ShaderImpl() = default;

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
    [[nodiscard]] virtual bool compile(std::string_view vertexShaderCode,
                                       std::string_view geometryShaderCode,
                                       std::string_view fragmentShaderCode) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Set a uniform
    ///
    /// \param name     Name of the uniform variable
    /// \param x        Value to set
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUniform(const std::string& name, float x) = 0;
    virtual void setUniform(const std::string& name, int x) = 0;
    virtual void setUniform(const std::string& name, bool x) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Vec2& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Vec3& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Vec4& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Ivec2& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Ivec3& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Ivec4& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Bvec2& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Bvec3& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Bvec4& v) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Mat3& matrix) = 0;
    virtual void setUniform(const std::string& name, const Glsl::Mat4& matrix) = 0;
    virtual void setUniform(const std::string& name, const Texture& texture) = 0;
    virtual void setUniform(const std::string& name, Shader::CurrentTextureType) = 0;
    virtual void setUniformArray(const std::string& name, const float* scalarArray, std::size_t length) = 0;
    virtual void setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length) = 0;
    virtual void setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length) = 0;
    virtual void setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length) = 0;
    virtual void setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length) = 0;
    virtual void setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Bind all the textures used by the shader
    ///
    ////////////////////////////////////////////////////////////
    virtual void bindTextures() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying native handle of the shader
    ///
    /// \return Native handle of the shader or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual unsigned int getNativeHandle() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Bind a shader for rendering
    ///
    /// \param shaderImpl Pointer to the shader implementation to bind, can be null to use no shader
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const ShaderImpl* shaderImpl);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports shaders
    ///
    /// \return `true` if shaders are supported, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports geometry shaders
    ///
    /// \return `true` if geometry shaders are supported, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool isGeometryAvailable();
};

} // namespace sf::priv

