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

#ifndef SFML_BATCH_HPP
#define SFML_BATCH_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>


namespace sf
{
class Image;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Batch of geometry / render states to render
///
////////////////////////////////////////////////////////////
class Batch
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the batch with its render states
    ///
    /// \param texture Texture to use
    /// \param blendMode Blending mode
    /// \param viewport Target viewport
    ///
    ////////////////////////////////////////////////////////////
    Batch(const Image* texture = NULL, Blend::Mode blendMode = Blend::Alpha, const IntRect& viewport = IntRect());

    ////////////////////////////////////////////////////////////
    /// \brief Check if the batch matches a set of render states
    ///
    /// \param texture Texture to use
    /// \param blendMode Blending mode
    /// \param viewport Target viewport
    ///
    ////////////////////////////////////////////////////////////
    bool Matches(const Image* texture, Blend::Mode blendMode, const IntRect& viewport) const;

    ////////////////////////////////////////////////////////////
    /// \brief Setup the start index of the batch
    ///
    /// \param index Start index
    ///
    ////////////////////////////////////////////////////////////
    void Begin(std::size_t index);

    ////////////////////////////////////////////////////////////
    /// \brief Setup the end index of the batch
    ///
    /// \param index End index
    ///
    ////////////////////////////////////////////////////////////
    void End(std::size_t index);

    ////////////////////////////////////////////////////////////
    /// \brief Return the start index of the batch
    ///
    /// \return Start index
    ///
    ////////////////////////////////////////////////////////////
    std::size_t GetStartIndex() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the number of indices to render with this batch
    ///
    /// \return Index count
    ///
    ////////////////////////////////////////////////////////////
    std::size_t GetIndexCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Apply the render states of the batch
    ///
    ////////////////////////////////////////////////////////////
    void ApplyStates() const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const Image* myTexture;   ///< Texture used by the batch
    Blend::Mode  myBlendMode; ///< Blending mode used by the batch
    IntRect      myViewport;  ///< Target viewport for the batch
    std::size_t  myStart;     ///< Index of the first index to render with this batch
    std::size_t  myCount;     ///< Number of indices to render with this batch
};

} // namespace priv

} // namespace sf


#endif // SFML_BATCH_HPP
