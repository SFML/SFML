/*
 * Last edit by previous maintainer:
 * 2000/01/06 16:37:43, kusano
 *
 * Copyright (C) 1999 - 2005 Yoshi <yoshi@giganet.net>
 * Copyright (C) 2006 John M. Gabriele <jmg3000@gmail.com>
 * Copyright (C) 2007 James Adam <james@lazyatom.com>
 *
 * This program is distributed under the terms of the MIT license.
 * See the included MIT-LICENSE file for the terms of this license.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _RBOGL_H_
#define _RBOGL_H_

#include <ruby.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <mach-o/dyld.h>
#include <stdlib.h>
#include <string.h>
#elif defined WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif

#include "gl-enums.h"

#ifdef __APPLE__
#ifndef APIENTRY
#define APIENTRY /* to nothing */
#endif
#endif

typedef struct RArray RArray;

extern VALUE cProc;

#define LOAD_GL_FUNC(_NAME_) \
if (fptr_##_NAME_==NULL) \
fptr_##_NAME_ = load_gl_function(#_NAME_, 1);

#define FORCE_PIXEL_STORE_MODE \
	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT); \
	glPixelStorei(GL_PACK_ALIGNMENT, 1); \
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0); \
	glPixelStorei(GL_PACK_SKIP_ROWS, 0); \
	glPixelStorei(GL_PACK_ROW_LENGTH, 0); \
	glPixelStorei(GL_PACK_SKIP_IMAGES, 0); \
	glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
/*	glPixelStorei(GL_PACK_SKIP_VOLUMES_SGIS, 0);
	glPixelStorei(GL_PACK_IMAGE_DEPTH_SGIS, 0); */

#define RESTORE_PIXEL_STORE_MODE \
	glPopClientAttrib();


/* -------------------------------------------------------------------- */
#ifndef NUM2DBL
#define NUM2DBL(_val) num2double(_val) 

static inline double num2double( VALUE val )
{
    struct RFloat* flt;
    if (NIL_P(val)) return 0;
    flt = RFLOAT(f_float(0, val));
    return flt->value;
}
#endif

