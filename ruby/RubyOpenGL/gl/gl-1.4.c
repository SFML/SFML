/*
 * Copyright (C) 2007 Jan Dvorak <jan.dvorak@kraxnet.cz>
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

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif defined WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "../common/common.h"

/* OpenGL 1.4 functions */

static void (APIENTRY * fptr_glBlendFuncSeparate)(GLenum,GLenum,GLenum,GLenum);
static VALUE
gl_BlendFuncSeparate(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum srcRGB;
	GLenum dstRGB;
	GLenum srcAlpha;
	GLenum dstAlpha;
	LOAD_GL_FUNC(glBlendFuncSeparate)
	srcRGB = (GLenum)NUM2INT(arg1);
	dstRGB = (GLenum)NUM2INT(arg2);
	srcAlpha = (GLenum)NUM2INT(arg3);
	dstAlpha = (GLenum)NUM2INT(arg4);
	fptr_glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);
	return Qnil;
}

static void (APIENTRY * fptr_glFogCoordf)(GLfloat);
static VALUE
gl_FogCoordf(obj,arg1)
VALUE obj,arg1;
{
	GLfloat coord;
	LOAD_GL_FUNC(glFogCoordf)
	coord=(GLfloat)NUM2DBL(arg1);
	fptr_glFogCoordf(coord);
	return Qnil;
}

static void (APIENTRY * fptr_glFogCoordfv)(GLfloat *);
static VALUE
gl_FogCoordfv(obj,arg1)
VALUE obj,arg1;
{
	GLfloat coord;
	LOAD_GL_FUNC(glFogCoordfv)
	Check_Type(arg1,T_ARRAY);
	ary2cflt(arg1,&coord,1);
	fptr_glFogCoordfv(&coord);
	return Qnil;
}

static void (APIENTRY * fptr_glFogCoordd)(GLdouble);
static VALUE
gl_FogCoordd(obj,arg1)
VALUE obj,arg1;
{
	GLdouble coord;
	LOAD_GL_FUNC(glFogCoordd)
	coord=(GLdouble)NUM2DBL(arg1);
	fptr_glFogCoordd(coord);
	return Qnil;
}

static void (APIENTRY * fptr_glFogCoorddv)(GLdouble *);
static VALUE
gl_FogCoorddv(obj,arg1)
VALUE obj,arg1;
{
	GLdouble coord;
	LOAD_GL_FUNC(glFogCoorddv)
	Check_Type(arg1,T_ARRAY);
	ary2cdbl(arg1,&coord,1);
	fptr_glFogCoorddv(&coord);
	return Qnil;
}

