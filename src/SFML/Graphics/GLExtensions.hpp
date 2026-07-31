////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <glad/gl.h>

#ifdef SFML_OPENGL_ES

// OpenGL ES 2.0 is SFML's required baseline. All rendering entry points
// below are core in ES 2.0 unless explicitly marked optional.
#define GLEXT_multitexture    SF_GLAD_GL_ES_VERSION_2_0
#define GLEXT_glActiveTexture glActiveTexture
#define GLEXT_GL_TEXTURE0     GL_TEXTURE0

#define GLEXT_multitexture_dependencies SF_GLAD_GL_ES_VERSION_2_0, glActiveTexture

#define GLEXT_texture_edge_clamp true
#define GLEXT_GL_CLAMP           GL_CLAMP_TO_EDGE
#define GLEXT_GL_CLAMP_TO_EDGE   GL_CLAMP_TO_EDGE

#define GLEXT_vertex_buffer_object SF_GLAD_GL_ES_VERSION_2_0
#define GLEXT_glBindBuffer         glBindBuffer
#define GLEXT_glBufferData         glBufferData
#define GLEXT_glBufferSubData      glBufferSubData
#define GLEXT_glDeleteBuffers      glDeleteBuffers
#define GLEXT_glGenBuffers         glGenBuffers
#define GLEXT_GL_ARRAY_BUFFER      GL_ARRAY_BUFFER
#define GLEXT_GL_DYNAMIC_DRAW      GL_DYNAMIC_DRAW
#define GLEXT_GL_STATIC_DRAW       GL_STATIC_DRAW
#define GLEXT_GL_STREAM_DRAW       GL_STREAM_DRAW

#define GLEXT_vertex_buffer_object_dependencies \
    SF_GLAD_GL_ES_VERSION_2_0, glBindBuffer, glBufferData, glBufferSubData, glDeleteBuffers, glGenBuffers

#define GLEXT_blend_subtract           SF_GLAD_GL_ES_VERSION_2_0
#define GLEXT_glBlendEquation          glBlendEquation
#define GLEXT_GL_FUNC_ADD              GL_FUNC_ADD
#define GLEXT_GL_FUNC_SUBTRACT         GL_FUNC_SUBTRACT
#define GLEXT_GL_FUNC_REVERSE_SUBTRACT GL_FUNC_REVERSE_SUBTRACT

#define GLEXT_blend_subtract_dependencies SF_GLAD_GL_ES_VERSION_2_0, glBlendEquation

#define GLEXT_blend_func_separate SF_GLAD_GL_ES_VERSION_2_0
#define GLEXT_glBlendFuncSeparate glBlendFuncSeparate

#define GLEXT_blend_func_separate_dependencies SF_GLAD_GL_ES_VERSION_2_0, glBlendFuncSeparate

#define GLEXT_blend_equation_separate SF_GLAD_GL_ES_VERSION_2_0
#define GLEXT_glBlendEquationSeparate glBlendEquationSeparate

#define GLEXT_blend_equation_separate_dependencies SF_GLAD_GL_ES_VERSION_2_0, glBlendEquationSeparate

#define GLEXT_texture_non_power_of_two (SF_GLAD_GL_ES_VERSION_3_0 || SF_GLAD_GL_OES_texture_npot)

#define GLEXT_framebuffer_object               SF_GLAD_GL_ES_VERSION_2_0
#define GLEXT_glBindRenderbuffer               glBindRenderbuffer
#define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffers
#define GLEXT_glGenRenderbuffers               glGenRenderbuffers
#define GLEXT_glRenderbufferStorage            glRenderbufferStorage
#define GLEXT_glBindFramebuffer                glBindFramebuffer
#define GLEXT_glDeleteFramebuffers             glDeleteFramebuffers
#define GLEXT_glGenFramebuffers                glGenFramebuffers
#define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatus
#define GLEXT_glFramebufferTexture2D           glFramebufferTexture2D
#define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbuffer
#define GLEXT_glGenerateMipmap                 glGenerateMipmap
#define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER
#define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER
#define GLEXT_GL_DEPTH_COMPONENT               GL_DEPTH_COMPONENT16
#define GLEXT_GL_STENCIL_INDEX8                GL_STENCIL_INDEX8
#define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0
#define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT
#define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE
#define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING
#define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION
#define GLEXT_GL_STENCIL_ATTACHMENT            GL_STENCIL_ATTACHMENT

