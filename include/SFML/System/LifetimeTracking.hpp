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

#ifdef SFML_ENABLE_LIFETIME_TRACKING

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <SFML/System/Export.hpp>

#include <atomic>


namespace sf::priv
{

class SFML_SYSTEM_API LifetimeDependee
{
public:
    struct SFML_SYSTEM_API TestingModeGuard
    {
        TestingModeGuard();
        ~TestingModeGuard();

        TestingModeGuard(const TestingModeGuard&) = delete;
        TestingModeGuard(TestingModeGuard&&)      = delete;

        TestingModeGuard& operator=(const TestingModeGuard&) = delete;
        TestingModeGuard& operator=(TestingModeGuard&&)      = delete;

        [[nodiscard]] static bool fatalErrorTriggered();
    };

    explicit LifetimeDependee(const char* dependeeName, const char* dependantName);
    ~LifetimeDependee();

    LifetimeDependee(const LifetimeDependee& rhs);
    LifetimeDependee(LifetimeDependee&& rhs) noexcept;

    LifetimeDependee& operator=(const LifetimeDependee& rhs);
    LifetimeDependee& operator=(LifetimeDependee&& rhs) noexcept;

    void addDependant();
    void subDependant();

private:
    // NOLINTBEGIN(readability-identifier-naming)
    static inline std::atomic<bool> s_testingMode{false};
    static inline std::atomic<bool> s_fatalErrorTriggered{false};
    // NOLINTEND(readability-identifier-naming)

    const char*               m_dependeeName;   ///< Readable dependee type name
    const char*               m_dependantName;  ///< Readable dependent type name
    std::atomic<unsigned int> m_dependantCount; ///< Current alive dependant count
};

class SFML_SYSTEM_API LifetimeDependant
{
public:
    explicit LifetimeDependant(LifetimeDependee* dependee = nullptr) noexcept;
    ~LifetimeDependant();

    LifetimeDependant(const LifetimeDependant& rhs) noexcept;
    LifetimeDependant(LifetimeDependant&& rhs) noexcept;

    LifetimeDependant& operator=(const LifetimeDependant& rhs) noexcept;
    LifetimeDependant& operator=(LifetimeDependant&& rhs) noexcept;

    void update(LifetimeDependee* dependee) noexcept;

private:
    void addSelfAsDependant();
    void subSelfAsDependant();

    LifetimeDependee* m_dependee;
};

} // namespace sf::priv

// NOLINTBEGIN(bugprone-macro-parentheses)
#define SFML_DEFINE_LIFETIME_DEPENDEE(dependeeType, dependantType)                                               \
    friend dependantType;                                                                                        \
    mutable ::sf::priv::LifetimeDependee m_sfPrivLifetimeDependee##dependantType{#dependeeType, #dependantType}; \
    using sfPrivSwallowSemicolon##dependantType##dependeeType = void

#define SFML_DEFINE_LIFETIME_DEPENDANT(dependantType)                               \
    mutable ::sf::priv::LifetimeDependant m_sfPrivLifetimeDependant##dependantType; \
    using sfPrivSwallowSemicolon##dependantType = void

#define SFML_UPDATE_LIFETIME_DEPENDANT(dependantType, dependeeType, dependantMemberPtr) \
    m_sfPrivLifetimeDependant##dependantType.update(                                    \
        dependantMemberPtr == nullptr ? nullptr : &dependantMemberPtr->m_sfPrivLifetimeDependee##dependeeType)
// NOLINTEND(bugprone-macro-parentheses)

#else // SFML_ENABLE_LIFETIME_TRACKING

#define SFML_DEFINE_LIFETIME_DEPENDEE(dependantType, dependeeType) \
    using sfPrivSwallowSemicolon##dependantType##dependeeType = void

#define SFML_DEFINE_LIFETIME_DEPENDANT(dependantType) using sfPrivSwallowSemicolon##dependantType = void

#define SFML_UPDATE_LIFETIME_DEPENDANT(...) (void)0

#endif // SFML_ENABLE_LIFETIME_TRACKING
