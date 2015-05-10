////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SF_POINTER_C_GENERATED_HEADER_OPENGL_HPP
#define SF_POINTER_C_GENERATED_HEADER_OPENGL_HPP

#if defined(__glew_h__) || defined(__GLEW_H__)
#error Attempt to include auto-generated header after including glew.h
#endif
#if defined(__gl_h_) || defined(__GL_H__)
#error Attempt to include auto-generated header after including gl.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error Attempt to include auto-generated header after including glext.h
#endif
#if defined(__gltypes_h_)
#error Attempt to include auto-generated header after gltypes.h
#endif
#if defined(__gl_ATI_h_)
#error Attempt to include auto-generated header after including glATI.h
#endif

#define __glew_h__
#define __GLEW_H__
#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gltypes_h_
#define __gl_ATI_h_

#ifndef APIENTRY
    #if defined(__MINGW32__) || defined(__CYGWIN__)
        #define APIENTRY __stdcall
    #elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #define APIENTRY __stdcall
    #else
        #define APIENTRY
    #endif
#endif /*APIENTRY*/

#ifndef CODEGEN_FUNCPTR
    #define CODEGEN_REMOVE_FUNCPTR
    #if defined(_WIN32)
        #define CODEGEN_FUNCPTR APIENTRY
    #else
        #define CODEGEN_FUNCPTR
    #endif
#endif /*CODEGEN_FUNCPTR*/

#ifndef GLAPI
    #if defined(_WIN32)
        #if defined(__MINGW32__) || defined(__CYGWIN__)
            #define GLAPI extern
        #else
            #define GLAPI __declspec(dllimport)
        #endif
    #else
        #define GLAPI extern
    #endif
#endif


#ifndef GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS
#define GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS


#endif /*GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS*/


#include <stddef.h>
#ifndef GLEXT_64_TYPES_DEFINED
/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif
#endif
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef unsigned short GLhalfARB;
typedef unsigned short GLhalf;
typedef GLint GLfixed;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

extern int sfogl_ext_SGIS_texture_edge_clamp;
extern int sfogl_ext_EXT_texture_edge_clamp;
extern int sfogl_ext_EXT_blend_minmax;
extern int sfogl_ext_EXT_blend_subtract;
extern int sfogl_ext_ARB_multitexture;
extern int sfogl_ext_EXT_blend_func_separate;
extern int sfogl_ext_ARB_shading_language_100;
extern int sfogl_ext_ARB_shader_objects;
extern int sfogl_ext_ARB_vertex_shader;
extern int sfogl_ext_ARB_fragment_shader;
extern int sfogl_ext_ARB_texture_non_power_of_two;
extern int sfogl_ext_EXT_blend_equation_separate;
extern int sfogl_ext_EXT_framebuffer_object;

#define GL_CLAMP_TO_EDGE_SGIS 0x812F

#define GL_CLAMP_TO_EDGE_EXT 0x812F

#define GL_BLEND_EQUATION_EXT 0x8009
#define GL_FUNC_ADD_EXT 0x8006
#define GL_MAX_EXT 0x8008
#define GL_MIN_EXT 0x8007

#define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B
#define GL_FUNC_SUBTRACT_EXT 0x800A

#define GL_ACTIVE_TEXTURE_ARB 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB 0x84E2
#define GL_TEXTURE0_ARB 0x84C0
#define GL_TEXTURE10_ARB 0x84CA
#define GL_TEXTURE11_ARB 0x84CB
#define GL_TEXTURE12_ARB 0x84CC
#define GL_TEXTURE13_ARB 0x84CD
#define GL_TEXTURE14_ARB 0x84CE
#define GL_TEXTURE15_ARB 0x84CF
#define GL_TEXTURE16_ARB 0x84D0
#define GL_TEXTURE17_ARB 0x84D1
#define GL_TEXTURE18_ARB 0x84D2
#define GL_TEXTURE19_ARB 0x84D3
#define GL_TEXTURE1_ARB 0x84C1
#define GL_TEXTURE20_ARB 0x84D4
#define GL_TEXTURE21_ARB 0x84D5
#define GL_TEXTURE22_ARB 0x84D6
#define GL_TEXTURE23_ARB 0x84D7
#define GL_TEXTURE24_ARB 0x84D8
#define GL_TEXTURE25_ARB 0x84D9
#define GL_TEXTURE26_ARB 0x84DA
#define GL_TEXTURE27_ARB 0x84DB
#define GL_TEXTURE28_ARB 0x84DC
#define GL_TEXTURE29_ARB 0x84DD
#define GL_TEXTURE2_ARB 0x84C2
#define GL_TEXTURE30_ARB 0x84DE
#define GL_TEXTURE31_ARB 0x84DF
#define GL_TEXTURE3_ARB 0x84C3
#define GL_TEXTURE4_ARB 0x84C4
#define GL_TEXTURE5_ARB 0x84C5
#define GL_TEXTURE6_ARB 0x84C6
#define GL_TEXTURE7_ARB 0x84C7
#define GL_TEXTURE8_ARB 0x84C8
#define GL_TEXTURE9_ARB 0x84C9

