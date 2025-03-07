////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#include <SFML/System/InputStream.hpp>

#include <cstddef>
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Implementation of input stream based on a memory chunk
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API MemoryInputStream : public InputStream
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the stream from its data
    ///
    /// \param data        Pointer to the data in memory
    /// \param sizeInBytes Size of the data, in bytes
    ///
    ////////////////////////////////////////////////////////////
    MemoryInputStream(const void* data, std::size_t sizeInBytes);

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
    [[nodiscard]] std::optional<std::size_t> read(void* data, std::size_t size) override;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current reading position
    ///
    /// \param position The position to seek to, from the beginning
    ///
    /// \return The position actually sought to, or `std::nullopt` on error
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<std::size_t> seek(std::size_t position) override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current reading position in the stream
    ///
    /// \return The current position, or `std::nullopt` on error.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<std::size_t> tell() override;

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the stream
    ///
    /// \return The total number of bytes available in the stream, or `std::nullopt` on error
    ///
    ////////////////////////////////////////////////////////////
    std::optional<std::size_t> getSize() override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const std::byte* m_data{};   //!< Pointer to the data in memory
    std::size_t      m_size{};   //!< Total size of the data
    std::size_t      m_offset{}; //!< Current reading position
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::MemoryInputStream
/// \ingroup system
///
/// This class is a specialization of `InputStream` that
/// reads from data in memory.
///
/// It wraps a memory chunk in the common `InputStream` interface
/// and therefore allows to use generic classes or functions
/// that accept such a stream, with content already loaded in memory.
///
/// In addition to the virtual functions inherited from
/// `InputStream`, `MemoryInputStream` adds a function to
/// specify the pointer and size of the data in memory.
///
/// SFML resource classes can usually be loaded directly from
/// memory, so this class shouldn't be useful to you unless
/// you create your own algorithms that operate on an InputStream.
///
/// Usage example:
/// \code
/// void process(InputStream& stream);
///
/// MemoryInputStream stream(thePtr, theSize);
/// process(stream);
/// \endcode
///
/// \see `InputStream`, `FileInputStream`
///
////////////////////////////////////////////////////////////
