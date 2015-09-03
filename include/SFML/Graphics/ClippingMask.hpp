////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_CLIPPINGMASK_HPP
#define SFML_CLIPPINGMASK_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <algorithm>
#include <vector>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Define a part of the world that rendered objects are clipped to
///
/// The clipping mask is implemented using the stencil buffer. If the
/// stencil buffer was disabled during the context or target creation this class
/// will not function.
/// The mask can operate inclusively or exclusively - meaning you can either include
/// or exclude the area that will be visible through the mask.
/// The mask supports any type of drawable. Sprites that have textures will alpha
/// channels can even be used to construct pixel based masks.
/// It is also important to note that the mask only stores pointers to drawables
/// and not copies. This means if the passed in drawable is destructed the
/// behavior is undefined.
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API ClippingMask : public Transformable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Clipping modes
    ///
    ////////////////////////////////////////////////////////////
    enum Mode
    {
        Inclusive, ///< Inclusive mode, only areas defined by the mask will be visible
        Exclusive  ///< Exclusive mode, only areas defined by the mask will be hidden
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty clipping mask.
    ///
    ////////////////////////////////////////////////////////////
    ClippingMask();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a clipping mask with the specified mode
    ///
    /// \param theMode Clipping mode to use
    ///
    ////////////////////////////////////////////////////////////
    ClippingMask(Mode theMode);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a clipping mask with a custom transform
    ///
    /// \param theTransform Transform to use
    ///
    ////////////////////////////////////////////////////////////
    ClippingMask(const Transform& theTransform);

    ////////////////////////////////////////////////////////////
    /// \brief Return the drawable count
    ///
    /// \return Number of drawables in the array
    ///
    ////////////////////////////////////////////////////////////
    std::size_t getDrawableCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-write access to a drawable by its index
    ///
    /// This function doesn't check \a index, it must be in range
    /// [0, getDrawableCount() - 1]. The behavior is undefined
    /// otherwise.
    ///
    /// \param index Index of the drawable to get
    ///
    /// \return Reference to the index-th vertex
    ///
    /// \see getDrawableCount
    ///
    ////////////////////////////////////////////////////////////
    const Drawable*& operator [](std::size_t index);

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only access to a drawable by its index
    ///
    /// This function doesn't check \a index, it must be in range
    /// [0, getDrawableCount() - 1]. The behavior is undefined
    /// otherwise.
    ///
    /// \param index Index of the drawable to get
    ///
    /// \return Reference to the index-th vertex
    ///
    /// \see getDrawableCount
    ///
    ////////////////////////////////////////////////////////////
    const Drawable* const& operator [](std::size_t index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the drawable array
    ///
    /// This function removes all the drawables from the array.
    /// It doesn't deallocate the corresponding memory, so that
    /// adding new drawables after clearing doesn't involve
    /// reallocating all the memory.
    ///
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief Add a drawable to the array
    ///
    /// Only a pointer to the drawable will be stored, so the caller
    /// must ensure that the drawable does not get destructed.
    ///
    /// \param drawable Drawable to add
    ///
    ////////////////////////////////////////////////////////////
    void append(const Drawable& drawable);

    ////////////////////////////////////////////////////////////
    /// \brief Removes a drawable from the array
    ///
    /// \param drawable Drawable to remove
    ///
    ////////////////////////////////////////////////////////////
    void remove(const Drawable& drawable);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current clipping mode
    ///
    ////////////////////////////////////////////////////////////
    Mode getMode() const;

    ////////////////////////////////////////////////////////////
    /// \brief Sets the current clipping mode
    ///
    /// \param theMode Mode that the mask will use
    ///
    ////////////////////////////////////////////////////////////
    void setMode(Mode theMode);

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<const Drawable*> m_drawables; ///< Drawables that define the mask
    Mode                         m_mode;      ///< Mode that the mask will use
};

} // namespace sf

#endif // SFML_CLIPPINGMASK_HPP