extern VALUE g_FogCoord_ptr;
static void (APIENTRY * fptr_glFogCoordPointer)(GLenum,GLsizei,const GLvoid *);
static VALUE
gl_FogCoordPointer(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum type;
	GLsizei stride;
	LOAD_GL_FUNC(glFogCoordPointer)
	type = (GLenum)NUM2INT(arg1);
	stride = (GLsizei)NUM2UINT(arg2);
	Check_Type(arg3, T_STRING);
	rb_str_freeze(arg3);
	g_FogCoord_ptr = arg3;
	fptr_glFogCoordPointer(type, stride, (const GLvoid*)RSTRING(arg3)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glMultiDrawArrays)(GLenum,GLint*,GLsizei*,GLsizei);
static VALUE
gl_MultiDrawArrays(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	/* TODO: check ary1,ary2 if len < primcount then raise */
	/* stringy ? */
	GLenum mode;
	GLsizei primcount;
	GLint *ary1;
	GLsizei *ary2;
	LOAD_GL_FUNC(glMultiDrawArrays)
	mode = (GLenum)NUM2INT(arg1);
	primcount = (GLsizei)NUM2UINT(arg4);
	ary1 = ALLOC_N(GLint,primcount);
	ary2 = ALLOC_N(GLsizei,primcount);
	ary2cint(arg2,ary1,primcount);
	ary2cint(arg3,ary2,primcount);
	fptr_glMultiDrawArrays(mode,ary1,ary2,primcount);
	xfree(ary1);
	xfree(ary2);
	return Qnil;
}

static void (APIENTRY * fptr_glMultiDrawElements)(GLenum,const GLsizei *,GLenum,GLvoid **,GLsizei);
static VALUE
gl_MultiDrawElements(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum mode;
	GLenum type;
	GLsizei *counts;
	GLvoid **indices;
	GLint size;
	RArray *ary;
	int i;
	LOAD_GL_FUNC(glMultiDrawElements)
	mode = (GLenum)NUM2INT(arg1);
	type = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary = RARRAY(arg3);
	size = ary->len;
	counts = ALLOC_N(GLsizei,size);
	indices = ALLOC_N(GLvoid*,size);
	for (i=0;i<size;i++) {
		indices[i] = RSTRING(ary->ptr[i])->ptr;
		counts[i] = RSTRING(ary->ptr[i])->len;
	}
	fptr_glMultiDrawElements(mode,counts,type,indices,size);
	xfree(counts);
	xfree(indices);
	return Qnil;
}

static void (APIENTRY * fptr_glPointParameterf)(GLenum,GLfloat);
static VALUE
gl_PointParameterf(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat param;
	LOAD_GL_FUNC(glPointParameterf)
	pname = (GLenum)NUM2INT(arg1);
	param = (GLfloat)NUM2INT(arg2);
	fptr_glPointParameterf(pname,param);
	return Qnil;
}

static void (APIENTRY * fptr_glPointParameterfv)(GLenum,GLfloat *);
static VALUE
gl_PointParameterfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat params[3] = {0.0,0.0,0.0};
	GLint size;
	LOAD_GL_FUNC(glPointParameterfv)
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	if (pname==GL_POINT_DISTANCE_ATTENUATION)
		size = 3;
	else
		size = 1;
	ary2cflt(arg2,params,size);
	fptr_glPointParameterfv(pname,params);	
	return Qnil;
}

static void (APIENTRY * fptr_glPointParameteri)(GLenum,GLint);
static VALUE
gl_PointParameteri(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint param;
	LOAD_GL_FUNC(glPointParameteri)
	pname = (GLenum)NUM2INT(arg1);
	param = (GLint)NUM2INT(arg2);
	fptr_glPointParameteri(pname,param);
	return Qnil;
}

