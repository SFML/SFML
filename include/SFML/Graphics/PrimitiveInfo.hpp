////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_PRIMITIVEINFO_HPP
#define SFML_PRIMITIVEINFO_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <functional>
#include <vector>

namespace sf
{
class Vertex;

////////////////////////////////////////////////////////////
/// \brief Abstract base class for objects that provide access
///        to primitive information
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API PrimitiveInfo
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~PrimitiveInfo() {}

    ////////////////////////////////////////////////////////////
    /// \brief Callback that uses constant vertex info
    ///
    ////////////////////////////////////////////////////////////
    typedef std::function<void(const Vertex* vertices, std::size_t vertexCount, PrimitiveType type)> PrimitiveCallback;

    ////////////////////////////////////////////////////////////
    /// \brief Callback that uses mutable vertex info
    ///
    ////////////////////////////////////////////////////////////
    typedef std::function<void(std::vector<Vertex>& vertices, PrimitiveType type)> MutablePrimitiveCallback;

    ////////////////////////////////////////////////////////////
    /// \brief Iterate over each primitive and call the function
    ///
    /// This is a virtual function that has to be implemented by
    /// the derived class to define what primitive info is being
    /// provided.
    ///
    /// While the order of the primitives is not specified, it
    /// would be a good idea to iterate them in draw order.
    ///
    /// \param callback The function to call for each primitive
    ///
    ////////////////////////////////////////////////////////////
    virtual void forEachPrimitive(PrimitiveCallback callback) const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Iterate over each primitive and call the function
    ///
    /// This is a helper method that calls the virtual overload,
    /// and puts the results inside a mutable std::vector<Vertex>
    /// parameter.
    ///
    /// \param callback The function to call for each primitive
    ///
    ////////////////////////////////////////////////////////////
    void forEachMutablePrimitive(MutablePrimitiveCallback callback) const;
};

} // namespace sf


#endif // SFML_PRIMITIVEINFO_HPP


////////////////////////////////////////////////////////////
/// \class sf::PrimitiveInfo
/// \ingroup graphics
///
/// sf::PrimitiveInfo is a simple interface that allows users
/// to retrieve primitive information, for use with custom
/// rendering methods (such as sprite batching).
///
/// Note that most SFML drawables (with the exception of
/// sf::VertexBuffer) also implement sf::PrimitiveInfo.
///
/// Example:
/// \code
/// // Custom sprite batch class
/// #include "MyGraphicsBatch.hpp"
///
/// void MyGraphicsBatch::addToBatch(const PrimitiveInfo& batchable, const Texture* texture,
///                                  const Transform& transform, int order)
/// {
///     batchable.forEachPrimitive(
///         [&](const Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type)
///         {
///             // Batch each primitive, and draw it when
///             // MyGraphicsBatch::endBatch(...) is called
///         }
///     );
/// }
/// \endcode
///
/// \see sf::Drawable
///
////////////////////////////////////////////////////////////
