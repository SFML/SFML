/*
 * Copyright (C) 1999 - 2005 Yoshi <yoshi@giganet.net>
 * Copyright (C) 2006 John M. Gabriele <jmg3000@gmail.com>
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

/* OpenGL 1.2 functions */

static void (APIENTRY * fptr_glBlendColor)(GLclampf,GLclampf,GLclampf,GLclampf);
static VALUE
gl_BlendColor(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
	LOAD_GL_FUNC(glBlendColor)
	red = (GLclampf)NUM2DBL(arg1);
	green = (GLclampf)NUM2DBL(arg2);
	blue = (GLclampf)NUM2DBL(arg3);
	alpha = (GLclampf)NUM2DBL(arg4);
	fptr_glBlendColor(red,green,blue,alpha);
	return Qnil;
}

static void (APIENTRY * fptr_glBlendEquation)(GLenum);
static VALUE
gl_BlendEquation(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	LOAD_GL_FUNC(glBlendEquation)
	mode = (GLenum)NUM2INT(arg1);
	fptr_glBlendEquation(mode);
	return Qnil;
}

static void (APIENTRY * fptr_glDrawRangeElements)(GLenum,GLuint,GLuint,GLsizei,GLenum,GLvoid*);
static VALUE
gl_DrawRangeElements(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum mode;
	GLuint start;
	GLuint end;
	GLsizei count;
	GLenum type;
	LOAD_GL_FUNC(glDrawRangeElements)
	mode = (GLenum)NUM2INT(arg1);
	start = (GLuint)NUM2UINT(arg2);
	end = (GLuint)NUM2UINT(arg3);
	count = (GLsizei)NUM2UINT(arg4);
	type = (GLenum)NUM2INT(arg5);
	Check_Type(arg6, T_STRING);
	fptr_glDrawRangeElements(mode, start, end, count, type, RSTRING(arg6)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glColorTable)(GLenum,GLenum,GLsizei,GLenum,GLenum,GLvoid*);
static VALUE
gl_ColorTable(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLenum internalformat;
	GLsizei width;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLsizei size;
	LOAD_GL_FUNC(glColorTable)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	format = (GLenum)NUM2INT(arg4);
	type = (GLenum)NUM2INT(arg5);
	Check_Type(arg6,T_STRING);
	format_size = glformat_size(format);	
	type_size = gltype_size(type);	
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	size = format_size*type_size*width;
	if (RSTRING(arg6)->len < size)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg6)->len);
	fptr_glColorTable(target,internalformat,width,format,type,RSTRING(arg6)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glColorTableParameterfv)(GLenum,GLenum,const GLfloat*);
static VALUE
gl_ColorTableParameterfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	LOAD_GL_FUNC(glColorTableParameterfv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	fptr_glColorTableParameterfv(target,pname,params);
	return Qnil;
}

static void (APIENTRY * fptr_glColorTableParameteriv)(GLenum,GLenum,const GLint*);
static VALUE
gl_ColorTableParameteriv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	LOAD_GL_FUNC(glColorTableParameteriv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	fptr_glColorTableParameteriv(target,pname,params);
	return Qnil;
}

static void (APIENTRY * fptr_glCopyColorTable)(GLenum,GLenum,GLint,GLint,GLsizei);
static VALUE
gl_CopyColorTable(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLenum target;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	LOAD_GL_FUNC(glCopyColorTable)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	x = (GLenum)NUM2INT(arg3);
	y = (GLenum)NUM2INT(arg4);
	width = (GLenum)NUM2INT(arg5);
	fptr_glCopyColorTable(target,internalformat,x,y,width);
	return Qnil;
}

