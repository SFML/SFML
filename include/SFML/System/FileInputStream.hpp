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

#include <filesystem>
#include <memory>

#include <cstdint>
#include <cstdio>

#ifdef SFML_SYSTEM_ANDROID
namespace sf::priv
{
class SFML_SYSTEM_API ResourceStream;
}
#endif


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Implementation of input stream based on a file
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API FileInputStream : public InputStream
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Construct a file input stream that is not associated
    /// with a file to read.
    ///
    ////////////////////////////////////////////////////////////
    FileInputStream();

    ////////////////////////////////////////////////////////////
    /// \brief Default destructor
    ///
    ////////////////////////////////////////////////////////////
    ~FileInputStream() override;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    FileInputStream(const FileInputStream&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    FileInputStream& operator=(const FileInputStream&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    FileInputStream(FileInputStream&&) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    FileInputStream& operator=(FileInputStream&&) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the stream from a file path
    ///
    /// \param filename Name of the file to open
    ///
    /// \throws sf::Exception on error
    ///
    ////////////////////////////////////////////////////////////
    explicit FileInputStream(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open the stream from a file path
    ///
    /// \param filename Name of the file to open
    ///
    /// \return `true` on success, `false` on error
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(const std::filesystem::path& filename);

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
    /// \brief Deleter for stdio file stream that closes the file stream
    ///
    ////////////////////////////////////////////////////////////
    struct FileCloser
    {
        void operator()(std::FILE* file);
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
#ifdef SFML_SYSTEM_ANDROID
    std::unique_ptr<priv::ResourceStream> m_androidFile;
#endif

    std::unique_ptr<std::FILE, FileCloser> m_file; //!< stdio file stream
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::FileInputStream
/// \ingroup system
///
/// This class is a specialization of `InputStream` that
/// reads from a file on disk.
///
/// It wraps a file in the common `InputStream` interface
/// and therefore allows to use generic classes or functions
/// that accept such a stream, with a file on disk as the data
/// source.
///
/// In addition to the virtual functions inherited from
/// `InputStream`, `FileInputStream` adds a function to
/// specify the file to open.
///
/// SFML resource classes can usually be loaded directly from
/// a filename, so this class shouldn't be useful to you unless
/// you create your own algorithms that operate on an InputStream.
///
/// Usage example:
/// \code
/// void process(InputStream& stream);
///
/// std::optional stream = sf::FileInputStream::open("some_file.dat");
/// if (stream)
///    process(*stream);
/// \endcode
///
/// \see `InputStream`, `MemoryInputStream`
///
////////////////////////////////////////////////////////////
