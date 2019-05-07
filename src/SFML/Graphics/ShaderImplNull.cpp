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
#include <SFML/Graphics/ShaderImplNull.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, float x)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Vec2& v)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Vec3& v)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Vec4& v)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, int x)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Ivec2& v)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Ivec3& v)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Ivec4& v)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Mat3& matrix)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Glsl::Mat4& matrix)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, const Texture& texture)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniform(const std::string& name, Shader::CurrentTextureType)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniformArray(const std::string& name, const float* scalarArray, std::size_t length)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniformArray(const std::string& name, const Glsl::Vec2* vectorArray, std::size_t length)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniformArray(const std::string& name, const Glsl::Vec3* vectorArray, std::size_t length)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniformArray(const std::string& name, const Glsl::Vec4* vectorArray, std::size_t length)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniformArray(const std::string& name, const Glsl::Mat3* matrixArray, std::size_t length)
{
}


////////////////////////////////////////////////////////////
void ShaderImplNull::setUniformArray(const std::string& name, const Glsl::Mat4* matrixArray, std::size_t length)
{
}


////////////////////////////////////////////////////////////
unsigned int ShaderImplNull::getNativeHandle() const
{
    return 0;
}


////////////////////////////////////////////////////////////
void ShaderImplNull::bind(const ShaderImplNull* shader)
{
}


////////////////////////////////////////////////////////////
bool ShaderImplNull::isAvailable()
{
    return false;
}


////////////////////////////////////////////////////////////
bool ShaderImplNull::isGeometryAvailable()
{
    return false;
}


////////////////////////////////////////////////////////////
bool ShaderImplNull::compile(const char* vertexShaderCode, const char* geometryShaderCode, const char* fragmentShaderCode)
{
    return false;
}

} // namespace priv

} // namespace sf