static void (APIENTRY * fptr_glGetColorTableParameterfv)(GLenum,GLenum,GLfloat *);
static VALUE
gl_GetColorTableParameterfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	GLsizei size;
	VALUE retary;
	int i;
	LOAD_GL_FUNC(glGetColorTableParameterfv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch (pname) {
		case GL_COLOR_TABLE_SCALE:
		case GL_COLOR_TABLE_BIAS:
			size = 4;
			break;
		default:
			size = 1;
			break;
	}
	fptr_glGetColorTableParameterfv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static void (APIENTRY * fptr_glGetColorTableParameteriv)(GLenum,GLenum,GLint *);
static VALUE
gl_GetColorTableParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	GLsizei size;
	VALUE retary;
	int i;
	LOAD_GL_FUNC(glGetColorTableParameteriv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch (pname) {
		case GL_COLOR_TABLE_SCALE:
		case GL_COLOR_TABLE_BIAS:
			size = 4;
			break;
		default:
			size = 1;
			break;
	}
	fptr_glGetColorTableParameteriv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}

static void (APIENTRY * fptr_glGetColorTable)(GLenum,GLenum,GLenum,GLvoid *);
static VALUE
gl_GetColorTable(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLsizei size;
	GLsizei width = 0;
	VALUE data;
	LOAD_GL_FUNC(glGetColorTable)
	LOAD_GL_FUNC(glGetColorTableParameteriv)
	target = (GLenum)NUM2INT(arg1);
	format = (GLenum)NUM2INT(arg2);
	type = (GLenum)NUM2INT(arg3);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	fptr_glGetColorTableParameteriv(target,GL_COLOR_TABLE_WIDTH,&width);
	if (type==GL_BITMAP)
		size = format_size*(width/8);
	else
		size = type_size*format_size*width;
	data = allocate_buffer_with_string(size);
	FORCE_PIXEL_STORE_MODE
	fptr_glGetColorTable(target,format,type,(GLvoid*)RSTRING(data)->ptr);	
	RESTORE_PIXEL_STORE_MODE
	return data;
}

static void (APIENTRY * fptr_glColorSubTable)(GLenum,GLsizei,GLsizei,GLenum,GLenum,const GLvoid *data);
static VALUE
gl_ColorSubTable(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLsizei start;
	GLsizei count;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLsizei size;
	LOAD_GL_FUNC(glColorSubTable)
	target = (GLenum)NUM2INT(arg1);	
	start = (GLsizei)NUM2UINT(arg2);	
	count = (GLsizei)NUM2UINT(arg3);	
	format = (GLenum)NUM2INT(arg4);	
	type = (GLenum)NUM2INT(arg5);	
	Check_Type(arg6,T_STRING);
	format_size = glformat_size(format);	
	type_size = gltype_size(type);	
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	size = format_size*type_size*count;
	if (RSTRING(arg6)->len < size)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg6)->len);
	fptr_glColorSubTable(target,start,count,format,type,RSTRING(arg6)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glCopyColorSubTable)(GLenum,GLsizei,GLint,GLint,GLsizei);
static VALUE
gl_CopyColorSubTable(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLenum target;
	GLsizei start;
	GLint x;
	GLint y;
	GLsizei width;
	LOAD_GL_FUNC(glCopyColorSubTable)
	target = (GLenum)NUM2INT(arg1);
	start = (GLsizei)NUM2UINT(arg2);
	x = (GLint)NUM2INT(arg3);
	y = (GLint)NUM2INT(arg4);
	width = (GLsizei)NUM2UINT(arg5);
	fptr_glCopyColorSubTable(target,start,x,y,width);
	return Qnil;
}

