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
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/GlResource.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <map>
#include <string>


namespace sf
{
class Renderer;

////////////////////////////////////////////////////////////
/// \brief Pixel/fragment shader class
///
////////////////////////////////////////////////////////////
class SFML_API Shader : GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor creates an invalid shader
    ///
    ////////////////////////////////////////////////////////////
    Shader();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Shader(const Shader& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Shader();

    ////////////////////////////////////////////////////////////
    /// \brief Load the shader from a file
    ///
    /// The source must be a text file containing a valid
    /// fragment shader in GLSL language. GLSL is a C-like
    /// language dedicated to OpenGL shaders; you'll probably
    /// need to read a good documentation for it before writing
    /// your own shaders.
    ///
    /// \param filename Path of the shader file to load
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromMemory, LoadFromStream
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the shader from a source code in memory
    ///
    /// The source code must be a valid fragment shader in
    /// GLSL language. GLSL is a C-like language dedicated
    /// to OpenGL shaders; you'll probably need to read a
    /// good documentation for it before writing your own shaders.
    ///
    /// \param shader String containing the source code of the shader
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromFile, LoadFromStream
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& shader);

    ////////////////////////////////////////////////////////////
    /// \brief Load the shader from a custom stream
    ///
    /// The source code must be a valid fragment shader in
    /// GLSL language. GLSL is a C-like language dedicated
    /// to OpenGL shaders; you'll probably need to read a
    /// good documentation for it before writing your own shaders.
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromFile, LoadFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Change a float parameter of the shader
    ///
    /// \a name is the name of the variable to change in the shader.
    /// For example:
    /// \code
    /// uniform float myparam; // this is the variable in the pixel shader
    /// \endcode
    /// \code
    /// shader.SetParameter("myparam", 5.2f);
    /// \endcode
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    Value to assign
    ///
    /// \see SetTexture, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, float x);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 2-components vector parameter of the shader
    ///
    /// \a name is the name of the variable to change in the shader.
    /// For example:
    /// \code
    /// uniform vec2 myparam; // this is the variable in the pixel shader
    /// \endcode
    /// \code
    /// shader.SetParameter("myparam", 5.2f, 6.0f);
    /// \endcode
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    First component of the value to assign
    /// \param y    Second component of the value to assign
    ///
    /// \see SetTexture, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, float x, float y);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 3-components vector parameter of the shader
    ///
    /// \a name is the name of the variable to change in the shader.
    /// For example:
    /// \code
    /// uniform vec3 myparam; // this is the variable in the pixel shader
    /// \endcode
    /// \code
    /// shader.SetParameter("myparam", 5.2f, 6.0f, -8.1f);
    /// \endcode
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    First component of the value to assign
    /// \param y    Second component of the value to assign
    /// \param z    Third component of the value to assign
    ///
    /// \see SetTexture, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 4-components vector parameter of the shader
    ///
    /// \a name is the name of the variable to change in the shader.
    /// For example:
    /// \code
    /// uniform vec4 myparam; // this is the variable in the pixel shader
    /// \endcode
    /// \code
    /// shader.SetParameter("myparam", 5.2f, 6.0f, -8.1f, 0.4f);
    /// \endcode
    ///
    /// \param name Name of the parameter in the shader
    /// \param x    First component of the value to assign
    /// \param y    Second component of the value to assign
    /// \param z    Third component of the value to assign
    /// \param w    Fourth component of the value to assign
    ///
    /// \see SetTexture, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, float x, float y, float z, float w);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 2-components vector parameter of the shader
    ///
    /// \a name is the name of the variable to change in the shader.
    /// For example:
    /// \code
    /// uniform vec2 myparam; // this is the variable in the pixel shader
    /// \endcode
    /// \code
    /// shader.SetParameter("myparam", sf::Vector2f(5.2f, 6.0f));
    /// \endcode
    ///
    /// \param name   Name of the parameter in the shader
    /// \param vector Vector to assign
    ///
    /// \see SetTexture, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, const Vector2f& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Change a 2-components vector parameter of the shader
    ///
    /// \a name is the name of the variable to change in the shader.
    /// For example:
    /// \code
    /// uniform vec3 myparam; // this is the variable in the pixel shader
    /// \endcode
    /// \code
    /// shader.SetParameter("myparam", sf::Vector3f(5.2f, 6.0f, -8.1f));
    /// \endcode
    ///
    /// \param name   Name of the parameter in the shader
    /// \param vector Vector to assign
    ///
    /// \see SetTexture, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetParameter(const std::string& name, const Vector3f& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Change a texture parameter of the shader
    ///
    /// \a name is the name of the texture to change in the shader.
    /// This function maps an external texture to the given shader
    /// variable; to use the current texture of the object being drawn,
    /// use SetCurrentTexture instead.
    /// Example:
    /// \code
    /// // These are the variables in the pixel shader
    /// uniform sampler2D the_texture;
    /// \endcode
    /// \code
    /// sf::Texture texture;
    /// ...
    /// shader.SetTexture("the_texture", texture);
    /// \endcode
    /// It is important to note that \a texture must remain alive as long
    /// as the shader uses it, no copy is made internally.
    ///
    /// \param name    Name of the texture in the shader
    /// \param texture Texture to assign
    ///
    /// \see SetParameter, SetCurrentTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetTexture(const std::string& name, const Texture& texture);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current object texture in the shader
    ///
    /// This function maps a shader texture variable to the
    /// texture of the object being drawn.
    /// Example:
    /// \code
    /// // This is the variable in the pixel shader
    /// uniform sampler2D current;
    /// \endcode
    /// \code
    /// shader.SetCurrentTexture("current");
    /// \endcode
    ///
    /// \param name Name of the texture in the shader
    ///
    /// \see SetParameter, SetTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetCurrentTexture(const std::string& name);

