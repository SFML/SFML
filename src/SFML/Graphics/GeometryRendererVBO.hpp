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

#ifndef SFML_GEOMETRYRENDERERVBO_HPP
#define SFML_GEOMETRYRENDERERVBO_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/GeometryRenderer.hpp>
#include <SFML/Graphics/GLCheck.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Geometry renderer using vertex buffer objects
///
////////////////////////////////////////////////////////////
class GeometryRendererVBO : public GeometryRenderer
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Check if this implementation is supported by the system
    ///
    /// \return True if the vertex buffer objects renderer is supported
    ///
    ////////////////////////////////////////////////////////////
    static bool IsSupported();

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    GeometryRendererVBO();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~GeometryRendererVBO();

public :

    ////////////////////////////////////////////////////////////
    /// \brief Prepare the geometry for rendering
    ///
    /// This function is called once before all the triangles
    /// are rendered.
    ///
    /// \param vertices Pointer to the vertex array
    /// \param verticesCount Number of vertices to render
    /// \param indices Pointer to the index array
    /// \param indicesCount Number of indices to render
    ///
    ////////////////////////////////////////////////////////////
    virtual void Begin(const float* vertices, std::size_t verticesCount, const unsigned int* indices, std::size_t indicesCount);

    ////////////////////////////////////////////////////////////
    /// \brief Stop rendering geometry
    ///
    /// This function is called once after all the triangles
    /// have been rendered.
    ///
    ////////////////////////////////////////////////////////////
    virtual void End();

    ////////////////////////////////////////////////////////////
    /// \brief Render a chunk of triangles
    ///
    /// The primitives are rendered as a list of triangles (no strip or fan).
    ///
    /// \param start Index in the indices array of the first index to be rendered
    /// \param count Number of indices to be rendered
    ///
    ////////////////////////////////////////////////////////////
    virtual void RenderTriangles(std::size_t start, std::size_t count);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    GLuint      myVertexBuffer;
    GLuint      myIndexBuffer;
    std::size_t myVertexBufferSize;
    std::size_t myIndexBufferSize;
};

} // namespace priv

} // namespace sf


#endif // SFML_GEOMETRYRENDERERVBO_HPP