static void (APIENTRY * fptr_glConvolutionFilter1D)(GLenum,GLenum,GLsizei,GLenum,GLenum,const GLvoid *);
static VALUE
gl_ConvolutionFilter1D(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLenum internalformat;
	GLsizei width;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLsizei size;
	LOAD_GL_FUNC(glConvolutionFilter1D)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	format = (GLenum)NUM2INT(arg4);
	type = (GLenum)NUM2INT(arg5);
	Check_Type(arg6,T_STRING);
	format_size = glformat_size(format);	
	type_size = gltype_size(type);	
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	if (type==GL_BITMAP)
		size = format_size*(width/8);
	else
		size = type_size*format_size*width;
	if (RSTRING(arg6)->len < size)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg6)->len);
	fptr_glConvolutionFilter1D(target,internalformat,width,format,type,RSTRING(arg6)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glConvolutionFilter2D)(GLenum,GLenum,GLsizei,GLsizei,GLenum,GLenum,const GLvoid *);
static VALUE
gl_ConvolutionFilter2D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7;
{
	GLenum target;
	GLenum internalformat;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLsizei size;
	LOAD_GL_FUNC(glConvolutionFilter2D)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	height = (GLsizei)NUM2UINT(arg4);
	format = (GLenum)NUM2INT(arg5);
	type = (GLenum)NUM2INT(arg6);
	Check_Type(arg7,T_STRING);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	if (type==GL_BITMAP)
		size = format_size*((width*height)/8);
	else
		size = type_size*format_size*width*height;
	if (RSTRING(arg7)->len < size)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg7)->len);
	fptr_glConvolutionFilter2D(target,internalformat,width,height,format,type,RSTRING(arg7)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glConvolutionParameterf)(GLenum,GLenum,GLfloat);
static VALUE
gl_ConvolutionParameterf(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat params;
	LOAD_GL_FUNC(glConvolutionParameterf)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	params = (GLfloat)NUM2INT(arg3);
	fptr_glConvolutionParameterf(target,pname,params);
	return Qnil;
}

static void (APIENTRY * fptr_glConvolutionParameterfv)(GLenum,GLenum,const GLfloat *);
static VALUE
gl_ConvolutionParameterfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	LOAD_GL_FUNC(glConvolutionParameterfv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	fptr_glConvolutionParameterfv(target,pname,params);
	return Qnil;
}

static void (APIENTRY * fptr_glConvolutionParameteri)(GLenum,GLenum,GLint);
static VALUE
gl_ConvolutionParameteri(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint params;
	LOAD_GL_FUNC(glConvolutionParameteri)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	params = (GLint)NUM2INT(arg3);
	fptr_glConvolutionParameteri(target,pname,params);
	return Qnil;
}

static void (APIENTRY * fptr_glConvolutionParameteriv)(GLenum,GLenum,const GLint *);
static VALUE
gl_ConvolutionParameteriv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	LOAD_GL_FUNC(glConvolutionParameteriv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	fptr_glConvolutionParameteriv(target,pname,params);
	return Qnil;
}

static void (APIENTRY * fptr_glCopyConvolutionFilter1D)(GLenum,GLenum,GLint,GLint,GLsizei);
static VALUE
gl_CopyConvolutionFilter1D(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLenum target;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	LOAD_GL_FUNC(glCopyConvolutionFilter1D)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	x = (GLint)NUM2INT(arg3);
	y = (GLint)NUM2INT(arg4);
	width = (GLsizei)NUM2UINT(arg5);
	fptr_glCopyConvolutionFilter1D(target,internalformat,x,y,width);
	return Qnil;
}

static void (APIENTRY * fptr_glCopyConvolutionFilter2D)(GLenum,GLenum,GLint,GLint,GLsizei,GLsizei);
static VALUE
gl_CopyConvolutionFilter2D(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	LOAD_GL_FUNC(glCopyConvolutionFilter2D)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	x = (GLint)NUM2INT(arg3);
	y = (GLint)NUM2INT(arg4);
	width = (GLsizei)NUM2UINT(arg5);
	height = (GLsizei)NUM2UINT(arg6);
	fptr_glCopyConvolutionFilter2D(target,internalformat,x,y,width,height);
	return Qnil;
}

