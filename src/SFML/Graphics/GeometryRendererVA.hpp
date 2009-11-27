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

#ifndef SFML_GEOMETRYRENDERERVA_HPP
#define SFML_GEOMETRYRENDERERVA_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/GeometryRenderer.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Geometry renderer using vertex arrays
///
////////////////////////////////////////////////////////////
class GeometryRendererVA : public GeometryRenderer
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Check if this implementation is supported by the system
    ///
    /// \return True if the vertex array renderer is supported
    ///
    ////////////////////////////////////////////////////////////
    static bool IsSupported();

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    GeometryRendererVA();

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
    virtual void Begin(const float* vertices, std::size_t verticesCount, const std::size_t* indices, std::size_t indicesCount);

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
    const std::size_t* myIndices; ///< Pointer to the indices to render
};

} // namespace priv

} // namespace sf


#endif // SFML_GEOMETRYRENDERERVA_HPP