#define GL_BLEND_DST_ALPHA_EXT 0x80CA
#define GL_BLEND_DST_RGB_EXT 0x80C8
#define GL_BLEND_SRC_ALPHA_EXT 0x80CB
#define GL_BLEND_SRC_RGB_EXT 0x80C9

#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C

#define GL_BOOL_ARB 0x8B56
#define GL_BOOL_VEC2_ARB 0x8B57
#define GL_BOOL_VEC3_ARB 0x8B58
#define GL_BOOL_VEC4_ARB 0x8B59
#define GL_FLOAT_MAT2_ARB 0x8B5A
#define GL_FLOAT_MAT3_ARB 0x8B5B
#define GL_FLOAT_MAT4_ARB 0x8B5C
#define GL_FLOAT_VEC2_ARB 0x8B50
#define GL_FLOAT_VEC3_ARB 0x8B51
#define GL_FLOAT_VEC4_ARB 0x8B52
#define GL_INT_VEC2_ARB 0x8B53
#define GL_INT_VEC3_ARB 0x8B54
#define GL_INT_VEC4_ARB 0x8B55
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#define GL_OBJECT_TYPE_ARB 0x8B4E
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#define GL_PROGRAM_OBJECT_ARB 0x8B40
#define GL_SAMPLER_1D_ARB 0x8B5D
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#define GL_SAMPLER_2D_ARB 0x8B5E
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#define GL_SAMPLER_3D_ARB 0x8B5F
#define GL_SAMPLER_CUBE_ARB 0x8B60
#define GL_SHADER_OBJECT_ARB 0x8B48

#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB 0x8B4D
#define GL_MAX_VARYING_FLOATS_ARB 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB 0x8B4A
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB 0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB 0x8B8A
#define GL_VERTEX_SHADER_ARB 0x8B31

#define GL_FRAGMENT_SHADER_ARB 0x8B30
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49

#define GL_BLEND_EQUATION_ALPHA_EXT 0x883D
#define GL_BLEND_EQUATION_RGB_EXT 0x8009

#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT 0x8D53
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#define GL_RENDERBUFFER_BLUE_SIZE_EXT 0x8D52
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT 0x8D54
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_RENDERBUFFER_GREEN_SIZE_EXT 0x8D51
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_RENDERBUFFER_RED_SIZE_EXT 0x8D50
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT 0x8D55
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#define GL_STENCIL_INDEX16_EXT 0x8D49
#define GL_STENCIL_INDEX1_EXT 0x8D46
#define GL_STENCIL_INDEX4_EXT 0x8D47
#define GL_STENCIL_INDEX8_EXT 0x8D48