#define GLEXT_framebuffer_object_dependencies                                                             \
    SF_GLAD_GL_ES_VERSION_2_0, glBindRenderbuffer, glDeleteRenderbuffers, glGenRenderbuffers,              \
        glRenderbufferStorage, glBindFramebuffer, glDeleteFramebuffers, glGenFramebuffers,                 \
        glCheckFramebufferStatus, glFramebufferTexture2D, glFramebufferRenderbuffer, glGenerateMipmap

#define GLEXT_packed_depth_stencil (SF_GLAD_GL_ES_VERSION_3_0 || SF_GLAD_GL_OES_packed_depth_stencil)
#define GLEXT_GL_DEPTH24_STENCIL8  GL_DEPTH24_STENCIL8

#define GLEXT_framebuffer_blit            SF_GLAD_GL_ES_VERSION_3_0
#define GLEXT_glBlitFramebuffer           glBlitFramebuffer
#define GLEXT_GL_READ_FRAMEBUFFER         GL_READ_FRAMEBUFFER
#define GLEXT_GL_DRAW_FRAMEBUFFER         GL_DRAW_FRAMEBUFFER
#define GLEXT_GL_DRAW_FRAMEBUFFER_BINDING GL_DRAW_FRAMEBUFFER_BINDING
#define GLEXT_GL_READ_FRAMEBUFFER_BINDING GL_READ_FRAMEBUFFER_BINDING

#define GLEXT_framebuffer_multisample          SF_GLAD_GL_ES_VERSION_3_0
#define GLEXT_glRenderbufferStorageMultisample glRenderbufferStorageMultisample
#define GLEXT_GL_MAX_SAMPLES                   GL_MAX_SAMPLES

#define GLEXT_copy_buffer          SF_GLAD_GL_ES_VERSION_3_0
#define GLEXT_GL_COPY_READ_BUFFER  GL_COPY_READ_BUFFER
#define GLEXT_GL_COPY_WRITE_BUFFER GL_COPY_WRITE_BUFFER
#define GLEXT_glCopyBufferSubData  glCopyBufferSubData

#define GLEXT_texture_sRGB    (SF_GLAD_GL_ES_VERSION_3_0 || SF_GLAD_GL_EXT_sRGB)
#define GLEXT_GL_SRGB8_ALPHA8 GL_SRGB8_ALPHA8

#define GLEXT_framebuffer_sRGB false

#define GLEXT_blend_minmax    SF_GLAD_GL_EXT_blend_minmax
#define GLEXT_GL_MIN          GL_MIN_EXT
#define GLEXT_GL_MAX          GL_MAX_EXT

#define GLEXT_blend_minmax_dependencies SF_GLAD_GL_EXT_blend_minmax, glBlendEquation

#else

// OpenGL 2.0 is SFML's required desktop baseline.
#define GLEXT_GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT
#define GLEXT_GL_CLAMP           GL_CLAMP

// The following extensions are listed chronologically
// Extension macro first, followed by tokens then
// functions according to the corresponding specification

// The following extensions are optional.

// Core in the required OpenGL 2.0 baseline
#define GLEXT_texture_edge_clamp true
#define GLEXT_GL_CLAMP_TO_EDGE   GL_CLAMP_TO_EDGE

// Core in the required OpenGL 2.0 baseline
#define GLEXT_blend_minmax       SF_GLAD_GL_VERSION_2_0
#define GLEXT_glBlendEquation    glBlendEquation
#define GLEXT_GL_FUNC_ADD        GL_FUNC_ADD
#define GLEXT_GL_MIN             GL_MIN
#define GLEXT_GL_MAX             GL_MAX

#define GLEXT_blend_minmax_dependencies SF_GLAD_GL_VERSION_2_0, glBlendEquation

// Core since 1.2 - EXT_blend_subtract
#define GLEXT_blend_subtract           SF_GLAD_GL_VERSION_2_0
#define GLEXT_GL_FUNC_SUBTRACT         GL_FUNC_SUBTRACT
#define GLEXT_GL_FUNC_REVERSE_SUBTRACT GL_FUNC_REVERSE_SUBTRACT

// Core since 1.3 - ARB_multitexture
#define GLEXT_multitexture    SF_GLAD_GL_VERSION_2_0
#define GLEXT_glActiveTexture glActiveTexture
#define GLEXT_GL_TEXTURE0     GL_TEXTURE0

#define GLEXT_multitexture_dependencies SF_GLAD_GL_VERSION_2_0, glActiveTexture

// Core since 1.4 - EXT_blend_func_separate
#define GLEXT_blend_func_separate SF_GLAD_GL_VERSION_2_0
#define GLEXT_glBlendFuncSeparate glBlendFuncSeparate