static void (APIENTRY * fptr_glPointParameteriv)(GLenum,GLint *);
static VALUE
gl_PointParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint params[3] = {0.0,0.0,0.0};
	GLint size;
	LOAD_GL_FUNC(glPointParameteriv)
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	if (pname==GL_POINT_DISTANCE_ATTENUATION)
		size = 3;
	else
		size = 1;
	ary2cint(arg2,params,size);
	fptr_glPointParameteriv(pname,params);	
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3b)(GLbyte,GLbyte,GLbyte);
static VALUE
gl_SecondaryColor3b(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLbyte red;
	GLbyte green;
	GLbyte blue;
	LOAD_GL_FUNC(glSecondaryColor3b)
	red = (GLbyte)NUM2INT(arg1);
	green = (GLbyte)NUM2INT(arg2);
	blue = (GLbyte)NUM2INT(arg3);
	fptr_glSecondaryColor3b(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3d)(GLdouble,GLdouble,GLdouble);
static VALUE
gl_SecondaryColor3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble red;
	GLdouble green;
	GLdouble blue;
	LOAD_GL_FUNC(glSecondaryColor3d)
	red = (GLdouble)NUM2DBL(arg1);
	green = (GLdouble)NUM2DBL(arg2);
	blue = (GLdouble)NUM2DBL(arg3);
	fptr_glSecondaryColor3d(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3f)(GLfloat,GLfloat,GLfloat);
static VALUE
gl_SecondaryColor3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	LOAD_GL_FUNC(glSecondaryColor3f)
	red = (GLfloat)NUM2DBL(arg1);
	green = (GLfloat)NUM2DBL(arg2);
	blue = (GLfloat)NUM2DBL(arg3);
	fptr_glSecondaryColor3f(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3i)(GLint,GLint,GLint);
static VALUE
gl_SecondaryColor3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint red;
	GLint green;
	GLint blue;
	LOAD_GL_FUNC(glSecondaryColor3i)
	red = (GLint)NUM2INT(arg1);
	green = (GLint)NUM2INT(arg2);
	blue = (GLint)NUM2INT(arg3);
	fptr_glSecondaryColor3i(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3s)(GLshort,GLshort,GLshort);
static VALUE
gl_SecondaryColor3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort red;
	GLshort green;
	GLshort blue;
	LOAD_GL_FUNC(glSecondaryColor3s)
	red = (GLshort)NUM2INT(arg1);
	green = (GLshort)NUM2INT(arg2);
	blue = (GLshort)NUM2INT(arg3);
	fptr_glSecondaryColor3s(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3ub)(GLubyte,GLubyte,GLubyte);
static VALUE
gl_SecondaryColor3ub(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLubyte red;
	GLubyte green;
	GLubyte blue;
	LOAD_GL_FUNC(glSecondaryColor3ub)
	red = (GLubyte)NUM2INT(arg1);
	green = (GLubyte)NUM2INT(arg2);
	blue = (GLubyte)NUM2INT(arg3);
	fptr_glSecondaryColor3ub(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3ui)(GLuint,GLuint,GLuint);
static VALUE
gl_SecondaryColor3ui(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLuint red;
	GLuint green;
	GLuint blue;
	LOAD_GL_FUNC(glSecondaryColor3ui)
	red = (GLuint)NUM2UINT(arg1);
	green = (GLuint)NUM2UINT(arg2);
	blue = (GLuint)NUM2UINT(arg3);
	fptr_glSecondaryColor3ui(red,green,blue);
	return Qnil;
}

static void (APIENTRY * fptr_glSecondaryColor3us)(GLushort,GLushort,GLushort);
static VALUE
gl_SecondaryColor3us(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLushort red;
	GLushort green;
	GLushort blue;
	LOAD_GL_FUNC(glSecondaryColor3us)
	red = (GLushort)NUM2INT(arg1);
	green = (GLushort)NUM2INT(arg2);
	blue = (GLushort)NUM2INT(arg3);
	fptr_glSecondaryColor3us(red,green,blue);
	return Qnil;
}

#define GLSECONDARYCOLOR_VFUNC(_type_) \
static VALUE \
gl_SecondaryColor##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[3]; \
	RArray *ary; \
	switch (num = rb_scan_args(argc, argv, "12", &args[0], &args[1], &args[2])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 3: \
			gl_SecondaryColor3##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2]); \
			break; \
			default: \
				rb_raise(rb_eArgError, "array length:%d", num); \
		} \
		} \
		else \
			rb_raise(rb_eArgError, "array length:%d", num); \
		break; \
	case 3: \
		gl_SecondaryColor3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "too many arguments"); \
		break; \
	} \
	return Qnil; \
}

GLSECONDARYCOLOR_VFUNC(b)
GLSECONDARYCOLOR_VFUNC(d)
GLSECONDARYCOLOR_VFUNC(f)
GLSECONDARYCOLOR_VFUNC(i)
GLSECONDARYCOLOR_VFUNC(s)
GLSECONDARYCOLOR_VFUNC(ui)
GLSECONDARYCOLOR_VFUNC(ub)
GLSECONDARYCOLOR_VFUNC(us)
#undef GLSECONDARYCOLOR_VFUNC