#define GL_2D 0x0600
#define GL_2_BYTES 0x1407
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_3_BYTES 0x1408
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_4_BYTES 0x1409
#define GL_ACCUM 0x0100
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ADD 0x0104
#define GL_ALL_ATTRIB_BITS 0xFFFFFFFF
#define GL_ALPHA 0x1906
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_ALPHA_BITS 0x0D55
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_ALWAYS 0x0207
#define GL_AMBIENT 0x1200
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_AND 0x1501
#define GL_AND_INVERTED 0x1504
#define GL_AND_REVERSE 0x1502
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_AUTO_NORMAL 0x0D80
#define GL_BACK 0x0405
#define GL_BITMAP 0x1A00
#define GL_BITMAP_TOKEN 0x0704
#define GL_BLEND 0x0BE2
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLUE 0x1905
#define GL_BLUE_BIAS 0x0D1B
#define GL_BLUE_BITS 0x0D54
#define GL_BLUE_SCALE 0x0D1A
#define GL_BYTE 0x1400
#define GL_C3F_V3F 0x2A24
#define GL_C4F_N3F_V3F 0x2A26
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_CCW 0x0901
#define GL_CLAMP 0x2900
#define GL_CLEAR 0x1500
#define GL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_COEFF 0x0A00
#define GL_COLOR 0x1800
#define GL_COLOR_ARRAY 0x8076
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_INDEX 0x1900
#define GL_COLOR_INDEXES 0x1603
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_COLOR_MATERIAL 0x0B57
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_COPY 0x1503
#define GL_COPY_INVERTED 0x150C
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_CURRENT_BIT 0x00000001
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CW 0x0900
#define GL_DECAL 0x2101
#define GL_DECR 0x1E03
#define GL_DEPTH 0x1801
#define GL_DEPTH_BIAS 0x0D1F
#define GL_DEPTH_BITS 0x0D56
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_FUNC 0x0B74
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DITHER 0x0BD0
#define GL_DOMAIN 0x0A02
#define GL_DONT_CARE 0x1100
#define GL_DOUBLE 0x140A
#define GL_DOUBLEBUFFER 0x0C32
#define GL_DRAW_BUFFER 0x0C01
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_DST_ALPHA 0x0304
#define GL_DST_COLOR 0x0306
#define GL_EDGE_FLAG 0x0B43
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_EMISSION 0x1600
#define GL_ENABLE_BIT 0x00002000
#define GL_EQUAL 0x0202
#define GL_EQUIV 0x1509
#define GL_EVAL_BIT 0x00010000
#define GL_EXP 0x0800
#define GL_EXP2 0x0801
#define GL_EXTENSIONS 0x1F03
#define GL_EYE_LINEAR 0x2400
#define GL_EYE_PLANE 0x2502
#define GL_FALSE 0
#define GL_FASTEST 0x1101
#define GL_FEEDBACK 0x1C01
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_FILL 0x1B02
#define GL_FLAT 0x1D00
#define GL_FLOAT 0x1406
#define GL_FRONT 0x0404
#define GL_FRONT_AND_BACK 0x0408
#define GL_FRONT_FACE 0x0B46
#define GL_GEQUAL 0x0206
#define GL_GREATER 0x0204
#define GL_GREEN 0x1904
#define GL_GREEN_BIAS 0x0D19
#define GL_GREEN_BITS 0x0D53
#define GL_GREEN_SCALE 0x0D18
#define GL_HINT_BIT 0x00008000
#define GL_INCR 0x1E02
#define GL_INDEX_ARRAY 0x8077
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_BITS 0x0D51
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_INDEX_MODE 0x0C30
#define GL_INDEX_OFFSET 0x0D13
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_INT 0x1404
#define GL_INTENSITY 0x8049
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_OPERATION 0x0502
#define GL_INVALID_VALUE 0x0501
#define GL_INVERT 0x150A
#define GL_KEEP 0x1E00
#define GL_LEQUAL 0x0203
#define GL_LESS 0x0201
#define GL_LIGHTING 0x0B50
#define GL_LINE 0x1B01
#define GL_LINEAR 0x2601
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_LINES 0x0001
#define GL_LINE_BIT 0x00000004
#define GL_LINE_LOOP 0x0002
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LINE_STRIP 0x0003
#define GL_LINE_TOKEN 0x0702
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LOAD 0x0101
#define GL_LOGIC_OP 0x0BF1
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_MATRIX_MODE 0x0BA0
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MODELVIEW 0x1700
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_MODULATE 0x2100
#define GL_MULT 0x0103
#define GL_N3F_V3F 0x2A25
#define GL_NAND 0x150E
#define GL_NEAREST 0x2600
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_NEVER 0x0200
#define GL_NICEST 0x1102
#define GL_NONE 0
#define GL_NOOP 0x1505
#define GL_NOR 0x1508
#define GL_NORMALIZE 0x0BA1
#define GL_NORMAL_ARRAY 0x8075
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NOTEQUAL 0x0205
#define GL_NO_ERROR 0
#define GL_OBJECT_LINEAR 0x2401
#define GL_OBJECT_PLANE 0x2501
#define GL_ONE 1
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_OR 0x1507
#define GL_ORDER 0x0A01
#define GL_OR_INVERTED 0x150D
#define GL_OR_REVERSE 0x150B
#define GL_OUT_OF_MEMORY 0x0505
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_POINT 0x1B00
#define GL_POINTS 0x0000
#define GL_POINT_BIT 0x00000002
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_POINT_TOKEN 0x0701
#define GL_POLYGON 0x0009
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_POLYGON_TOKEN 0x0703
#define GL_POSITION 0x1203
#define GL_PROJECTION 0x1701
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_Q 0x2003
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_R 0x2002
#define GL_R3_G3_B2 0x2A10
#define GL_READ_BUFFER 0x0C02
#define GL_RED 0x1903
#define GL_RED_BIAS 0x0D15
#define GL_RED_BITS 0x0D52
#define GL_RED_SCALE 0x0D14
#define GL_RENDER 0x1C00
#define GL_RENDERER 0x1F01
#define GL_RENDER_MODE 0x0C40
#define GL_REPEAT 0x2901
#define GL_REPLACE 0x1E01
#define GL_RETURN 0x0102
#define GL_RGB 0x1907
#define GL_RGB10 0x8052
#define GL_RGB10_A2 0x8059
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB5_A1 0x8057
#define GL_RGB8 0x8051
#define GL_RGBA 0x1908
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGBA8 0x8058
#define GL_RGBA_MODE 0x0C31
#define GL_S 0x2000
#define GL_SCISSOR_BIT 0x00080000
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_SET 0x150F
#define GL_SHADE_MODEL 0x0B54
#define GL_SHININESS 0x1601
#define GL_SHORT 0x1402
#define GL_SMOOTH 0x1D01
#define GL_SPECULAR 0x1202
#define GL_SPHERE_MAP 0x2402
#define GL_SRC_ALPHA 0x0302
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_SRC_COLOR 0x0300
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_STENCIL 0x1802
#define GL_STENCIL_BITS 0x0D57
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_INDEX 0x1901
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_STEREO 0x0C33
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_T 0x2001
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_V3F 0x2A27
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_T4F_V4F 0x2A28
#define GL_TEXTURE 0x1702
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_TEXTURE_BIT 0x00040000
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_BORDER 0x1005
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_COMPONENTS 0x1003
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_ENV 0x2300
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TRANSFORM_BIT 0x00001000
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_FAN 0x0006
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRUE 1
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_SHORT 0x1403
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_VENDOR 0x1F00
#define GL_VERSION 0x1F02
#define GL_VERTEX_ARRAY 0x8074
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VIEWPORT 0x0BA2
#define GL_VIEWPORT_BIT 0x00000800
#define GL_XOR 0x1506
#define GL_ZERO 0
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17