static void (APIENTRY * fptr_glGetConvolutionParameterfv)(GLenum,GLenum,GLfloat *);
static VALUE
gl_GetConvolutionParameterfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	GLsizei size;
	int i;
	VALUE retary;	
	LOAD_GL_FUNC(glGetConvolutionParameterfv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_CONVOLUTION_BORDER_COLOR:
		case GL_CONVOLUTION_FILTER_SCALE:
		case GL_CONVOLUTION_FILTER_BIAS:
			size = 4;
			break;
		default:
			size = 1;
			break;
	}
	fptr_glGetConvolutionParameterfv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static void (APIENTRY * fptr_glGetConvolutionParameteriv)(GLenum,GLenum,GLint *);
static VALUE
gl_GetConvolutionParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	GLsizei size;
	int i;
	VALUE retary;	
	LOAD_GL_FUNC(glGetConvolutionParameteriv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_CONVOLUTION_BORDER_COLOR:
		case GL_CONVOLUTION_FILTER_SCALE:
		case GL_CONVOLUTION_FILTER_BIAS:
			size = 4;
			break;
		default:
			size = 1;
			break;
	}
	fptr_glGetConvolutionParameteriv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}

static void (APIENTRY * fptr_glGetConvolutionFilter)(GLenum,GLenum,GLenum,GLvoid *);
static VALUE
gl_GetConvolutionFilter(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLint size = 0;
	VALUE data;
	LOAD_GL_FUNC(glGetConvolutionFilter)
	LOAD_GL_FUNC(glGetConvolutionParameteriv)
	target = (GLenum)NUM2INT(arg1);
	format = (GLenum)NUM2INT(arg2);
	type = (GLenum)NUM2INT(arg3);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	if (target==GL_CONVOLUTION_1D) {
		fptr_glGetConvolutionParameteriv(target,GL_CONVOLUTION_WIDTH,&size);
	} else {
		GLint tmp = 0;
		fptr_glGetConvolutionParameteriv(target,GL_CONVOLUTION_WIDTH,&tmp);
		fptr_glGetConvolutionParameteriv(target,GL_CONVOLUTION_HEIGHT,&size);
		size *=tmp;
	}
	if (type==GL_BITMAP)
		size = (size/8)*format_size;
	else
		size = size*type_size*format_size;
	data = allocate_buffer_with_string(size);
	FORCE_PIXEL_STORE_MODE
	fptr_glGetConvolutionFilter(target,format,type,(GLvoid*)RSTRING(data)->ptr);
	RESTORE_PIXEL_STORE_MODE
	return data;
}

static void (APIENTRY * fptr_glGetSeparableFilter)(GLenum,GLenum,GLenum,GLvoid*,GLvoid*,GLvoid*);
static VALUE
gl_GetSeparableFilter(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLint size_row = 0;
	GLint size_column = 0;
	VALUE data_row;
	VALUE data_column;
	VALUE retary;
	LOAD_GL_FUNC(glGetSeparableFilter)
	LOAD_GL_FUNC(glGetConvolutionParameteriv)
	target = (GLenum)NUM2INT(arg1);
	format = (GLenum)NUM2INT(arg2);
	type = (GLenum)NUM2INT(arg3);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	fptr_glGetConvolutionParameteriv(target,GL_CONVOLUTION_WIDTH,&size_row);
	fptr_glGetConvolutionParameteriv(target,GL_CONVOLUTION_HEIGHT,&size_column);
	if (type==GL_BITMAP) {
		size_row = (size_row/8)*format_size;
		size_column = (size_column/8)*format_size;
	} else {
		size_row *= type_size*format_size;
		size_column *= type_size*format_size;
	}
	data_row = allocate_buffer_with_string(size_row);
	data_column = allocate_buffer_with_string(size_column);
	FORCE_PIXEL_STORE_MODE
	fptr_glGetSeparableFilter(target,format,type,(GLvoid*)RSTRING(data_row)->ptr,(GLvoid*)RSTRING(data_column)->ptr,0);
	RESTORE_PIXEL_STORE_MODE
	retary = rb_ary_new2(2);
	rb_ary_push(retary, data_row);
	rb_ary_push(retary, data_column);
	return retary;
}