/* -------------------------------------------------------------------- */
#define ARY2INTEGRAL(_type_,_convert_) \
static inline int ary2c##_type_( arg, cary, maxlen ) \
VALUE arg; \
GL##_type_ cary[]; \
int maxlen; \
{ \
    int i; \
    struct RArray* ary; \
    VALUE entry; \
    ary = RARRAY(rb_Array(arg)); \
    if (maxlen < 1) \
        maxlen = ary->len; \
    else \
        maxlen = maxlen < ary->len ? maxlen : ary->len; \
    for (i=0; i < maxlen; i++) \
    { \
        entry = rb_ary_entry((VALUE)ary,i); \
        cary[i] = (GL##_type_)_convert_(entry); \
    } \
    return i; \
}

ARY2INTEGRAL(int,NUM2INT)
ARY2INTEGRAL(uint,NUM2UINT)
ARY2INTEGRAL(byte,NUM2INT)
ARY2INTEGRAL(ubyte,NUM2INT)
ARY2INTEGRAL(short,NUM2INT)
ARY2INTEGRAL(ushort,NUM2INT)
ARY2INTEGRAL(boolean,NUM2INT)
#undef ARY2INTEGRAL

/* -------------------------------------------------------------------- */
static inline int ary2cflt(arg, cary, maxlen)
VALUE arg;
float cary[];
int maxlen;
{
    int i;
    struct RArray* ary;
    ary = RARRAY(rb_Array(arg));
    if (maxlen < 1)
        maxlen = ary->len;
    else
        maxlen = maxlen < ary->len ? maxlen : ary->len;
    for (i=0; i < maxlen; i++)
        cary[i] = (float)NUM2DBL(rb_ary_entry((VALUE)ary,i));
    return i;
}

/* -------------------------------------------------------------------- */
static inline int ary2cdbl(arg, cary, maxlen)
VALUE arg;
double cary[];
int maxlen;
{
    int i;
    struct RArray* ary;
    ary = RARRAY(rb_Array(arg));
    if (maxlen < 1)
        maxlen = ary->len;
    else
        maxlen = maxlen < ary->len ? maxlen : ary->len;
    for (i=0; i < maxlen; i++)
        cary[i] = NUM2DBL(rb_ary_entry((VALUE)ary,i));
    return i;
}

/* -------------------------------------------------------------------- */
static inline void mary2ary(src, ary)
VALUE src;
VALUE ary;
{
    struct RArray* tmp_ary;
    int i;
    tmp_ary = RARRAY(rb_Array(src));
    for (i = 0; i < tmp_ary->len; i++)
    {
        if (TYPE(tmp_ary->ptr[i]) == T_ARRAY)
            mary2ary((VALUE)tmp_ary->ptr[i], ary);
        else
            rb_ary_push(ary, tmp_ary->ptr[i]);
    }
}

/* -------------------------------------------------------------------- */
static inline void ary2cmat4x4dbl(ary, cary)
VALUE ary;
double cary[];
{
    int i,j;
    RArray *ary_r,*ary_c;
    memset(cary, 0x0, sizeof(double[4*4]));
    ary_c = RARRAY(rb_Array(ary));
    if (TYPE(ary_c->ptr[0]) != T_ARRAY)
        ary2cdbl((VALUE)ary_c, cary, 16);
    else
    {
        for (i = 0; i < ary_c->len && i < 4; i++)
        {
            ary_r = RARRAY(rb_Array(ary_c->ptr[i]));
            for(j = 0; j < ary_r->len && j < 4; j++)
                cary[i*4+j] = (GLdouble)NUM2DBL(ary_r->ptr[j]);
        }
    }
}

static inline void ary2cmat4x4flt(ary, cary)
VALUE ary;
float cary[];
{
    int i,j;
    RArray *ary_r,*ary_c;
    memset(cary, 0x0, sizeof(float[4*4]));
    ary_c = RARRAY(rb_Array(ary));
    if (TYPE(ary_c->ptr[0]) != T_ARRAY)
        ary2cflt((VALUE)ary_c, cary, 16);
    else
    {
        for (i = 0; i < ary_c->len && i < 4; i++)
        {
            ary_r = RARRAY(rb_Array(ary_c->ptr[i]));
            for(j = 0; j < ary_r->len && j < 4; j++)
                cary[i*4+j] = (GLfloat)NUM2DBL(ary_r->ptr[j]);
        }
    }
}

/* -------------------------------------------------------------------- */
/*Need to find proper size for glReadPixels array*/
static inline int glformat_size(GLenum format)
{
    switch(format)
    {
        case GL_COLOR_INDEX:
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
        case GL_ALPHA:
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_LUMINANCE:
            return 1;

        case GL_LUMINANCE_ALPHA:
            return 2;

        case GL_RGB:
        case GL_BGR_EXT:
            return 3;

        case GL_RGBA:
        case GL_BGRA_EXT:
        case GL_ABGR_EXT:
            return 4;
        case 1:
        case 2:
        case 3:
        case 4:
            return format;
        default:
            return -1;
    }
}

/* -------------------------------------------------------------------- */
static inline int gltype_size(GLenum type)
{
    switch(type)
    {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
		case GL_UNSIGNED_BYTE_3_3_2:
		case GL_UNSIGNED_BYTE_2_3_3_REV:
            return 1;

        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_5_6_5_REV:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
            return 2;

        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
		case GL_UNSIGNED_INT_8_8_8_8:
		case GL_UNSIGNED_INT_8_8_8_8_REV:
		case GL_UNSIGNED_INT_10_10_10_2:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
            return 4;

        case GL_BITMAP:
            return 0;

        default:
            return -1;
    }
}

/* -------------------------------------------------------------------- */
static inline VALUE allocate_buffer_with_string( int size )
{
    return rb_str_new(NULL, size);
}

/* -------------------------------------------------------------------- */
static inline void *load_gl_function(const char *name,int raise) 
{
	void *func_ptr = NULL;

#if defined(__APPLE__)
	static const struct mach_header* library = NULL;
	char* symbolName;
	NSSymbol symbol;
	if (library == NULL)
		library = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL",NSADDIMAGE_OPTION_RETURN_ON_ERROR);

	if (library == NULL)
		rb_raise(rb_eRuntimeError,"Can't load OpenGL library for dynamic loading");
		
	/*  prepend a '_' for the Unix C symbol mangling convention  */
	symbolName = ALLOC_N(char,strlen(name) + 2);
	symbolName[0] = '_';
	strcpy(symbolName+1, name);

	symbol = NSLookupSymbolInImage(library,symbolName,NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
	xfree(symbolName);

	if (symbol == NULL)
		func_ptr = NULL;
	else
		func_ptr = NSAddressOfSymbol(symbol);
#elif defined(WIN32) || defined(_WIN32)
	func_ptr = wglGetProcAddress((LPCSTR)name);
#elif defined(GLX_VERSION_1_4)
    func_ptr = glXGetProcAddress((const GLubyte *)name);
#else
    func_ptr = glXGetProcAddressARB((const GLubyte *)name);
#endif

	if (func_ptr == NULL && raise == 1)
	    rb_raise(rb_eNotImpError,"Function %s is not available at this machine",name);

	return func_ptr;
}

#endif /* _RBOGL_H_ */