#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glBlendEquationEXT)(GLenum);
#define glBlendEquationEXT sf_ptrc_glBlendEquationEXT
#endif /*GL_EXT_blend_minmax*/


#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glActiveTextureARB)(GLenum);
#define glActiveTextureARB sf_ptrc_glActiveTextureARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glClientActiveTextureARB)(GLenum);
#define glClientActiveTextureARB sf_ptrc_glClientActiveTextureARB
#endif /*GL_ARB_multitexture*/

#ifndef GL_EXT_blend_func_separate
#define GL_EXT_blend_func_separate 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glBlendFuncSeparateEXT)(GLenum, GLenum, GLenum, GLenum);
#define glBlendFuncSeparateEXT sf_ptrc_glBlendFuncSeparateEXT
#endif /*GL_EXT_blend_func_separate*/

#ifndef GL_ARB_shader_objects
#define GL_ARB_shader_objects 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glAttachObjectARB)(GLhandleARB, GLhandleARB);
#define glAttachObjectARB sf_ptrc_glAttachObjectARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glCompileShaderARB)(GLhandleARB);
#define glCompileShaderARB sf_ptrc_glCompileShaderARB
extern GLhandleARB (CODEGEN_FUNCPTR *sf_ptrc_glCreateProgramObjectARB)();
#define glCreateProgramObjectARB sf_ptrc_glCreateProgramObjectARB
extern GLhandleARB (CODEGEN_FUNCPTR *sf_ptrc_glCreateShaderObjectARB)(GLenum);
#define glCreateShaderObjectARB sf_ptrc_glCreateShaderObjectARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteObjectARB)(GLhandleARB);
#define glDeleteObjectARB sf_ptrc_glDeleteObjectARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glDetachObjectARB)(GLhandleARB, GLhandleARB);
#define glDetachObjectARB sf_ptrc_glDetachObjectARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetActiveUniformARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
#define glGetActiveUniformARB sf_ptrc_glGetActiveUniformARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetAttachedObjectsARB)(GLhandleARB, GLsizei, GLsizei *, GLhandleARB *);
#define glGetAttachedObjectsARB sf_ptrc_glGetAttachedObjectsARB
extern GLhandleARB (CODEGEN_FUNCPTR *sf_ptrc_glGetHandleARB)(GLenum);
#define glGetHandleARB sf_ptrc_glGetHandleARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetInfoLogARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
#define glGetInfoLogARB sf_ptrc_glGetInfoLogARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetObjectParameterfvARB)(GLhandleARB, GLenum, GLfloat *);
#define glGetObjectParameterfvARB sf_ptrc_glGetObjectParameterfvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetObjectParameterivARB)(GLhandleARB, GLenum, GLint *);
#define glGetObjectParameterivARB sf_ptrc_glGetObjectParameterivARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetShaderSourceARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
#define glGetShaderSourceARB sf_ptrc_glGetShaderSourceARB
extern GLint (CODEGEN_FUNCPTR *sf_ptrc_glGetUniformLocationARB)(GLhandleARB, const GLcharARB *);
#define glGetUniformLocationARB sf_ptrc_glGetUniformLocationARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetUniformfvARB)(GLhandleARB, GLint, GLfloat *);
#define glGetUniformfvARB sf_ptrc_glGetUniformfvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetUniformivARB)(GLhandleARB, GLint, GLint *);
#define glGetUniformivARB sf_ptrc_glGetUniformivARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glLinkProgramARB)(GLhandleARB);
#define glLinkProgramARB sf_ptrc_glLinkProgramARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glShaderSourceARB)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *);
#define glShaderSourceARB sf_ptrc_glShaderSourceARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1fARB)(GLint, GLfloat);
#define glUniform1fARB sf_ptrc_glUniform1fARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1fvARB)(GLint, GLsizei, const GLfloat *);
#define glUniform1fvARB sf_ptrc_glUniform1fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1iARB)(GLint, GLint);
#define glUniform1iARB sf_ptrc_glUniform1iARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1ivARB)(GLint, GLsizei, const GLint *);
#define glUniform1ivARB sf_ptrc_glUniform1ivARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2fARB)(GLint, GLfloat, GLfloat);
#define glUniform2fARB sf_ptrc_glUniform2fARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2fvARB)(GLint, GLsizei, const GLfloat *);
#define glUniform2fvARB sf_ptrc_glUniform2fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2iARB)(GLint, GLint, GLint);
#define glUniform2iARB sf_ptrc_glUniform2iARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2ivARB)(GLint, GLsizei, const GLint *);
#define glUniform2ivARB sf_ptrc_glUniform2ivARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3fARB)(GLint, GLfloat, GLfloat, GLfloat);
#define glUniform3fARB sf_ptrc_glUniform3fARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3fvARB)(GLint, GLsizei, const GLfloat *);
#define glUniform3fvARB sf_ptrc_glUniform3fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3iARB)(GLint, GLint, GLint, GLint);
#define glUniform3iARB sf_ptrc_glUniform3iARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3ivARB)(GLint, GLsizei, const GLint *);
#define glUniform3ivARB sf_ptrc_glUniform3ivARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4fARB)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
#define glUniform4fARB sf_ptrc_glUniform4fARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4fvARB)(GLint, GLsizei, const GLfloat *);
#define glUniform4fvARB sf_ptrc_glUniform4fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4iARB)(GLint, GLint, GLint, GLint, GLint);
#define glUniform4iARB sf_ptrc_glUniform4iARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4ivARB)(GLint, GLsizei, const GLint *);
#define glUniform4ivARB sf_ptrc_glUniform4ivARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniformMatrix2fvARB)(GLint, GLsizei, GLboolean, const GLfloat *);
#define glUniformMatrix2fvARB sf_ptrc_glUniformMatrix2fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniformMatrix3fvARB)(GLint, GLsizei, GLboolean, const GLfloat *);
#define glUniformMatrix3fvARB sf_ptrc_glUniformMatrix3fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUniformMatrix4fvARB)(GLint, GLsizei, GLboolean, const GLfloat *);
#define glUniformMatrix4fvARB sf_ptrc_glUniformMatrix4fvARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glUseProgramObjectARB)(GLhandleARB);
#define glUseProgramObjectARB sf_ptrc_glUseProgramObjectARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glValidateProgramARB)(GLhandleARB);
#define glValidateProgramARB sf_ptrc_glValidateProgramARB
#endif /*GL_ARB_shader_objects*/