static void (APIENTRY * fptr_glSeparableFilter2D)(GLenum,GLenum,GLsizei,GLsizei,GLenum,GLenum,const GLvoid *,const GLvoid *);
static VALUE
gl_SeparableFilter2D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8;
{
	GLenum target;
	GLenum internalformat;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLsizei size_row;
	GLsizei size_column;
	LOAD_GL_FUNC(glSeparableFilter2D)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	height = (GLsizei)NUM2UINT(arg4);
	format = (GLenum)NUM2INT(arg5);
	type = (GLenum)NUM2INT(arg6);
	Check_Type(arg7,T_STRING);
	Check_Type(arg8,T_STRING);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	if (type==GL_BITMAP) {
		size_row = format_size*(width/8);
		size_column = format_size*(height/8);
	} else {
		size_row = type_size*format_size*width;		
		size_column = type_size*format_size*height;		
	}
	if (RSTRING(arg7)->len < size_row)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg7)->len);
	if (RSTRING(arg8)->len < size_column)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg8)->len);

	fptr_glSeparableFilter2D(target,internalformat,width,height,format,type,RSTRING(arg7)->ptr,RSTRING(arg8)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glGetHistogramParameterfv)(GLenum,GLenum,GLfloat *);
static VALUE
gl_GetHistogramParameterfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat params = 0.0;
	VALUE retary;	
	LOAD_GL_FUNC(glGetHistogramParameterfv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetHistogramParameterfv(target,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, rb_float_new(params));
	return retary;
}

static void (APIENTRY * fptr_glGetHistogramParameteriv)(GLenum,GLenum,GLint *);
static VALUE
gl_GetHistogramParameteriv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint params = 0;
	VALUE retary;	
	LOAD_GL_FUNC(glGetHistogramParameteriv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetHistogramParameteriv(target,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, INT2NUM(params));
	return retary;
}

static void (APIENTRY * fptr_glGetHistogram)(GLenum,GLboolean,GLenum,GLenum,GLvoid*);
static VALUE
gl_GetHistogram(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLboolean reset;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLint size = 0;
	VALUE data;
	LOAD_GL_FUNC(glGetHistogram)
	LOAD_GL_FUNC(glGetHistogramParameteriv)
	target = (GLenum)NUM2INT(arg1);
	reset = (GLboolean)NUM2INT(arg2);
	format = (GLenum)NUM2INT(arg3);
	type = (GLenum)NUM2INT(arg4);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	fptr_glGetHistogramParameteriv(target,GL_HISTOGRAM_WIDTH,&size);
	if (type==GL_BITMAP)
		size = (size/8)*format_size;
	else
		size = size*type_size*format_size;
	data = allocate_buffer_with_string(size);
	FORCE_PIXEL_STORE_MODE
	fptr_glGetHistogram(target,reset,format,type,(GLvoid*)RSTRING(data)->ptr);
	RESTORE_PIXEL_STORE_MODE
	return data;
}

static void (APIENTRY * fptr_glGetMinmax)(GLenum,GLboolean,GLenum,GLenum,GLvoid *);
static VALUE
gl_GetMinmax(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLboolean reset;
	GLenum format;
	GLenum type;
	GLsizei format_size;
	GLsizei type_size;
	GLint size;
	VALUE data;
	LOAD_GL_FUNC(glGetMinmax)
	target = (GLenum)NUM2INT(arg1);
	reset = (GLboolean)NUM2INT(arg2);
	format = (GLenum)NUM2INT(arg3);
	type = (GLenum)NUM2INT(arg4);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eTypeError, "type/format mismatch");
	if (type==GL_BITMAP)
		size = format_size*(2/8);
	else
		size = type_size*format_size*2;
	data = allocate_buffer_with_string(size);
	FORCE_PIXEL_STORE_MODE
	fptr_glGetMinmax(target,reset,format,type,(GLvoid*)RSTRING(data)->ptr);
	RESTORE_PIXEL_STORE_MODE
	return data;
}

