////////////////////////////////////////////////////////////
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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

#include <gl.h>

#include <array>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>

#include <cmath>

namespace gl2
{
using Matrix = std::array<GLfloat, 16>;

inline constexpr char colorVertexShader[] = R"(
    attribute vec3 position;
    attribute vec4 color;

    uniform mat4 projection;
    uniform mat4 modelView;

    varying vec4 vertexColor;

    void main()
    {
        gl_Position = projection * modelView * vec4(position, 1.0);
        vertexColor = color;
    }
)";

inline constexpr char colorFragmentShader[] = R"(
    #ifdef GL_ES
    precision mediump float;
    #endif

    varying vec4 vertexColor;

    void main()
    {
        gl_FragColor = vertexColor;
    }
)";

inline constexpr char texturedVertexShader[] = R"(
    attribute vec3 position;
    attribute vec2 texCoord;

    uniform mat4 projection;
    uniform mat4 modelView;

    varying vec2 vertexTexCoord;

    void main()
    {
        gl_Position    = projection * modelView * vec4(position, 1.0);
        vertexTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    }
)";

inline constexpr char texturedFragmentShader[] = R"(
    #ifdef GL_ES
    precision mediump float;
    #endif

    uniform sampler2D texture0;
    varying vec2 vertexTexCoord;

    void main()
    {
        gl_FragColor = texture2D(texture0, vertexTexCoord);
    }
)";

class Program
{
public:
    Program() = default;

    Program(const Program&)            = delete;
    Program& operator=(const Program&) = delete;

    ~Program()
    {
        reset();
    }

    [[nodiscard]] bool load(const char*                                           vertexSource,
                            const char*                                           fragmentSource,
                            std::initializer_list<std::pair<GLuint, const char*>> attributes)
    {
        const GLuint vertexShader = compile(GL_VERTEX_SHADER, vertexSource);
        if (!vertexShader)
            return false;

        const GLuint fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource);
        if (!fragmentShader)
        {
            glDeleteShader(vertexShader);
            return false;
        }

        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);

        for (const auto& [location, name] : attributes)
            glBindAttribLocation(m_program, location, name);

        glLinkProgram(m_program);

        GLint linked = GL_FALSE;
        glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE)
        {
            printProgramLog(m_program);
            glDeleteProgram(m_program);
            m_program = 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return m_program != 0;
    }

    void use() const
    {
        glUseProgram(m_program);
    }

    void reset()
    {
        if (m_program)
        {
            glDeleteProgram(m_program);
            m_program = 0;
        }
    }

    [[nodiscard]] GLint uniform(const char* name) const
    {
        return glGetUniformLocation(m_program, name);
    }

    [[nodiscard]] GLuint nativeHandle() const
    {
        return m_program;
    }

private:
    [[nodiscard]] static GLuint compile(GLenum type, const char* source)
    {
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_TRUE)
            return shader;

        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> log(static_cast<std::size_t>(length > 1 ? length : 1));
        glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());
        std::cerr << "Failed to compile OpenGL shader:\n" << log.data() << std::endl;

        glDeleteShader(shader);
        return 0;
    }

    static void printProgramLog(GLuint program)
    {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> log(static_cast<std::size_t>(length > 1 ? length : 1));
        glGetProgramInfoLog(program, static_cast<GLsizei>(log.size()), nullptr, log.data());
        std::cerr << "Failed to link OpenGL program:\n" << log.data() << std::endl;
    }

    GLuint m_program{};
};

[[nodiscard]] inline constexpr Matrix identity()
{
    return {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
}

[[nodiscard]] inline Matrix multiply(const Matrix& left, const Matrix& right)
{
    Matrix result{};

    for (std::size_t column = 0; column < 4; ++column)
    {
        for (std::size_t row = 0; row < 4; ++row)
        {
            for (std::size_t item = 0; item < 4; ++item)
                result[column * 4 + row] += left[item * 4 + row] * right[column * 4 + item];
        }
    }

    return result;
}

[[nodiscard]] inline Matrix translation(float x, float y, float z)
{
    Matrix result = identity();
    result[12]    = x;
    result[13]    = y;
    result[14]    = z;
    return result;
}

[[nodiscard]] inline Matrix rotation(float degrees, float x, float y, float z)
{
    constexpr float pi      = 3.141592654f;
    const float     radians = degrees * pi / 180.f;
    const float     cosine  = std::cos(radians);
    const float     sine    = std::sin(radians);

    if (x != 0.f)
        return {1.f, 0.f, 0.f, 0.f, 0.f, cosine, sine, 0.f, 0.f, -sine, cosine, 0.f, 0.f, 0.f, 0.f, 1.f};

    if (y != 0.f)
        return {cosine, 0.f, -sine, 0.f, 0.f, 1.f, 0.f, 0.f, sine, 0.f, cosine, 0.f, 0.f, 0.f, 0.f, 1.f};

    return {cosine, sine, 0.f, 0.f, -sine, cosine, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
}

[[nodiscard]] inline Matrix frustum(float left, float right, float bottom, float top, float near, float far)
{
    return {2.f * near / (right - left),
            0.f,
            0.f,
            0.f,
            0.f,
            2.f * near / (top - bottom),
            0.f,
            0.f,
            (right + left) / (right - left),
            (top + bottom) / (top - bottom),
            -(far + near) / (far - near),
            -1.f,
            0.f,
            0.f,
            -(2.f * far * near) / (far - near),
            0.f};
}

[[nodiscard]] inline Matrix modelView(float x, float y, float z, float xRotation, float yRotation, float zRotation)
{
    return multiply(multiply(multiply(translation(x, y, z), rotation(xRotation, 1.f, 0.f, 0.f)),
                             rotation(yRotation, 0.f, 1.f, 0.f)),
                    rotation(zRotation, 0.f, 0.f, 1.f));
}

inline void setMatrix(GLint location, const Matrix& matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
}
} // namespace gl2