extern VALUE g_SecondaryColor_ptr;
static void (APIENTRY * fptr_glSecondaryColorPointer)(GLint,GLenum,GLsizei,const GLvoid *);
static VALUE
gl_SecondaryColorPointer(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint size;
	GLenum type;
	GLsizei stride;
	LOAD_GL_FUNC(glSecondaryColorPointer)
	size = (GLint)NUM2INT(arg1);
	type = (GLenum)NUM2INT(arg2);
	stride = (GLsizei)NUM2UINT(arg3);
	Check_Type(arg4, T_STRING);
	rb_str_freeze(arg4);
	g_SecondaryColor_ptr = arg4;
	fptr_glSecondaryColorPointer(size,type, stride, (const GLvoid*)RSTRING(arg4)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos2d)(GLdouble,GLdouble);
static VALUE
gl_WindowPos2d(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLdouble x;
	GLdouble y;
	LOAD_GL_FUNC(glWindowPos2d)
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	fptr_glWindowPos2d(x,y);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos2f)(GLfloat,GLfloat);
static VALUE
gl_WindowPos2f(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat x;
	GLfloat y;
	LOAD_GL_FUNC(glWindowPos2f)
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	fptr_glWindowPos2f(x,y);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos2i)(GLint,GLint);
static VALUE
gl_WindowPos2i(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLint x;
	GLint y;
	LOAD_GL_FUNC(glWindowPos2i)
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	fptr_glWindowPos2i(x,y);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos2s)(GLshort,GLshort);
static VALUE
gl_WindowPos2s(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLshort x;
	GLshort y;
	LOAD_GL_FUNC(glWindowPos2s)
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	fptr_glWindowPos2s(x,y);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos3d)(GLdouble,GLdouble,GLdouble);
static VALUE
gl_WindowPos3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	LOAD_GL_FUNC(glWindowPos3d)
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	fptr_glWindowPos3d(x,y,z);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos3f)(GLfloat,GLfloat,GLfloat);
static VALUE
gl_WindowPos3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	LOAD_GL_FUNC(glWindowPos3f)
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	fptr_glWindowPos3f(x,y,z);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos3i)(GLint,GLint,GLint);
static VALUE
gl_WindowPos3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint x;
	GLint y;
	GLint z;
	LOAD_GL_FUNC(glWindowPos3i)
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	z = (GLint)NUM2INT(arg3);
	fptr_glWindowPos3i(x,y,z);
	return Qnil;
}

static void (APIENTRY * fptr_glWindowPos3s)(GLshort,GLshort,GLshort);
static VALUE
gl_WindowPos3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort x;
	GLshort y;
	GLshort z;
	LOAD_GL_FUNC(glWindowPos3s)
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	z = (GLshort)NUM2INT(arg3);
	fptr_glWindowPos3s(x,y,z);
	return Qnil;
}

#define GLWINDOWPOS_VFUNC(_type_) \
static VALUE \
gl_WindowPos##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[3]; \
	RArray *ary; \
	switch (num = rb_scan_args(argc, argv, "12", &args[0], &args[1], &args[2])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 2: \
			gl_WindowPos2##_type_(obj,ary->ptr[0],ary->ptr[1]); \
			break; \
			case 3: \
			gl_WindowPos3##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2]); \
			break; \
			default: \
				rb_raise(rb_eArgError, "array length:%d", num); \
		} \
		} \
		else \
			rb_raise(rb_eArgError, "array length:%d", num); \
		break; \
	case 2: \
		gl_WindowPos2##_type_(obj,args[0], args[1]); \
		break; \
	case 3: \
		gl_WindowPos3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "too many arguments"); \
		break; \
	} \
	return Qnil; \
}

GLWINDOWPOS_VFUNC(d)
GLWINDOWPOS_VFUNC(f)
GLWINDOWPOS_VFUNC(i)
GLWINDOWPOS_VFUNC(s)
#undef GLWINDOWPOS_VFUNC