static void (APIENTRY * fptr_glGetMinmaxParameterfv)(GLenum,GLenum,GLfloat *);
static VALUE
gl_GetMinmaxParameterfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLfloat params = 0.0;
	VALUE retary;	
	LOAD_GL_FUNC(glGetMinmaxParameterfv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetMinmaxParameterfv(target,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, rb_float_new(params));
	return retary;
}

static void (APIENTRY * fptr_glGetMinmaxParameteriv)(GLenum,GLenum,GLint *);
static VALUE
gl_GetMinmaxParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params = 0;
	VALUE retary;	
	LOAD_GL_FUNC(glGetMinmaxParameteriv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetMinmaxParameteriv(target,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, INT2NUM(params));
	return retary;
}

static void (APIENTRY * fptr_glHistogram)(GLenum,GLsizei,GLenum,GLboolean);
static VALUE
gl_Histogram(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLsizei width;
	GLenum internalformat;
	GLboolean sink;
	LOAD_GL_FUNC(glHistogram)
	target = (GLenum)NUM2INT(arg1);
	width = (GLsizei)NUM2UINT(arg2);
	internalformat = (GLenum)NUM2INT(arg3);
	sink = (GLboolean)NUM2INT(arg4);
	fptr_glHistogram(target,width,internalformat,sink);
	return Qnil;
}

static void (APIENTRY * fptr_glMinmax)(GLenum,GLenum,GLboolean);
static VALUE
gl_Minmax(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum internalformat;
	GLboolean sink;
	LOAD_GL_FUNC(glMinmax)
	target = (GLenum)NUM2INT(arg1);
	internalformat = (GLenum)NUM2INT(arg2);
	sink = (GLboolean)NUM2INT(arg3);
	fptr_glMinmax(target,internalformat,sink);
	return Qnil;
}

static void (APIENTRY * fptr_glResetHistogram)(GLenum);
static VALUE
gl_ResetHistogram(obj,arg1)
VALUE obj,arg1;
{
	GLenum target;
	LOAD_GL_FUNC(glResetHistogram)
	target = (GLenum)NUM2INT(arg1);
	fptr_glResetHistogram(target);
	return Qnil;
}

static void (APIENTRY * fptr_glResetMinmax)(GLenum);
static VALUE
gl_ResetMinmax(obj,arg1)
VALUE obj,arg1;
{
	GLenum target;
	LOAD_GL_FUNC(glResetMinmax)
	target = (GLenum)NUM2INT(arg1);
	fptr_glResetMinmax(target);
	return Qnil;
}

static void (APIENTRY * fptr_glTexImage3D)(GLenum,GLint,GLint,GLsizei,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*);
static VALUE
gl_TexImage3D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	GLenum target;
	GLint level;
	GLint internalFormat;
	GLsizei width;
	GLsizei height;
	GLsizei depth;
	GLint border;
	GLenum format;
	GLenum type;
	const char *pixels;
	int size;
	int type_size;
	int format_size;
	LOAD_GL_FUNC(glTexImage3D)
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	internalFormat = (GLint)NUM2INT(arg3);
	width = (GLsizei)NUM2UINT(arg4);
	height = (GLsizei)NUM2UINT(arg5);
	depth = (GLsizei)NUM2UINT(arg6);
	border = (GLint)NUM2INT(arg7);
	format = (GLenum)NUM2INT(arg8);
	type = (GLenum)NUM2INT(arg9);
	Check_Type(arg10, T_STRING);
	type_size = gltype_size(type);
	format_size = glformat_size(format);
	if (type_size == -1 || format_size == -1)
		return Qnil;
	if (type==GL_BITMAP)
		size = format_size*((height*width*depth)/8);
	else	
		size = type_size*format_size*height*width*depth;

	if (target == GL_PROXY_TEXTURE_3D || NIL_P(arg10)) { /* proxy texture, no data read */
		pixels = NULL;
	} else {
		if (TYPE(arg10) == T_FIXNUM || TYPE(arg10) == T_BIGNUM) { /* arg10 is offset to unpack buffer */
			pixels = (const char *)NUM2UINT(arg10);
		} else if (TYPE(arg10) == T_STRING) { /* image data */
			if (RSTRING(arg10)->len < size)
				rb_raise(rb_eArgError, "string length:%d",RSTRING(arg10)->len);
			pixels = RSTRING(arg10)->ptr;
		} else {
			Check_Type(arg10,T_STRING); /* force exception */
			return Qnil;
		}
	}
	fptr_glTexImage3D( target, level, internalFormat, width, height,
				  depth, border, format, type,pixels);
	return Qnil;
}