#ifndef GL_ARB_vertex_shader
#define GL_ARB_vertex_shader 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glBindAttribLocationARB)(GLhandleARB, GLuint, const GLcharARB *);
#define glBindAttribLocationARB sf_ptrc_glBindAttribLocationARB
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetActiveAttribARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
#define glGetActiveAttribARB sf_ptrc_glGetActiveAttribARB
extern GLint (CODEGEN_FUNCPTR *sf_ptrc_glGetAttribLocationARB)(GLhandleARB, const GLcharARB *);
#define glGetAttribLocationARB sf_ptrc_glGetAttribLocationARB
#endif /*GL_ARB_vertex_shader*/



#ifndef GL_EXT_blend_equation_separate
#define GL_EXT_blend_equation_separate 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glBlendEquationSeparateEXT)(GLenum, GLenum);
#define glBlendEquationSeparateEXT sf_ptrc_glBlendEquationSeparateEXT
#endif /*GL_EXT_blend_equation_separate*/

#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object 1
extern void (CODEGEN_FUNCPTR *sf_ptrc_glBindFramebufferEXT)(GLenum, GLuint);
#define glBindFramebufferEXT sf_ptrc_glBindFramebufferEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glBindRenderbufferEXT)(GLenum, GLuint);
#define glBindRenderbufferEXT sf_ptrc_glBindRenderbufferEXT
extern GLenum (CODEGEN_FUNCPTR *sf_ptrc_glCheckFramebufferStatusEXT)(GLenum);
#define glCheckFramebufferStatusEXT sf_ptrc_glCheckFramebufferStatusEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteFramebuffersEXT)(GLsizei, const GLuint *);
#define glDeleteFramebuffersEXT sf_ptrc_glDeleteFramebuffersEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteRenderbuffersEXT)(GLsizei, const GLuint *);
#define glDeleteRenderbuffersEXT sf_ptrc_glDeleteRenderbuffersEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferRenderbufferEXT)(GLenum, GLenum, GLenum, GLuint);
#define glFramebufferRenderbufferEXT sf_ptrc_glFramebufferRenderbufferEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferTexture1DEXT)(GLenum, GLenum, GLenum, GLuint, GLint);
#define glFramebufferTexture1DEXT sf_ptrc_glFramebufferTexture1DEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferTexture2DEXT)(GLenum, GLenum, GLenum, GLuint, GLint);
#define glFramebufferTexture2DEXT sf_ptrc_glFramebufferTexture2DEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferTexture3DEXT)(GLenum, GLenum, GLenum, GLuint, GLint, GLint);
#define glFramebufferTexture3DEXT sf_ptrc_glFramebufferTexture3DEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGenFramebuffersEXT)(GLsizei, GLuint *);
#define glGenFramebuffersEXT sf_ptrc_glGenFramebuffersEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGenRenderbuffersEXT)(GLsizei, GLuint *);
#define glGenRenderbuffersEXT sf_ptrc_glGenRenderbuffersEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGenerateMipmapEXT)(GLenum);
#define glGenerateMipmapEXT sf_ptrc_glGenerateMipmapEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetFramebufferAttachmentParameterivEXT)(GLenum, GLenum, GLenum, GLint *);
#define glGetFramebufferAttachmentParameterivEXT sf_ptrc_glGetFramebufferAttachmentParameterivEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glGetRenderbufferParameterivEXT)(GLenum, GLenum, GLint *);
#define glGetRenderbufferParameterivEXT sf_ptrc_glGetRenderbufferParameterivEXT
extern GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsFramebufferEXT)(GLuint);
#define glIsFramebufferEXT sf_ptrc_glIsFramebufferEXT
extern GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsRenderbufferEXT)(GLuint);
#define glIsRenderbufferEXT sf_ptrc_glIsRenderbufferEXT
extern void (CODEGEN_FUNCPTR *sf_ptrc_glRenderbufferStorageEXT)(GLenum, GLenum, GLsizei, GLsizei);
#define glRenderbufferStorageEXT sf_ptrc_glRenderbufferStorageEXT
#endif /*GL_EXT_framebuffer_object*/

