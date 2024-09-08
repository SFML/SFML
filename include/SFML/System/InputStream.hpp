////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Config.hpp>

#include <SFML/System/Export.hpp>

#include <optional>

#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Abstract class for custom file input streams
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API InputStream
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~InputStream() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Read data from the stream
    ///
    /// After reading, the stream's reading position must be
    /// advanced by the amount of bytes read.
    ///
    /// \param data Buffer where to copy the read data
    /// \param size Desired number of bytes to read
    ///
    /// \return The number of bytes actually read, or `std::nullopt` on error
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual std::optional<std::size_t> read(void* data, std::size_t size) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current reading position
    ///
    /// \param position The position to seek to, from the beginning
    ///
    /// \return The position actually sought to, or `std::nullopt` on error
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual std::optional<std::size_t> seek(std::size_t position) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current reading position in the stream
    ///
    /// \return The current position, or `std::nullopt` on error.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual std::optional<std::size_t> tell() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the stream
    ///
    /// \return The total number of bytes available in the stream, or `std::nullopt` on error
    ///
    ////////////////////////////////////////////////////////////
    virtual std::optional<std::size_t> getSize() = 0;
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::InputStream
/// \ingroup system
///
/// This class allows users to define their own file input sources
/// from which SFML can load resources.
///
/// SFML resource classes like `sf::Texture` and
/// `sf::SoundBuffer` provide `loadFromFile` and `loadFromMemory` functions,
/// which read data from conventional sources. However, if you
/// have data coming from a different source (over a network,
/// embedded, encrypted, compressed, etc) you can derive your
/// own class from `sf::InputStream` and load SFML resources with
/// their `loadFromStream` function.
///
/// Usage example:
/// \code
/// // custom stream class that reads from inside a zip file
/// class ZipStream : public sf::InputStream
/// {
/// public:
///
///     ZipStream(const std::string& archive);
///
///     [[nodiscard]] bool open(const std::filesystem::path& filename);
///
///     [[nodiscard]] std::optional<std::size_t> read(void* data, std::size_t size);
///
///     [[nodiscard]] std::optional<std::size_t> seek(std::size_t position);
///
///     [[nodiscard]] std::optional<std::size_t> tell();
///
///     std::optional<std::size_t> getSize();
///
/// private:
///
///     ...
/// };
///
/// // now you can load textures...
/// ZipStream stream("resources.zip");
///
/// if (!stream.open("images/img.png"))
/// {
///     // Handle error...
/// }
///
/// const sf::Texture texture(stream);
///
/// // musics...
/// sf::Music music;
/// ZipStream stream("resources.zip");
///
/// if (!stream.open("musics/msc.ogg"))
/// {
///     // Handle error...
/// }
///
/// if (!music.openFromStream(stream))
/// {
///     // Handle error...
/// }
///
/// // etc.
/// \endcode
///
/// \see `FileInputStream`, `MemoryInputStream`
///
////////////////////////////////////////////////////////////