static void (APIENTRY * fptr_glTexSubImage3D)(GLenum,GLint,GLint,GLint,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*);
static VALUE
gl_TexSubImage3D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11)
VALUE arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11;
{
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLint zoffset;
	GLsizei width;
	GLsizei height;
	GLsizei depth;
	GLenum format;
	GLenum type;
	const char *pixels;
	int size;
	int type_size;
	int format_size;
	LOAD_GL_FUNC(glTexSubImage3D)
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	xoffset = (GLint)NUM2INT(arg3);
	yoffset = (GLint)NUM2INT(arg4);
	zoffset = (GLint)NUM2INT(arg5);
	width = (GLsizei)NUM2UINT(arg6);
	height = (GLsizei)NUM2UINT(arg7);
	depth = (GLsizei)NUM2UINT(arg8);
	format = (GLenum)NUM2INT(arg9);
	type = (GLenum)NUM2INT(arg10);
	Check_Type(arg11, T_STRING);
	type_size = gltype_size(type);
	format_size = glformat_size(format);
	if (type_size == -1 || format_size == -1)
		return Qnil;
	if (type==GL_BITMAP)
		size = format_size*((height*width*depth)/8);
	else	
		size = type_size*format_size*height*width*depth;

	if (TYPE(arg11) == T_STRING) {
		if (RSTRING(arg11)->len < size)
			rb_raise(rb_eArgError, "string length:%d",RSTRING(arg11)->len);
		 pixels = RSTRING(arg11)->ptr;
	} else if (TYPE(arg11) == T_FIXNUM || TYPE(arg11) == T_BIGNUM) { /* arg11 is offset to unpack buffer */
			pixels = (const char *)NUM2UINT(arg11);
	} else {
		Check_Type(arg11,T_STRING); /* force exception */
		return Qnil;
	}

	fptr_glTexSubImage3D( target, level, xoffset, yoffset, zoffset,
			width, height, depth,
			format, type, pixels);
	return Qnil;
}

static void (APIENTRY * fptr_glCopyTexSubImage3D)(GLenum,GLint,GLint,GLint,GLint,GLint,GLint,GLsizei,GLsizei);
static VALUE
gl_CopyTexSubImage3D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9;
{
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLint zoffset;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	LOAD_GL_FUNC(glCopyTexSubImage3D)
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	xoffset = (GLint)NUM2INT(arg3);
	yoffset = (GLint)NUM2INT(arg4);
	zoffset = (GLint)NUM2INT(arg5);
	x = (GLint)NUM2INT(arg6);
	y = (GLint)NUM2INT(arg7);
	width = (GLsizei)NUM2UINT(arg8);
	height = (GLsizei)NUM2UINT(arg9);
	fptr_glCopyTexSubImage3D( target, level,
			xoffset, yoffset, zoffset,
			x, y, width, height );
	return Qnil;
}