GLAPI void APIENTRY glBlendFunc(GLenum, GLenum);
GLAPI void APIENTRY glClear(GLbitfield);
GLAPI void APIENTRY glClearColor(GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glClearDepth(GLdouble);
GLAPI void APIENTRY glClearStencil(GLint);
GLAPI void APIENTRY glClipPlane(GLenum, const GLdouble *);
GLAPI void APIENTRY glColorMask(GLboolean, GLboolean, GLboolean, GLboolean);
GLAPI void APIENTRY glCopyPixels(GLint, GLint, GLsizei, GLsizei, GLenum);
GLAPI void APIENTRY glCullFace(GLenum);
GLAPI void APIENTRY glDepthFunc(GLenum);
GLAPI void APIENTRY glDepthMask(GLboolean);
GLAPI void APIENTRY glDepthRange(GLdouble, GLdouble);
GLAPI void APIENTRY glDisable(GLenum);
GLAPI void APIENTRY glDrawBuffer(GLenum);
GLAPI void APIENTRY glEnable(GLenum);
GLAPI void APIENTRY glFinish();
GLAPI void APIENTRY glFlush();
GLAPI void APIENTRY glFrontFace(GLenum);
GLAPI void APIENTRY glFrustum(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI void APIENTRY glGetBooleanv(GLenum, GLboolean *);
GLAPI void APIENTRY glGetDoublev(GLenum, GLdouble *);
GLAPI GLenum APIENTRY glGetError();
GLAPI void APIENTRY glGetFloatv(GLenum, GLfloat *);
GLAPI void APIENTRY glGetIntegerv(GLenum, GLint *);
GLAPI const GLubyte * APIENTRY glGetString(GLenum);
GLAPI void APIENTRY glGetTexEnvfv(GLenum, GLenum, GLfloat *);
GLAPI void APIENTRY glGetTexEnviv(GLenum, GLenum, GLint *);
GLAPI void APIENTRY glGetTexGendv(GLenum, GLenum, GLdouble *);
GLAPI void APIENTRY glGetTexGenfv(GLenum, GLenum, GLfloat *);
GLAPI void APIENTRY glGetTexGeniv(GLenum, GLenum, GLint *);
GLAPI void APIENTRY glGetTexImage(GLenum, GLint, GLenum, GLenum, GLvoid *);
GLAPI void APIENTRY glGetTexLevelParameterfv(GLenum, GLint, GLenum, GLfloat *);
GLAPI void APIENTRY glGetTexLevelParameteriv(GLenum, GLint, GLenum, GLint *);
GLAPI void APIENTRY glGetTexParameterfv(GLenum, GLenum, GLfloat *);
GLAPI void APIENTRY glGetTexParameteriv(GLenum, GLenum, GLint *);
GLAPI void APIENTRY glHint(GLenum, GLenum);
GLAPI void APIENTRY glIndexMask(GLuint);
GLAPI GLboolean APIENTRY glIsEnabled(GLenum);
GLAPI void APIENTRY glLineWidth(GLfloat);
GLAPI void APIENTRY glLoadIdentity();
GLAPI void APIENTRY glLoadMatrixd(const GLdouble *);
GLAPI void APIENTRY glLoadMatrixf(const GLfloat *);
GLAPI void APIENTRY glMatrixMode(GLenum);
GLAPI void APIENTRY glMultMatrixd(const GLdouble *);
GLAPI void APIENTRY glMultMatrixf(const GLfloat *);
GLAPI void APIENTRY glOrtho(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI void APIENTRY glPointSize(GLfloat);
GLAPI void APIENTRY glPopAttrib();
GLAPI void APIENTRY glPopMatrix();
GLAPI void APIENTRY glPushAttrib(GLbitfield);
GLAPI void APIENTRY glPushMatrix();
GLAPI void APIENTRY glReadBuffer(GLenum);
GLAPI void APIENTRY glReadPixels(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *);
GLAPI void APIENTRY glScissor(GLint, GLint, GLsizei, GLsizei);
GLAPI void APIENTRY glShadeModel(GLenum);
GLAPI void APIENTRY glStencilFunc(GLenum, GLint, GLuint);
GLAPI void APIENTRY glStencilMask(GLuint);
GLAPI void APIENTRY glStencilOp(GLenum, GLenum, GLenum);
GLAPI void APIENTRY glTexImage1D(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
GLAPI void APIENTRY glTexImage2D(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
GLAPI void APIENTRY glTexParameterf(GLenum, GLenum, GLfloat);
GLAPI void APIENTRY glTexParameterfv(GLenum, GLenum, const GLfloat *);
GLAPI void APIENTRY glTexParameteri(GLenum, GLenum, GLint);
GLAPI void APIENTRY glTexParameteriv(GLenum, GLenum, const GLint *);
GLAPI void APIENTRY glViewport(GLint, GLint, GLsizei, GLsizei);

GLAPI void APIENTRY glBindTexture(GLenum, GLuint);
GLAPI void APIENTRY glColorPointer(GLint, GLenum, GLsizei, const GLvoid *);
GLAPI void APIENTRY glCopyTexImage1D(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
GLAPI void APIENTRY glCopyTexImage2D(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
GLAPI void APIENTRY glCopyTexSubImage1D(GLenum, GLint, GLint, GLint, GLint, GLsizei);
GLAPI void APIENTRY glCopyTexSubImage2D(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
GLAPI void APIENTRY glDeleteTextures(GLsizei, const GLuint *);
GLAPI void APIENTRY glDisableClientState(GLenum);
GLAPI void APIENTRY glDrawArrays(GLenum, GLint, GLsizei);
GLAPI void APIENTRY glDrawElements(GLenum, GLsizei, GLenum, const GLvoid *);
GLAPI void APIENTRY glEnableClientState(GLenum);
GLAPI void APIENTRY glGenTextures(GLsizei, GLuint *);
GLAPI void APIENTRY glGetPointerv(GLenum, GLvoid **);
GLAPI void APIENTRY glNormalPointer(GLenum, GLsizei, const GLvoid *);
GLAPI void APIENTRY glPolygonOffset(GLfloat, GLfloat);
GLAPI void APIENTRY glPopClientAttrib();
GLAPI void APIENTRY glPushClientAttrib(GLbitfield);
GLAPI void APIENTRY glTexCoordPointer(GLint, GLenum, GLsizei, const GLvoid *);
GLAPI void APIENTRY glTexSubImage1D(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
GLAPI void APIENTRY glTexSubImage2D(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
GLAPI void APIENTRY glVertexPointer(GLint, GLenum, GLsizei, const GLvoid *);

enum sfogl_LoadStatus
{
    sfogl_LOAD_FAILED = 0,
    sfogl_LOAD_SUCCEEDED = 1
};

int sfogl_LoadFunctions();

int sfogl_GetMinorVersion();
int sfogl_GetMajorVersion();
int sfogl_IsVersionGEQ(int majorVersion, int minorVersion);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif //SF_POINTER_C_GENERATED_HEADER_OPENGL_HPP
