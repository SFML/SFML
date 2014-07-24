////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_GLEXTENSIONS_HPP
#define SFML_GLEXTENSIONS_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#ifdef SFML_OPENGL_ES

    #include <SFML/OpenGL.hpp>

#ifdef SFML_SYSTEM_ANDROID
    // Hack to make transparency working on some Android devices
    #define GLEXT_blend_func_separate              false
    #define GLEXT_blend_equation_separate          false
#else
    #define GLEXT_blend_func_separate              GL_OES_blend_func_separate
    #define GLEXT_blend_equation_separate          GL_OES_blend_equation_separate
#endif
    #define GLEXT_glBlendFuncSeparate              glBlendFuncSeparateOES
    #define GLEXT_glBlendEquationSeparate          glBlendEquationSeparateOES
    #define GLEXT_framebuffer_object               GL_OES_framebuffer_object
    #define GLEXT_glGenFramebuffers                glGenFramebuffersOES
    #define GLEXT_glGenRenderbuffers               glGenRenderbuffersOES
    #define GLEXT_glBindFramebuffer                glBindFramebufferOES
    #define GLEXT_glBindRenderbuffer               glBindRenderbufferOES
    #define GLEXT_glDeleteFramebuffers             glDeleteFramebuffersOES
    #define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffersOES
    #define GLEXT_glRenderbufferStorage            glRenderbufferStorageOES
    #define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbufferOES
    #define GLEXT_glFramebufferTexture2D           glFramebufferTexture2DOES
    #define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatusOES
    #define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER_OES
    #define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING_OES
    #define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER_OES
    #define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0_OES
    #define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT_OES
    #define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE_OES
    #define GLEXT_GL_DEPTH_COMPONENT               GL_DEPTH_COMPONENT16_OES
    #define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION_OES
    #define GLEXT_texture_non_power_of_two         false
    #define GLEXT_multitexture                     true
    #define GLEXT_glClientActiveTexture            glClientActiveTexture
    #define GLEXT_glActiveTexture                  glActiveTexture
    #define GLEXT_GL_TEXTURE0                      GL_TEXTURE0
    #define GLEXT_glBlendEquation                  glBlendEquationOES
    #define GLEXT_GL_FUNC_ADD                      GL_FUNC_ADD_OES
    #define GLEXT_GL_FUNC_SUBTRACT                 GL_FUNC_SUBTRACT_OES

#else

    #include <GL/glew.h>
    #include <SFML/OpenGL.hpp>

    #define GLEXT_blend_func_separate              GLEW_EXT_blend_func_separate
    #define GLEXT_blend_equation_separate          GLEW_EXT_blend_equation_separate
    #define GLEXT_glBlendFuncSeparate              glBlendFuncSeparateEXT
    #define GLEXT_glBlendEquationSeparate          glBlendEquationSeparateEXT
    #define GLEXT_framebuffer_object               GLEW_EXT_framebuffer_object
    #define GLEXT_glGenFramebuffers                glGenFramebuffersEXT
    #define GLEXT_glGenRenderbuffers               glGenRenderbuffersEXT
    #define GLEXT_glBindFramebuffer                glBindFramebufferEXT
    #define GLEXT_glBindRenderbuffer               glBindRenderbufferEXT
    #define GLEXT_glDeleteFramebuffers             glDeleteFramebuffersEXT
    #define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffersEXT
    #define GLEXT_glRenderbufferStorage            glRenderbufferStorageEXT
    #define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbufferEXT
    #define GLEXT_glFramebufferTexture2D           glFramebufferTexture2DEXT
    #define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatusEXT
    #define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER_EXT
    #define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING_EXT
    #define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER_EXT
    #define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0_EXT
    #define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT_EXT
    #define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE_EXT
    #define GLEXT_GL_DEPTH_COMPONENT               GL_DEPTH_COMPONENT
    #define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION_EXT
    #define GLEXT_texture_non_power_of_two         GLEW_ARB_texture_non_power_of_two
    #define GLEXT_multitexture                     GLEW_ARB_multitexture
    #define GLEXT_glClientActiveTexture            glClientActiveTextureARB
    #define GLEXT_glActiveTexture                  glActiveTextureARB
    #define GLEXT_GL_TEXTURE0                      GL_TEXTURE0_ARB
    #define GLEXT_glBlendEquation                  glBlendEquation
    #define GLEXT_GL_FUNC_ADD                      GL_FUNC_ADD
    #define GLEXT_GL_FUNC_SUBTRACT                 GL_FUNC_SUBTRACT

#endif

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
/// \brief Make sure that extensions are initialized
///
////////////////////////////////////////////////////////////
void ensureExtensionsInit();

} // namespace priv

} // namespace sf


#endif // SFML_GLEXTENSIONS_HPP