void gl_init_functions_1_2(VALUE module)
{
	rb_define_module_function(module, "glBlendColor", gl_BlendColor, 4);
	rb_define_module_function(module, "glBlendEquation", gl_BlendEquation, 1);
	rb_define_module_function(module, "glDrawRangeElements", gl_DrawRangeElements, 6);
	rb_define_module_function(module, "glColorTable", gl_ColorTable, 6);
	rb_define_module_function(module, "glColorTableParameterfv", gl_ColorTableParameterfv, 3);
	rb_define_module_function(module, "glColorTableParameteriv", gl_ColorTableParameteriv, 3);
	rb_define_module_function(module, "glCopyColorTable", gl_CopyColorTable, 5);
	rb_define_module_function(module, "glGetColorTable", gl_GetColorTable, 3);
	rb_define_module_function(module, "glGetColorTableParameterfv", gl_GetColorTableParameterfv, 2);
	rb_define_module_function(module, "glGetColorTableParameteriv", gl_GetColorTableParameteriv, 2);
	rb_define_module_function(module, "glColorSubTable", gl_ColorSubTable, 6);
	rb_define_module_function(module, "glCopyColorSubTable", gl_CopyColorSubTable, 5);
	rb_define_module_function(module, "glConvolutionFilter1D", gl_ConvolutionFilter1D, 6);
	rb_define_module_function(module, "glConvolutionFilter2D", gl_ConvolutionFilter2D, 7);
	rb_define_module_function(module, "glConvolutionParameterf", gl_ConvolutionParameterf, 3);
	rb_define_module_function(module, "glConvolutionParameterfv", gl_ConvolutionParameterfv, 3);
	rb_define_module_function(module, "glConvolutionParameteri", gl_ConvolutionParameteri, 3);
	rb_define_module_function(module, "glConvolutionParameteriv", gl_ConvolutionParameteriv, 3);
	rb_define_module_function(module, "glCopyConvolutionFilter1D", gl_CopyConvolutionFilter1D, 5);
	rb_define_module_function(module, "glCopyConvolutionFilter2D", gl_CopyConvolutionFilter2D, 6);
	rb_define_module_function(module, "glGetConvolutionFilter", gl_GetConvolutionFilter, 3);
	rb_define_module_function(module, "glGetConvolutionParameterfv", gl_GetConvolutionParameterfv, 2);
	rb_define_module_function(module, "glGetConvolutionParameteriv", gl_GetConvolutionParameteriv, 2);
	rb_define_module_function(module, "glGetSeparableFilter", gl_GetSeparableFilter, 3);
	rb_define_module_function(module, "glSeparableFilter2D", gl_SeparableFilter2D, 8);
	rb_define_module_function(module, "glGetHistogram", gl_GetHistogram, 4);
	rb_define_module_function(module, "glGetHistogramParameterfv", gl_GetHistogramParameterfv, 2);
	rb_define_module_function(module, "glGetHistogramParameteriv", gl_GetHistogramParameteriv, 2);
	rb_define_module_function(module, "glGetMinmax", gl_GetMinmax, 4);
	rb_define_module_function(module, "glGetMinmaxParameterfv", gl_GetMinmaxParameterfv, 2);
	rb_define_module_function(module, "glGetMinmaxParameteriv", gl_GetMinmaxParameteriv, 2);
	rb_define_module_function(module, "glHistogram", gl_Histogram, 4);
	rb_define_module_function(module, "glMinmax", gl_Minmax, 3);
	rb_define_module_function(module, "glResetHistogram", gl_ResetHistogram, 1);
	rb_define_module_function(module, "glResetMinmax", gl_ResetMinmax, 1);
	rb_define_module_function(module, "glTexImage3D", gl_TexImage3D, 10);
	rb_define_module_function(module, "glTexSubImage3D", gl_TexSubImage3D, 11);
	rb_define_module_function(module, "glCopyTexSubImage3D", gl_CopyTexSubImage3D, 9);
}
