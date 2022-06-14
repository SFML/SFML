////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <filesystem>
#include <sstream>
#include <string>

#include <cctype>


namespace sf
{
[[nodiscard]] inline std::string toLower(std::string str)
{
    for (char& c : str)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return str;
}

[[nodiscard]] inline std::string formatDebugPathInfo(const std::filesystem::path& path)
{
    std::ostringstream stream;
    stream << "    Provided path: " << path << '\n';
    stream << "    Absolute path: " << std::filesystem::absolute(path);
    return stream.str();
}

// Convert byte sequence into integer
// toInteger<int>(0x12, 0x34, 0x56) == 0x563412
template <typename IntegerType, typename... Bytes>
[[nodiscard]] constexpr IntegerType toInteger(Bytes... byte)
{
    static_assert(sizeof(IntegerType) >= sizeof...(Bytes), "IntegerType not large enough to contain bytes");

    IntegerType integer = 0;
    std::size_t index   = 0;
    return ((integer |= static_cast<IntegerType>(static_cast<IntegerType>(byte) << 8 * index++)), ...);
}
} // namespace sf
