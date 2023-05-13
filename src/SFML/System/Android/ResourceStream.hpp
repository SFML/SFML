////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/System/Export.hpp>

#include <SFML/System/InputStream.hpp>

#include <android/asset_manager.h>

#include <filesystem>
#include <string>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Read from Android asset files
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API ResourceStream : public InputStream
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param filename Filename of the asset
    ///
    ////////////////////////////////////////////////////////////
    ResourceStream(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Read data from the asset
    ///
    /// \param data Buffer where the asset data is copied
    /// \param size Number of bytes read
    ///
    /// \return The number of bytes actually read, or -1 on error
    ///
    ////////////////////////////////////////////////////////////
    std::int64_t read(void* data, std::int64_t size) override;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current reading position in the asset file
    ///
    /// \param position The position to seek to, from the beginning
    ///
    /// \return The position actually sought to, or -1 on error
    ///
    ////////////////////////////////////////////////////////////
    std::int64_t seek(std::int64_t position) override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current reading position in the asset file
    ///
    /// \return The current position, or -1 on error.
    ///
    ////////////////////////////////////////////////////////////
    std::int64_t tell() override;

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the asset file
    ///
    /// \return The total number of bytes available in the asset, or -1 on error
    ///
    ////////////////////////////////////////////////////////////
    std::int64_t getSize() override;

private:
    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    struct AAssetDeleter
    {
        void operator()(AAsset*);
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::unique_ptr<AAsset, AAssetDeleter> m_file; ///< The asset file to read
};

} // namespace sf::priv