#define GLEXT_blend_func_separate_dependencies SF_GLAD_GL_VERSION_2_0, glBlendFuncSeparate

// Core since 1.5 - ARB_vertex_buffer_object
#define GLEXT_vertex_buffer_object SF_GLAD_GL_VERSION_2_0
#define GLEXT_GL_ARRAY_BUFFER      GL_ARRAY_BUFFER
#define GLEXT_GL_DYNAMIC_DRAW      GL_DYNAMIC_DRAW
#define GLEXT_GL_READ_ONLY         GL_READ_ONLY
#define GLEXT_GL_STATIC_DRAW       GL_STATIC_DRAW
#define GLEXT_GL_STREAM_DRAW       GL_STREAM_DRAW
#define GLEXT_GL_WRITE_ONLY        GL_WRITE_ONLY
#define GLEXT_glBindBuffer         glBindBuffer
#define GLEXT_glBufferData         glBufferData
#define GLEXT_glBufferSubData      glBufferSubData
#define GLEXT_glDeleteBuffers      glDeleteBuffers
#define GLEXT_glGenBuffers         glGenBuffers
#define GLEXT_glMapBuffer          glMapBuffer
#define GLEXT_glUnmapBuffer        glUnmapBuffer

#define GLEXT_vertex_buffer_object_dependencies                                                          \
    SF_GLAD_GL_VERSION_2_0, glBindBuffer, glBufferData, glBufferSubData, glDeleteBuffers, glGenBuffers,  \
        glMapBuffer, glUnmapBuffer

#define GLEXT_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS

// Core since 2.0 - ARB_texture_non_power_of_two
#define GLEXT_texture_non_power_of_two SF_GLAD_GL_VERSION_2_0

// Core since 2.0 - EXT_blend_equation_separate
#define GLEXT_blend_equation_separate SF_GLAD_GL_VERSION_2_0
#define GLEXT_glBlendEquationSeparate glBlendEquationSeparate

#define GLEXT_blend_equation_separate_dependencies SF_GLAD_GL_VERSION_2_0, glBlendEquationSeparate

// Core since 2.1 - EXT_texture_sRGB
#define GLEXT_texture_sRGB    (SF_GLAD_GL_VERSION_2_1 || SF_GLAD_GL_EXT_texture_sRGB)
#define GLEXT_GL_SRGB8_ALPHA8 GL_SRGB8_ALPHA8

// Core since 3.0 - ARB_framebuffer_sRGB
#define GLEXT_framebuffer_sRGB (SF_GLAD_GL_VERSION_3_0 || SF_GLAD_GL_ARB_framebuffer_sRGB)

// Core since 3.0 - EXT_framebuffer_object
#define GLEXT_framebuffer_object \
    (SF_GLAD_GL_VERSION_3_0 || SF_GLAD_GL_ARB_framebuffer_object || SF_GLAD_GL_EXT_framebuffer_object)
#define GLEXT_glBindRenderbuffer               glBindRenderbuffer
#define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffers
#define GLEXT_glGenRenderbuffers               glGenRenderbuffers
#define GLEXT_glRenderbufferStorage            glRenderbufferStorage
#define GLEXT_glBindFramebuffer                glBindFramebuffer
#define GLEXT_glDeleteFramebuffers             glDeleteFramebuffers
#define GLEXT_glGenFramebuffers                glGenFramebuffers
#define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatus
#define GLEXT_glFramebufferTexture2D           glFramebufferTexture2D
#define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbuffer
#define GLEXT_glGenerateMipmap                 glGenerateMipmap
#define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER
#define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER
#define GLEXT_GL_STENCIL_INDEX8                GL_STENCIL_INDEX8
#define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0
#define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT
#define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE
#define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING
#define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION
#define GLEXT_GL_STENCIL_ATTACHMENT            GL_STENCIL_ATTACHMENT

#define GLEXT_framebuffer_object_dependencies                                                         \
    GLEXT_framebuffer_object, glBindRenderbuffer, glDeleteRenderbuffers, glGenRenderbuffers,           \
        glRenderbufferStorage, glBindFramebuffer, glDeleteFramebuffers, glGenFramebuffers,             \
        glCheckFramebufferStatus, glFramebufferTexture2D, glFramebufferRenderbuffer, glGenerateMipmap

// Core since 3.0 - EXT_packed_depth_stencil
#define GLEXT_packed_depth_stencil (SF_GLAD_GL_VERSION_3_0 || SF_GLAD_GL_EXT_packed_depth_stencil)
#define GLEXT_GL_DEPTH24_STENCIL8  GL_DEPTH24_STENCIL8

