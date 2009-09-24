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

#ifndef SFML_POSTFX_HPP
#define SFML_POSTFX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <istream>
#include <map>
#include <string>


namespace sf
{
////////////////////////////////////////////////////////////
/// PostFX is used to apply a post effect to a window
////////////////////////////////////////////////////////////
class SFML_API PostFX : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    PostFX();

    ////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy : Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    PostFX(const PostFX& copy);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~PostFX();

    ////////////////////////////////////////////////////////////
    /// Load the effect from a file
    ///
    /// \param filename : Path of the effect file to load
    ///
    /// \return True on success
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// Load the effect from a text in memory
    ///
    /// \param effect : String containing the code of the effect
    ///
    /// \return True on success
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& effect);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the effect (1 float)
    ///
    /// \param name : Name of the parameter in the effect
    /// \param x :    Value to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, float x);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the effect (2 floats)
    ///
    /// \param name : Name of the parameter in the effect
    /// \param x, y : Values to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& Name, float x, float y);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the effect (3 floats)
    ///
    /// \param name :    Name of the parameter in the effect
    /// \param x, y, z : Values to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& Name, float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// Change a parameter of the effect (4 floats)
    ///
    /// \param name :       Name of the parameter in the effect
    /// \param x, y, z, w : Values to assign
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& Name, float x, float y, float Z, float w);

    ////////////////////////////////////////////////////////////
    /// Set a texture parameter
    ///
    /// \param name :    Name of the texture in the effect
    /// \param texture : Image to set (pass NULL to use content of current framebuffer)
    ///
    ////////////////////////////////////////////////////////////
    void SetTexture(const std::string& name, const Image* texture);

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param other : Instance to assign
    ///
    /// \return Reference to the post-effect
    ///
    ////////////////////////////////////////////////////////////
    PostFX& operator =(const PostFX& other);

    ////////////////////////////////////////////////////////////
    /// Tell whether or not the system supports post-effects
    ///
    /// \return True if the system can use post-effects
    ///
    ////////////////////////////////////////////////////////////
    static bool CanUsePostFX();

protected :

    ////////////////////////////////////////////////////////////
    /// /see Drawable::Render
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& target, RenderQueue& queue) const;

private :

    ////////////////////////////////////////////////////////////
    /// Preprocess a SFML effect file
    /// to convert it to a valid GLSL fragment shader
    ///
    /// \param file : Stream containing the code to process
    ///
    /// \return Valid fragment shader source code
    ///
    ////////////////////////////////////////////////////////////
    static std::string PreprocessEffect(std::istream& file);

    ////////////////////////////////////////////////////////////
    /// Create the program and attach the shaders
    ///
    ////////////////////////////////////////////////////////////
    void CreateProgram();

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<std::string, const Image*> TextureTable;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int  myShaderProgram;  ///< OpenGL identifier for the program
    TextureTable  myTextures;       ///< Texture variables in the effect
    std::string   myFragmentShader; ///< Fragment shader source code
    mutable Image myFrameBuffer;    ///< Texture containing the current frame buffer
};

} // namespace sf


#endif // SFML_POSTFX_HPP