    ////////////////////////////////////////////////////////////
    /// \brief Bind the shader for rendering (activate it)
    ///
    /// This function is normally for internal use only, unless
    /// you want to use the shader with a custom OpenGL rendering
    /// instead of a SFML drawable.
    /// \code
    /// window.SetActive();
    /// shader.Bind();
    /// ... render OpenGL geometry ...
    /// shader.Unbind();
    /// \endcode
    ///
    /// \see Unbind
    ///
    ////////////////////////////////////////////////////////////
    void Bind() const;

    ////////////////////////////////////////////////////////////
    /// \brief Unbind the shader (deactivate it)
    ///
    /// This function is normally for internal use only, unless
    /// you want to use the shader with a custom OpenGL rendering
    /// instead of a SFML drawable.
    ///
    /// \see Bind
    ///
    ////////////////////////////////////////////////////////////
    void Unbind() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Shader& operator =(const Shader& right);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports shaders
    ///
    /// This function should always be called before using
    /// the shader features. If it returns false, then
    /// any attempt to use sf::Shader will fail.
    ///
    /// \return True if shaders are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool IsAvailable();

private :

    friend class Renderer;

    ////////////////////////////////////////////////////////////
    /// \brief Create the program and attach the shaders
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    bool CompileProgram();

    ////////////////////////////////////////////////////////////
    /// \brief Bind all the textures used by the shader
    ///
    /// This function each texture to a different unit, and
    /// updates the corresponding variables in the shader accordingly.
    ///
    ////////////////////////////////////////////////////////////
    void BindTextures() const;

    ////////////////////////////////////////////////////////////
    /// \brief Make sure that the shader is ready to be used
    ///
    /// This function is called by the Renderer class, to make
    /// sure that the shader's parameters are properly applied
    /// even when Use() is not called due to internal optimizations.
    ///
    ////////////////////////////////////////////////////////////
    void Use() const;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<int, const Texture*> TextureTable;

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


////////////////////////////////////////////////////////////
/// \class sf::Shader
/// \ingroup graphics
///
/// Pixel shaders (or fragment shaders) are programs written
/// using a specific language, executed directly by the
/// graphics card and allowing to apply per-pixel real-time
/// operations to the rendered entities.
///
/// Pixel shaders are written in GLSL, which is a C-like
/// language dedicated to OpenGL shaders. You'll probably
/// need to learn its basics before writing your own shaders
/// for SFML.
///
/// Like any C/C++ program, a shader has its own variables
/// that you can set from your C++ application. sf::Shader
/// handles 3 different types of variables:
/// \li floats
/// \li vectors (2, 3 or 4 components)
/// \li textures
///
/// The value of the variables can be changed at any time
/// with either Shader::SetParameter or Shader::SetTexture:
/// \code
/// shader.SetParameter("offset", 2.f);
/// shader.SetParameter("color", 0.5f, 0.8f, 0.3f);
/// shader.SetTexture("overlay", texture); // texture is a sf::Texture
/// shader.SetCurrentTexture("texture");
/// \endcode
///
/// Shader::SetCurrentTexture maps the given texture variable
/// to the current texture of the object being drawn.
///
/// To apply a shader to a drawable, you must pass it as an
/// additional parameter to the Draw function:
/// \code
/// window.Draw(sprite, shader);
/// \endcode
///
/// Shaders can be used on any drawable, but they are mainly
/// made for sf::Sprite. Using a shader on a sf::String is more
/// limited, because the texture of the text is not the
/// actual text that you see on screen, it is a big texture
/// containing all the characters of the font in an arbitrary
/// order. Thus, texture lookups on pixels other than the current
/// one may not give you the expected result. Using a shader
/// with sf::Shape is even more limited, as shapes don't use
/// any texture.
///
/// Shaders can also be used to apply global post-effects to the
/// current contents of the target (like the old sf::PostFx class
/// in SFML 1). This can be done in two different ways:
/// \li draw everything to a sf::RenderTexture, then draw it to
///     the main target using the shader
/// \li draw everything directly to the main target, then use
///     sf::Texture::Update(Window&) to copy its contents to a texture
///     and draw it to the main target using the shader
///
/// The first technique is more optimized because it doesn't involve
/// retrieving the target's pixels to system memory, but the
/// second one doesn't impact the rendering process and can be
/// easily inserted anywhere.
///
/// Like sf::Texture that can be used as a raw OpenGL texture,
/// sf::Shader can also be used directly as a raw fragment
/// shader for custom OpenGL geometry.
/// \code
/// window.SetActive();
/// shader.Bind();
/// ... render OpenGL geometry ...
/// shader.Unbind();
/// \endcode
///
////////////////////////////////////////////////////////////