// Core since 3.0 - EXT_framebuffer_blit
#define GLEXT_framebuffer_blit \
    (SF_GLAD_GL_VERSION_3_0 || SF_GLAD_GL_ARB_framebuffer_object || SF_GLAD_GL_EXT_framebuffer_blit)
#define GLEXT_glBlitFramebuffer           glBlitFramebuffer
#define GLEXT_GL_READ_FRAMEBUFFER         GL_READ_FRAMEBUFFER
#define GLEXT_GL_DRAW_FRAMEBUFFER         GL_DRAW_FRAMEBUFFER
#define GLEXT_GL_DRAW_FRAMEBUFFER_BINDING GL_DRAW_FRAMEBUFFER_BINDING
#define GLEXT_GL_READ_FRAMEBUFFER_BINDING GL_READ_FRAMEBUFFER_BINDING

#define GLEXT_framebuffer_blit_dependencies GLEXT_framebuffer_blit, glBlitFramebuffer

// Core since 3.0 - EXT_framebuffer_multisample
#define GLEXT_framebuffer_multisample \
    (SF_GLAD_GL_VERSION_3_0 || SF_GLAD_GL_ARB_framebuffer_object || SF_GLAD_GL_EXT_framebuffer_multisample)
#define GLEXT_glRenderbufferStorageMultisample glRenderbufferStorageMultisample
#define GLEXT_GL_MAX_SAMPLES                   GL_MAX_SAMPLES

#define GLEXT_framebuffer_multisample_dependencies \
    GLEXT_framebuffer_multisample, glRenderbufferStorageMultisample

// Core since 3.1 - ARB_copy_buffer
#define GLEXT_copy_buffer          (SF_GLAD_GL_VERSION_3_1 || SF_GLAD_GL_ARB_copy_buffer)
#define GLEXT_GL_COPY_READ_BUFFER  GL_COPY_READ_BUFFER
#define GLEXT_GL_COPY_WRITE_BUFFER GL_COPY_WRITE_BUFFER
#define GLEXT_glCopyBufferSubData  glCopyBufferSubData

#define GLEXT_copy_buffer_dependencies GLEXT_copy_buffer, glCopyBufferSubData

// Core since 3.2 - ARB_geometry_shader4
#define GLEXT_geometry_shader4         SF_GLAD_GL_ARB_geometry_shader4
#define GLEXT_GL_GEOMETRY_SHADER       GL_GEOMETRY_SHADER_ARB

#endif

// OpenGL Versions
#define GLEXT_GL_VERSION_1_0 SF_GLAD_GL_VERSION_1_0
#define GLEXT_GL_VERSION_1_1 SF_GLAD_GL_VERSION_1_1
#define GLEXT_GL_VERSION_1_2 SF_GLAD_GL_VERSION_1_2
#define GLEXT_GL_VERSION_1_3 SF_GLAD_GL_VERSION_1_3
#define GLEXT_GL_VERSION_1_4 SF_GLAD_GL_VERSION_1_4
#define GLEXT_GL_VERSION_1_5 SF_GLAD_GL_VERSION_1_5
#define GLEXT_GL_VERSION_2_0 SF_GLAD_GL_VERSION_2_0
#define GLEXT_GL_VERSION_2_1 SF_GLAD_GL_VERSION_2_1
#define GLEXT_GL_VERSION_3_0 SF_GLAD_GL_VERSION_3_0
#define GLEXT_GL_VERSION_3_1 SF_GLAD_GL_VERSION_3_1
#define GLEXT_GL_VERSION_3_2 SF_GLAD_GL_VERSION_3_2
#define GLEXT_GL_VERSION_3_3 SF_GLAD_GL_VERSION_3_3
#define GLEXT_GL_VERSION_4_0 SF_GLAD_GL_VERSION_4_0
#define GLEXT_GL_VERSION_4_1 SF_GLAD_GL_VERSION_4_1
#define GLEXT_GL_VERSION_4_2 SF_GLAD_GL_VERSION_4_2
#define GLEXT_GL_VERSION_4_3 SF_GLAD_GL_VERSION_4_3
#define GLEXT_GL_VERSION_4_4 SF_GLAD_GL_VERSION_4_4
#define GLEXT_GL_VERSION_4_5 SF_GLAD_GL_VERSION_4_5
#define GLEXT_GL_VERSION_4_6 SF_GLAD_GL_VERSION_4_6

namespace sf::priv
{

////////////////////////////////////////////////////////////
/// \brief Make sure that extensions are initialized
///
////////////////////////////////////////////////////////////
void ensureExtensionsInit();

} // namespace sf::priv
