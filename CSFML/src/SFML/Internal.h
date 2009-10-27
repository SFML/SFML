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

#ifndef SFML_INTERNAL_H
#define SFML_INTERNAL_H

////////////////////////////////////////////////////////////
// Define macros to check the validity of CSFML objects
// in debug run
////////////////////////////////////////////////////////////
#include <stdio.h>

#ifndef NDEBUG

    #define CSFML_CHECK(Object) \
                if (Object == NULL) \
                { \
                    fprintf(stderr, "SFML warning : trying to use a null " #Object " object\n"); \
                    return; \
                }

    #define CSFML_CALL(Object, Function) \
                if (Object) \
                { \
                    (Object->This.Function); \
                } \
                else \
                { \
                    fprintf(stderr, "SFML warning : trying to use a null " #Object " object\n"); \
                } \

    #define CSFML_CALL_PTR(Object, Function) \
                if (Object) \
                { \
                    (Object->This->Function); \
                } \
                else \
                { \
                    fprintf(stderr, "SFML warning : trying to use a null " #Object " object\n"); \
                } \

    #define CSFML_CHECK_RETURN(Object, Default) \
                if (Object == NULL) \
                { \
                    fprintf(stderr, "SFML warning : trying to use a null " #Object " object\n"); \
                    return Default; \
                }

    #define CSFML_CALL_RETURN(Object, Function, Default) \
                if (Object) \
                { \
                    return (Object->This.Function); \
                } \
                else \
                { \
                    fprintf(stderr, "SFML warning : trying to use a null " #Object " object\n"); \
                    return Default; \
                } \

    #define CSFML_CALL_PTR_RETURN(Object, Function, Default) \
                if (Object) \
                { \
                    return (Object->This->Function); \
                } \
                else \
                { \
                    fprintf(stderr, "SFML warning : trying to use a null " #Object " object\n"); \
                    return Default; \
                } \

#else

    #define CSFML_CHECK(Object)

    #define CSFML_CALL(Object, Function) (Object->This.Function);

    #define CSFML_CALL_PTR(Object, Function) (Object->This->Function);

    #define CSFML_CHECK_RETURN(Object, Default) (void)Default;

    #define CSFML_CALL_RETURN(Object, Function, Default) (void)Default; return (Object->This.Function);

    #define CSFML_CALL_PTR_RETURN(Object, Function, Default) (void)Default; return (Object->This->Function);

#endif

#endif // SFML_INTERNAL_H
