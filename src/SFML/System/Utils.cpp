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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Utils.hpp>

#include <sstream>

#include <cctype>


namespace sf
{
std::string toLower(std::string str)
{
    for (char& c : str)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return str;
}

std::string formatDebugPathInfo(const std::filesystem::path& path)
{
    std::ostringstream oss;
    // convert to UTF-8 to handle non-ascii/non-latin1 filenames on windows
    // cast is required to work in C++20 where u8string is char8_t which can't be printed to char stream
    oss << "    Provided path: " << reinterpret_cast<const char*>(path.u8string().c_str()) << '\n' //
        << "    Absolute path: " << reinterpret_cast<const char*>(std::filesystem::absolute(path).u8string().c_str());
    return oss.str();
}

std::FILE* openFile(const std::filesystem::path& filename, std::string_view mode)
{
#ifdef SFML_SYSTEM_WINDOWS
    const std::wstring wmode(mode.begin(), mode.end());
    return _wfopen(filename.c_str(), wmode.data());
#else
    return std::fopen(filename.c_str(), mode.data());
#endif
}

} // namespace sf
