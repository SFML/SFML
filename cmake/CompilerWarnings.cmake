# from here:
#
# https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Available.md

# Helper function to enable compiler warnings for a specific target
function(set_target_warnings target)
    option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" TRUE)

    if(SFML_COMPILER_MSVC)
        target_compile_options(${target} PRIVATE
            $<$<BOOL:${WARNINGS_AS_ERRORS}>:/WX>
            /W4 # Baseline reasonable warnings
            /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
            /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
            /w14263 # 'function': member function does not override any base class virtual member function
            /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
            /w14287 # 'operator': unsigned/negative constant mismatch
            /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
            /w14296 # 'operator': expression is always 'boolean_value'
            /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
            /w14545 # expression before comma evaluates to a function which is missing an argument list
            /w14546 # function call before comma missing argument list
            /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
            /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
            /w14555 # expression has no effect; expected expression with side- effect
            /w14619 # pragma warning: there is no warning number 'number'
            /w14640 # Enable warning on thread un-safe static member initialization
            /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
            /w14905 # wide string literal cast to 'LPSTR'
            /w14906 # string literal cast to 'LPWSTR'
            /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
            /permissive- # standards conformance mode

            # Disables, remove when appropriate
            /wd4068 # disable warnings about unknown pragmas (e.g. #pragma GCC)
            /wd4505 # disable warnings about unused functions that might be platform-specific
            /wd4800 # disable warnings regarding implicit conversions to bool
        )
    endif()

    if(SFML_COMPILER_GCC OR SFML_COMPILER_CLANG)
        target_compile_options(${target} PRIVATE
            $<$<BOOL:${WARNINGS_AS_ERRORS}>:-Werror>
            -Wall
            -Wextra # reasonable and standard
            -Wshadow # warn the user if a variable declaration shadows one from a parent context
            -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
            -Wcast-align # warn for potential performance problem casts
            -Wunused # warn on anything being unused
            -Woverloaded-virtual # warn if you overload (not override) a virtual function
            -Wconversion # warn on type conversions that may lose data
            -Wsign-conversion # warn on sign conversions
            -Wdouble-promotion # warn if float is implicit promoted to double
            -Wformat=2 # warn on security issues around functions that format output (ie printf)
            -Wimplicit-fallthrough # warn when a missing break causes control flow to continue at the next case in a switch statement
            -Wsuggest-override # warn when 'override' could be used on a member function overriding a virtual function
            -Wnull-dereference # warn if a null dereference is detected
            -Wold-style-cast # warn for c-style casts
            -Wpedantic # warn if non-standard C++ is used
            $<$<BOOL:${SFML_OS_ANDROID}>:-Wno-main> # allow main() to be called
        )
    endif()

    if(SFML_COMPILER_GCC)
        # Don't enable -Wduplicated-branches for GCC < 8.1 since it will lead to false positives
        # https://github.com/gcc-mirror/gcc/commit/6bebae75035889a4844eb4d32a695bebf412bcd7
        target_compile_options(${target} PRIVATE
            -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
            -Wduplicated-cond # warn if if / else chain has duplicated conditions
            -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
            # -Wuseless-cast # warn if you perform a cast to the same type (disabled because it is not portable as some type aliases might vary between platforms)
            $<$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},8.1>:-Wduplicated-branches> # warn if if / else branches have duplicated code
        )
    endif()

    if(SFML_COMPILER_CLANG OR SFML_COMPILER_CLANG_CL)
        target_compile_options(${target} PRIVATE
            -Wno-unknown-warning-option # do not warn on GCC-specific warning diagnostic pragmas
        )
    endif()

    # Disable certain deprecation warnings
    if(SFML_OS_WINDOWS)
        target_compile_definitions(${target} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
        target_compile_definitions(${target} PRIVATE -D_WINSOCK_DEPRECATED_NO_WARNINGS)
    endif()
endfunction()
