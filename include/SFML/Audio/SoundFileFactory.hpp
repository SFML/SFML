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
#include <SFML/Audio/Export.hpp>

#include <filesystem>
#include <memory>
#include <unordered_map>

#include <cstddef>


namespace sf
{
class InputStream;
class SoundFileReader;
class SoundFileWriter;

////////////////////////////////////////////////////////////
/// \brief Manages and instantiates sound file readers and writers
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API SoundFileFactory
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Register a new reader
    ///
    /// \see `unregisterReader`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    static void registerReader();

    ////////////////////////////////////////////////////////////
    /// \brief Unregister a reader
    ///
    /// \see `registerReader`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    static void unregisterReader();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a reader is registered
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] static bool isReaderRegistered();

    ////////////////////////////////////////////////////////////
    /// \brief Register a new writer
    ///
    /// \see `unregisterWriter`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    static void registerWriter();

    ////////////////////////////////////////////////////////////
    /// \brief Unregister a writer
    ///
    /// \see `registerWriter`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    static void unregisterWriter();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a writer is registered
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] static bool isWriterRegistered();

    ////////////////////////////////////////////////////////////
    /// \brief Instantiate the right reader for the given file on disk
    ///
    /// \param filename Path of the sound file
    ///
    /// \return A new sound file reader that can read the given file, or null if no reader can handle it
    ///
    /// \see `createReaderFromMemory`, `createReaderFromStream`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::unique_ptr<SoundFileReader> createReaderFromFilename(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Instantiate the right codec for the given file in memory
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Total size of the file data, in bytes
    ///
    /// \return A new sound file codec that can read the given file, or null if no codec can handle it
    ///
    /// \see `createReaderFromFilename`, `createReaderFromStream`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::unique_ptr<SoundFileReader> createReaderFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Instantiate the right codec for the given file in stream
    ///
    /// \param stream Source stream to read from
    ///
    /// \return A new sound file codec that can read the given file, or null if no codec can handle it
    ///
    /// \see `createReaderFromFilename`, `createReaderFromMemory`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::unique_ptr<SoundFileReader> createReaderFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Instantiate the right writer for the given file on disk
    ///
    /// \param filename Path of the sound file
    ///
    /// \return A new sound file writer that can write given file, or null if no writer can handle it
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::unique_ptr<SoundFileWriter> createWriterFromFilename(const std::filesystem::path& filename);

private:
    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    template <typename T>
    using CreateFnPtr = std::unique_ptr<T> (*)();

    using ReaderCheckFnPtr = bool (*)(InputStream&);
    using WriterCheckFnPtr = bool (*)(const std::filesystem::path&);

    using ReaderFactoryMap = std::unordered_map<CreateFnPtr<SoundFileReader>, ReaderCheckFnPtr>;
    using WriterFactoryMap = std::unordered_map<CreateFnPtr<SoundFileWriter>, WriterCheckFnPtr>;

    ////////////////////////////////////////////////////////////
    // Static member functions
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static ReaderFactoryMap& getReaderFactoryMap();
    [[nodiscard]] static WriterFactoryMap& getWriterFactoryMap();
};

} // namespace sf

#include <SFML/Audio/SoundFileFactory.inl>


////////////////////////////////////////////////////////////
/// \class sf::SoundFileFactory
/// \ingroup audio
///
/// This class is where all the sound file readers and writers are
/// registered. You should normally only need to use its registration
/// and unregistration functions; readers/writers creation and manipulation
/// are wrapped into the higher-level classes `sf::InputSoundFile` and
/// `sf::OutputSoundFile`.
///
/// To register a new reader (writer) use the `sf::SoundFileFactory::registerReader`
/// (`registerWriter`) static function. You don't have to call the `unregisterReader`
/// (`unregisterWriter`) function, unless you want to unregister a format before your
/// application ends (typically, when a plugin is unloaded).
///
/// Usage example:
/// \code
/// sf::SoundFileFactory::registerReader<MySoundFileReader>();
/// assert(sf::SoundFileFactory::isReaderRegistered<MySoundFileReader>());
///
/// sf::SoundFileFactory::registerWriter<MySoundFileWriter>();
/// assert(sf::SoundFileFactory::isWriterRegistered<MySoundFileWriter>());
/// \endcode
///
/// \see `sf::InputSoundFile`, `sf::OutputSoundFile`, `sf::SoundFileReader`, `sf::SoundFileWriter`
///
////////////////////////////////////////////////////////////