void gl_init_functions_1_4(VALUE module)
{
	rb_define_module_function(module, "glBlendFuncSeparate", gl_BlendFuncSeparate, 4);
	rb_define_module_function(module, "glFogCoordf", gl_FogCoordf, 1);
	rb_define_module_function(module, "glFogCoordfv", gl_FogCoordfv, 1);
	rb_define_module_function(module, "glFogCoordd", gl_FogCoordd, 1);
	rb_define_module_function(module, "glFogCoorddv", gl_FogCoorddv, 1);
	rb_define_module_function(module, "glFogCoordPointer", gl_FogCoordPointer, 3);
	rb_define_module_function(module, "glMultiDrawArrays", gl_MultiDrawArrays, 4);
	rb_define_module_function(module, "glMultiDrawElements", gl_MultiDrawElements, 3);
	rb_define_module_function(module, "glPointParameterf", gl_PointParameterf, 2);
	rb_define_module_function(module, "glPointParameterfv", gl_PointParameterfv, 2);
	rb_define_module_function(module, "glPointParameteri", gl_PointParameteri, 2);
	rb_define_module_function(module, "glPointParameteriv", gl_PointParameteriv, 2);
	rb_define_module_function(module, "glSecondaryColor3b", gl_SecondaryColor3b, 3);
	rb_define_module_function(module, "glSecondaryColor3d", gl_SecondaryColor3d, 3);
	rb_define_module_function(module, "glSecondaryColor3f", gl_SecondaryColor3f, 3);
	rb_define_module_function(module, "glSecondaryColor3i", gl_SecondaryColor3i, 3);
	rb_define_module_function(module, "glSecondaryColor3s", gl_SecondaryColor3s, 3);
	rb_define_module_function(module, "glSecondaryColor3ub", gl_SecondaryColor3ub, 3);
	rb_define_module_function(module, "glSecondaryColor3ui", gl_SecondaryColor3ui, 3);
	rb_define_module_function(module, "glSecondaryColor3us", gl_SecondaryColor3us, 3);
	rb_define_module_function(module, "glSecondaryColorPointer", gl_SecondaryColorPointer, 4);
	rb_define_module_function(module, "glWindowPos2d", gl_WindowPos2d, 2);
	rb_define_module_function(module, "glWindowPos2f", gl_WindowPos2f, 2);
	rb_define_module_function(module, "glWindowPos2i", gl_WindowPos2i, 2);
	rb_define_module_function(module, "glWindowPos2s", gl_WindowPos2s, 2);
	rb_define_module_function(module, "glWindowPos3d", gl_WindowPos3d, 3);
	rb_define_module_function(module, "glWindowPos3f", gl_WindowPos3f, 3);
	rb_define_module_function(module, "glWindowPos3i", gl_WindowPos3i, 3);
	rb_define_module_function(module, "glWindowPos3s", gl_WindowPos3s, 3);

	/* Additional Functions */
	rb_define_module_function(module, "glSecondaryColor3bv", gl_SecondaryColorbv, -1);
	rb_define_module_function(module, "glSecondaryColor3dv", gl_SecondaryColordv, -1);
	rb_define_module_function(module, "glSecondaryColor3fv", gl_SecondaryColorfv, -1);
	rb_define_module_function(module, "glSecondaryColor3iv", gl_SecondaryColoriv, -1);
	rb_define_module_function(module, "glSecondaryColor3sv", gl_SecondaryColorsv, -1);
	rb_define_module_function(module, "glSecondaryColor3ubv", gl_SecondaryColorubv, -1);
	rb_define_module_function(module, "glSecondaryColor3uiv", gl_SecondaryColoruiv, -1);
	rb_define_module_function(module, "glSecondaryColor3usv", gl_SecondaryColorusv, -1);

	rb_define_module_function(module, "glWindowPos2dv", gl_WindowPosdv, -1);
	rb_define_module_function(module, "glWindowPos2fv", gl_WindowPosfv, -1);
	rb_define_module_function(module, "glWindowPos2iv", gl_WindowPosiv, -1);
	rb_define_module_function(module, "glWindowPos2sv", gl_WindowPossv, -1);
	rb_define_module_function(module, "glWindowPos3dv", gl_WindowPosdv, -1);
	rb_define_module_function(module, "glWindowPos3fv", gl_WindowPosfv, -1);
	rb_define_module_function(module, "glWindowPos3iv", gl_WindowPosiv, -1);
	rb_define_module_function(module, "glWindowPos3sv", gl_WindowPossv, -1);
}
