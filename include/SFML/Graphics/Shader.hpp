////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_SHADER_HPP
#define SFML_SHADER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <map>
#include <string>


namespace sf
{
////////////////////////////////////////////////////////////
/// Wrapper for pixel shaders
////////////////////////////////////////////////////////////
class SFML_API Shader
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Shader();

    ////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy : Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Shader(const Shader& copy);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Shader();

    ////////////////////////////////////////////////////////////
    /// Load the shader from a file
    ///
    /// \param filename : Path of the shader file to load
    ///
    /// \return True on success
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// Load the shader from a text in memory
    ///
    /// \param effect : String containing the code of the shader
    ///
    /// \return True on success
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& shader);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the shader (1 float)
    ///
    /// \param name : Name of the parameter in the shader
    /// \param x :    Value to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, float x);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the shader (2 floats)
    ///
    /// \param name : Name of the parameter in the shader
    /// \param x, y : Values to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& Name, float x, float y);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the shader (3 floats)
    ///
    /// \param name :    Name of the parameter in the shader
    /// \param x, y, z : Values to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& Name, float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the shader (4 floats)
    ///
    /// \param name :       Name of the parameter in the shader
    /// \param x, y, z, w : Values to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& Name, float x, float y, float z, float w);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the shader (1 vector2)
    ///
    /// \param name : Name of the parameter in the shader
    /// \param v :    Value to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, const Vector2f& v);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the shader (1 vector3)
    ///
    /// \param name : Name of the parameter in the shader
    /// \param v :    Value to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, const Vector3f& v);

    ////////////////////////////////////////////////////////////
    /// Set a texture parameter
    ///
    /// \param name :    Name of the texture in the shader
    /// \param texture : Image to set (pass Shader::CurrentTexture to use the current texture)
    ///
    ////////////////////////////////////////////////////////////
    void SetTexture(const std::string& name, const Image& texture);

    ////////////////////////////////////////////////////////////
    /// Bind the shader for rendering
    ///
    ////////////////////////////////////////////////////////////
    void Bind() const;

    ////////////////////////////////////////////////////////////
    /// Unbind the shader
    ///
    ////////////////////////////////////////////////////////////
    void Unbind() const;

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Shader& operator =(const Shader& right);

    ////////////////////////////////////////////////////////////
    /// Tell whether or not the system supports shaders
    ///
    /// \return True if the system can use shaders
    ///
    ////////////////////////////////////////////////////////////
    static bool IsAvailable();

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Image CurrentTexture; ///< Special image representing the texture used by the object being drawn

private :

    ////////////////////////////////////////////////////////////
    /// Create the program and attach the shaders
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    bool CompileProgram();

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<int, const Image*> TextureTable;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int myShaderProgram;  ///< OpenGL identifier for the program
    int          myCurrentTexture; ///< Location of the current texture in the shader
    TextureTable myTextures;       ///< Texture variables in the shader, mapped to their location
    std::string  myFragmentShader; ///< Fragment shader source code
};

} // namespace sf


#endif // SFML_SHADER_HPP
