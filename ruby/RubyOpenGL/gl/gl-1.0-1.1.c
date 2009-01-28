/*
 * Copyright (C) 1999 - 2005 Yoshi <yoshi@giganet.net>
 * Copyright (C) 2006 John M. Gabriele <jmg3000@gmail.com>
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

#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

/* OpenGL 1.0 + 1.1 functions */

static VALUE
gl_NewList(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint list;
	GLenum mode;
	list = (GLuint)NUM2UINT(arg1);
	mode = (GLenum)NUM2INT(arg2);
	glNewList(list,mode);
	return Qnil;
}

static VALUE
gl_EndList(obj)
VALUE obj;
{
	glEndList();
	return Qnil;
}

static VALUE
gl_CallList(obj,arg1)
VALUE obj,arg1;
{
	GLuint list;
	list = (GLuint)NUM2UINT(arg1);
	glCallList(list);
	return Qnil;
}

static VALUE
gl_CallLists(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLenum type;
	GLvoid *lists;
	if (TYPE(arg1) == T_STRING) {
		type = GL_BYTE;
		lists = RSTRING(arg1)->ptr;
		n = RSTRING(arg1)->len;
	} else if (TYPE(arg1) == T_ARRAY) {
		type = GL_INT;
		lists = ALLOC_N(GLint, RARRAY(arg1)->len);
		n = ary2cint(arg1,lists,0);
	} else {
		Check_Type(arg1,T_ARRAY); /* force exception */
		return Qnil; /* not reached */
	}
	glCallLists(n, type, lists);
	if (type == GL_INT) xfree(lists);
	return Qnil;
}

static VALUE
gl_DeleteLists(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint list;
	GLsizei range;
	list = (GLuint)NUM2UINT(arg1);
	range = (GLsizei)NUM2UINT(arg2);
	glDeleteLists(list,range);
	return Qnil;
}

static VALUE
gl_GenLists(obj,arg1)
VALUE obj,arg1;
{
	GLsizei range;
	GLuint ret;
	range = (GLsizei)NUM2UINT(arg1);
	ret = glGenLists(range);
	return INT2NUM(ret);
}

static VALUE
gl_ListBase(obj,arg1)
VALUE obj,arg1;
{
	GLuint base;
	base = (GLuint)NUM2UINT(arg1);
	glListBase(base);
	return Qnil;
}

static VALUE
gl_Begin(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glBegin(mode);
	return Qnil;
}

static VALUE
gl_Bitmap(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7;
{
	GLsizei width;
	GLsizei height;
	GLfloat xorig;
	GLfloat yorig;
	GLfloat xmove;
	GLfloat ymove;
	width = (GLsizei)NUM2UINT(arg1);
	height = (GLsizei)NUM2UINT(arg2);
	xorig = (GLfloat)NUM2DBL(arg3);
	yorig = (GLfloat)NUM2DBL(arg4);
	xmove = (GLfloat)NUM2DBL(arg5);
	ymove = (GLfloat)NUM2DBL(arg6);
	if (TYPE(arg7) == T_FIXNUM || TYPE(arg7) == T_BIGNUM) { /* offset to unpack buffer */
		GLuint offset = NUM2UINT(arg7);
		glBitmap(width, height, xorig, yorig, xmove, ymove, (GLvoid *)offset);
	} else if (TYPE(arg7) == T_STRING) {
		const GLubyte *bitmap;
		if (RSTRING(arg7)->len < (width * height / 8))
			rb_raise(rb_eArgError, "string length:%d", RSTRING(arg7)->len);

		bitmap = (const GLubyte*)RSTRING(arg7)->ptr;
		glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap);
	} else {
		Check_Type(arg7,T_STRING); /* force exception */
	}
	return Qnil;
}

static VALUE
gl_Color3b(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLbyte red;
	GLbyte green;
	GLbyte blue;
	red = (GLbyte)NUM2INT(arg1);
	green = (GLbyte)NUM2INT(arg2);
	blue = (GLbyte)NUM2INT(arg3);
	glColor3b(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble red;
	GLdouble green;
	GLdouble blue;
	red = (GLdouble)NUM2DBL(arg1);
	green = (GLdouble)NUM2DBL(arg2);
	blue = (GLdouble)NUM2DBL(arg3);
	glColor3d(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	red = (GLfloat)NUM2DBL(arg1);
	green = (GLfloat)NUM2DBL(arg2);
	blue = (GLfloat)NUM2DBL(arg3);
	glColor3f(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint red;
	GLint green;
	GLint blue;
	red = (GLint)NUM2INT(arg1);
	green = (GLint)NUM2INT(arg2);
	blue = (GLint)NUM2INT(arg3);
	glColor3i(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort red;
	GLshort green;
	GLshort blue;
	red = (GLshort)NUM2INT(arg1);
	green = (GLshort)NUM2INT(arg2);
	blue = (GLshort)NUM2INT(arg3);
	glColor3s(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3ub(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLubyte red;
	GLubyte green;
	GLubyte blue;
	red = (GLubyte)NUM2INT(arg1);
	green = (GLubyte)NUM2INT(arg2);
	blue = (GLubyte)NUM2INT(arg3);
	glColor3ub(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3ui(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLuint red;
	GLuint green;
	GLuint blue;
	red = (GLuint)NUM2UINT(arg1);
	green = (GLuint)NUM2UINT(arg2);
	blue = (GLuint)NUM2UINT(arg3);
	glColor3ui(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color3us(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLushort red;
	GLushort green;
	GLushort blue;
	red = (GLushort)NUM2INT(arg1);
	green = (GLushort)NUM2INT(arg2);
	blue = (GLushort)NUM2INT(arg3);
	glColor3us(red,green,blue);
	return Qnil;
}

static VALUE
gl_Color4b(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLbyte red;
	GLbyte green;
	GLbyte blue;
	GLbyte alpha;
	red = (GLbyte)NUM2INT(arg1);
	green = (GLbyte)NUM2INT(arg2);
	blue = (GLbyte)NUM2INT(arg3);
	alpha = (GLbyte)NUM2INT(arg4);
	glColor4b(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4d(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLdouble red;
	GLdouble green;
	GLdouble blue;
	GLdouble alpha;
	red = (GLdouble)NUM2DBL(arg1);
	green = (GLdouble)NUM2DBL(arg2);
	blue = (GLdouble)NUM2DBL(arg3);
	alpha = (GLdouble)NUM2DBL(arg4);
	glColor4d(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4f(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
	red = (GLfloat)NUM2DBL(arg1);
	green = (GLfloat)NUM2DBL(arg2);
	blue = (GLfloat)NUM2DBL(arg3);
	alpha = (GLfloat)NUM2DBL(arg4);
	glColor4f(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4i(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint red;
	GLint green;
	GLint blue;
	GLint alpha;
	red = (GLint)NUM2INT(arg1);
	green = (GLint)NUM2INT(arg2);
	blue = (GLint)NUM2INT(arg3);
	alpha = (GLint)NUM2INT(arg4);
	glColor4i(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4s(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLshort red;
	GLshort green;
	GLshort blue;
	GLshort alpha;
	red = (GLshort)NUM2INT(arg1);
	green = (GLshort)NUM2INT(arg2);
	blue = (GLshort)NUM2INT(arg3);
	alpha = (GLshort)NUM2INT(arg4);
	glColor4s(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4ub(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLubyte red;
	GLubyte green;
	GLubyte blue;
	GLubyte alpha;
	red = (GLubyte)NUM2INT(arg1);
	green = (GLubyte)NUM2INT(arg2);
	blue = (GLubyte)NUM2INT(arg3);
	alpha = (GLubyte)NUM2INT(arg4);
	glColor4ub(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4ui(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLuint red;
	GLuint green;
	GLuint blue;
	GLuint alpha;
	red = (GLuint)NUM2UINT(arg1);
	green = (GLuint)NUM2UINT(arg2);
	blue = (GLuint)NUM2UINT(arg3);
	alpha = (GLuint)NUM2UINT(arg4);
	glColor4ui(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_Color4us(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLushort red;
	GLushort green;
	GLushort blue;
	GLushort alpha;
	red = (GLushort)NUM2INT(arg1);
	green = (GLushort)NUM2INT(arg2);
	blue = (GLushort)NUM2INT(arg3);
	alpha = (GLushort)NUM2INT(arg4);
	glColor4us(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_EdgeFlag(obj,arg1)
VALUE obj,arg1;
{
	GLboolean flag;
	flag = (GLboolean)NUM2INT(arg1);
	glEdgeFlag(flag);
	return Qnil;
}

static VALUE
gl_EdgeFlagv(obj,arg1)
VALUE obj,arg1;
{
	GLboolean flag[1] = { GL_FALSE };
	Check_Type(arg1,T_ARRAY);
	ary2cboolean(arg1,flag,1);
	glEdgeFlagv(flag);
	return Qnil;
}

static VALUE
gl_End(obj)
VALUE obj;
{
	glEnd();
	return Qnil;
}

static VALUE
gl_Indexd(obj,arg1)
VALUE obj,arg1;
{
	GLdouble c;
	c = (GLdouble)NUM2DBL(arg1);
	glIndexd(c);
	return Qnil;
}

static VALUE
gl_Indexdv(obj,arg1)
VALUE obj,arg1;
{
	GLdouble c[1] = {0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cdbl(arg1,c,1);
	glIndexdv(c);
	return Qnil;
}

static VALUE
gl_Indexf(obj,arg1)
VALUE obj,arg1;
{
	GLfloat c;
	c = (GLfloat)NUM2DBL(arg1);
	glIndexf(c);
	return Qnil;
}

static VALUE
gl_Indexfv(obj,arg1)
VALUE obj,arg1;
{
	GLfloat c[1] = {0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cflt(arg1,c,1);
	glIndexfv(c);
	return Qnil;
}

static VALUE
gl_Indexi(obj,arg1)
VALUE obj,arg1;
{
	GLint c;
	c = (GLint)NUM2INT(arg1);
	glIndexi(c);
	return Qnil;
}

static VALUE
gl_Indexiv(obj,arg1)
VALUE obj,arg1;
{
	GLint c[1] = {0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cint(arg1,c,1);
	glIndexiv(c);
	return Qnil;
}

static VALUE
gl_Indexs(obj,arg1)
VALUE obj,arg1;
{
	GLshort c;
	c = (GLshort)NUM2INT(arg1);
	glIndexs(c);
	return Qnil;
}

static VALUE
gl_Indexsv(obj,arg1)
VALUE obj,arg1;
{
	GLshort c[1] = {0};
	Check_Type(arg1,T_ARRAY);
	ary2cshort(arg1,c,1);
	glIndexsv(c);
	return Qnil;
}

static VALUE
gl_Normal3b(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLbyte nx;
	GLbyte ny;
	GLbyte nz;
	nx = (GLbyte)NUM2INT(arg1);
	ny = (GLbyte)NUM2INT(arg2);
	nz = (GLbyte)NUM2INT(arg3);
	glNormal3b(nx,ny,nz);
	return Qnil;
}

static VALUE
gl_Normal3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble nx;
	GLdouble ny;
	GLdouble nz;
	nx = (GLdouble)NUM2DBL(arg1);
	ny = (GLdouble)NUM2DBL(arg2);
	nz = (GLdouble)NUM2DBL(arg3);
	glNormal3d(nx,ny,nz);
	return Qnil;
}

static VALUE
gl_Normal3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	nx = (GLfloat)NUM2DBL(arg1);
	ny = (GLfloat)NUM2DBL(arg2);
	nz = (GLfloat)NUM2DBL(arg3);
	glNormal3f(nx,ny,nz);
	return Qnil;
}

static VALUE
gl_Normal3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint nx;
	GLint ny;
	GLint nz;
	nx = (GLint)NUM2INT(arg1);
	ny = (GLint)NUM2INT(arg2);
	nz = (GLint)NUM2INT(arg3);
	glNormal3i(nx,ny,nz);
	return Qnil;
}

static VALUE
gl_Normal3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort nx;
	GLshort ny;
	GLshort nz;
	nx = (GLshort)NUM2INT(arg1);
	ny = (GLshort)NUM2INT(arg2);
	nz = (GLshort)NUM2INT(arg3);
	glNormal3s(nx,ny,nz);
	return Qnil;
}

static VALUE
gl_RasterPos2d(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLdouble x;
	GLdouble y;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	glRasterPos2d(x,y);
	return Qnil;
}

static VALUE
gl_RasterPos2f(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat x;
	GLfloat y;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	glRasterPos2f(x,y);
	return Qnil;
}

static VALUE
gl_RasterPos2i(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLint x;
	GLint y;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	glRasterPos2i(x,y);
	return Qnil;
}

static VALUE
gl_RasterPos2s(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLshort x;
	GLshort y;
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	glRasterPos2s(x,y);
	return Qnil;
}

static VALUE
gl_RasterPos3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	glRasterPos3d(x,y,z);
	return Qnil;
}

static VALUE
gl_RasterPos3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	glRasterPos3f(x,y,z);
	return Qnil;
}

static VALUE
gl_RasterPos3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint x;
	GLint y;
	GLint z;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	z = (GLint)NUM2INT(arg3);
	glRasterPos3i(x,y,z);
	return Qnil;
}

static VALUE
gl_RasterPos3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort x;
	GLshort y;
	GLshort z;
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	z = (GLshort)NUM2INT(arg3);
	glRasterPos3s(x,y,z);
	return Qnil;
}

static VALUE
gl_RasterPos4d(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble w;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	w = (GLdouble)NUM2DBL(arg4);
	glRasterPos4d(x,y,z,w);
	return Qnil;
}

static VALUE
gl_RasterPos4f(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	w = (GLfloat)NUM2DBL(arg4);
	glRasterPos4f(x,y,z,w);
	return Qnil;
}

static VALUE
gl_RasterPos4i(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint x;
	GLint y;
	GLint z;
	GLint w;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	z = (GLint)NUM2INT(arg3);
	w = (GLint)NUM2INT(arg4);
	glRasterPos4i(x,y,z,w);
	return Qnil;
}

static VALUE
gl_RasterPos4s(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLshort x;
	GLshort y;
	GLshort z;
	GLshort w;
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	z = (GLshort)NUM2INT(arg3);
	w = (GLshort)NUM2INT(arg4);
	glRasterPos4s(x,y,z,w);
	return Qnil;
}

static VALUE
gl_Rectd(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLdouble x1;
	GLdouble y1;
	GLdouble x2;
	GLdouble y2;
	x1 = (GLdouble)NUM2DBL(arg1);
	y1 = (GLdouble)NUM2DBL(arg2);
	x2 = (GLdouble)NUM2DBL(arg3);
	y2 = (GLdouble)NUM2DBL(arg4);
	glRectd(x1,y1,x2,y2);
	return Qnil;
}

static VALUE
gl_Rectf(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat x1;
	GLfloat y1;
	GLfloat x2;
	GLfloat y2;
	x1 = (GLfloat)NUM2DBL(arg1);
	y1 = (GLfloat)NUM2DBL(arg2);
	x2 = (GLfloat)NUM2DBL(arg3);
	y2 = (GLfloat)NUM2DBL(arg4);
	glRectf(x1,y1,x2,y2);
	return Qnil;
}

static VALUE
gl_Recti(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint x1;
	GLint y1;
	GLint x2;
	GLint y2;
	x1 = (GLint)NUM2INT(arg1);
	y1 = (GLint)NUM2INT(arg2);
	x2 = (GLint)NUM2INT(arg3);
	y2 = (GLint)NUM2INT(arg4);
	glRecti(x1,y1,x2,y2);
	return Qnil;
}

static VALUE
gl_Rects(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLshort x1;
	GLshort y1;
	GLshort x2;
	GLshort y2;
	x1 = (GLshort)NUM2INT(arg1);
	y1 = (GLshort)NUM2INT(arg2);
	x2 = (GLshort)NUM2INT(arg3);
	y2 = (GLshort)NUM2INT(arg4);
	glRects(x1,y1,x2,y2);
	return Qnil;
}

static VALUE
gl_TexCoord1d(obj,arg1)
VALUE obj,arg1;
{
	GLdouble s;
	s = (GLdouble)NUM2DBL(arg1);
	glTexCoord1d(s);
	return Qnil;
}

static VALUE
gl_TexCoord1f(obj,arg1)
VALUE obj,arg1;
{
	GLfloat s;
	s = (GLfloat)NUM2DBL(arg1);
	glTexCoord1f(s);
	return Qnil;
}

static VALUE
gl_TexCoord1i(obj,arg1)
VALUE obj,arg1;
{
	GLint s;
	s = (GLint)NUM2INT(arg1);
	glTexCoord1i(s);
	return Qnil;
}

static VALUE
gl_TexCoord1s(obj,arg1)
VALUE obj,arg1;
{
	GLshort s;
	s = (GLshort)NUM2INT(arg1);
	glTexCoord1s(s);
	return Qnil;
}

static VALUE
gl_TexCoord2d(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLdouble s;
	GLdouble t;
	s = (GLdouble)NUM2DBL(arg1);
	t = (GLdouble)NUM2DBL(arg2);
	glTexCoord2d(s,t);
	return Qnil;
}

static VALUE
gl_TexCoord2f(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat s;
	GLfloat t;
	s = (GLfloat)NUM2DBL(arg1);
	t = (GLfloat)NUM2DBL(arg2);
	glTexCoord2f(s,t);
	return Qnil;
}

static VALUE
gl_TexCoord2i(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLint s;
	GLint t;
	s = (GLint)NUM2INT(arg1);
	t = (GLint)NUM2INT(arg2);
	glTexCoord2i(s,t);
	return Qnil;
}

static VALUE
gl_TexCoord2s(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLshort s;
	GLshort t;
	s = (GLshort)NUM2INT(arg1);
	t = (GLshort)NUM2INT(arg2);
	glTexCoord2s(s,t);
	return Qnil;
}

static VALUE
gl_TexCoord3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble s;
	GLdouble t;
	GLdouble r;
	s = (GLdouble)NUM2DBL(arg1);
	t = (GLdouble)NUM2DBL(arg2);
	r = (GLdouble)NUM2DBL(arg3);
	glTexCoord3d(s,t,r);
	return Qnil;
}

static VALUE
gl_TexCoord3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat s;
	GLfloat t;
	GLfloat r;
	s = (GLfloat)NUM2DBL(arg1);
	t = (GLfloat)NUM2DBL(arg2);
	r = (GLfloat)NUM2DBL(arg3);
	glTexCoord3f(s,t,r);
	return Qnil;
}

static VALUE
gl_TexCoord3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint s;
	GLint t;
	GLint r;
	s = (GLint)NUM2INT(arg1);
	t = (GLint)NUM2INT(arg2);
	r = (GLint)NUM2INT(arg3);
	glTexCoord3i(s,t,r);
	return Qnil;
}

static VALUE
gl_TexCoord3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort s;
	GLshort t;
	GLshort r;
	s = (GLshort)NUM2INT(arg1);
	t = (GLshort)NUM2INT(arg2);
	r = (GLshort)NUM2INT(arg3);
	glTexCoord3s(s,t,r);
	return Qnil;
}

static VALUE
gl_TexCoord4d(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLdouble s;
	GLdouble t;
	GLdouble r;
	GLdouble q;
	s = (GLdouble)NUM2DBL(arg1);
	t = (GLdouble)NUM2DBL(arg2);
	r = (GLdouble)NUM2DBL(arg3);
	q = (GLdouble)NUM2DBL(arg4);
	glTexCoord4d(s,t,r,q);
	return Qnil;
}

static VALUE
gl_TexCoord4f(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat s;
	GLfloat t;
	GLfloat r;
	GLfloat q;
	s = (GLfloat)NUM2DBL(arg1);
	t = (GLfloat)NUM2DBL(arg2);
	r = (GLfloat)NUM2DBL(arg3);
	q = (GLfloat)NUM2DBL(arg4);
	glTexCoord4f(s,t,r,q);
	return Qnil;
}

static VALUE
gl_TexCoord4i(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint s;
	GLint t;
	GLint r;
	GLint q;
	s = (GLint)NUM2INT(arg1);
	t = (GLint)NUM2INT(arg2);
	r = (GLint)NUM2INT(arg3);
	q = (GLint)NUM2INT(arg4);
	glTexCoord4i(s,t,r,q);
	return Qnil;
}

static VALUE
gl_TexCoord4s(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLshort s;
	GLshort t;
	GLshort r;
	GLshort q;
	s = (GLshort)NUM2INT(arg1);
	t = (GLshort)NUM2INT(arg2);
	r = (GLshort)NUM2INT(arg3);
	q = (GLshort)NUM2INT(arg4);
	glTexCoord4s(s,t,r,q);
	return Qnil;
}

static VALUE
gl_Vertex2d(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLdouble x;
	GLdouble y;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	glVertex2d(x,y);
	return Qnil;
}

static VALUE
gl_Vertex2f(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat x;
	GLfloat y;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	glVertex2f(x,y);
	return Qnil;
}

static VALUE
gl_Vertex2i(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLint x;
	GLint y;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	glVertex2i(x,y);
	return Qnil;
}

static VALUE
gl_Vertex2s(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLshort x;
	GLshort y;
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	glVertex2s(x,y);
	return Qnil;
}

static VALUE
gl_Vertex3d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	glVertex3d(x,y,z);
	return Qnil;
}

static VALUE
gl_Vertex3f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	glVertex3f(x,y,z);
	return Qnil;
}

static VALUE
gl_Vertex3i(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint x;
	GLint y;
	GLint z;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	z = (GLint)NUM2INT(arg3);
	glVertex3i(x,y,z);
	return Qnil;
}

static VALUE
gl_Vertex3s(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLshort x;
	GLshort y;
	GLshort z;
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	z = (GLshort)NUM2INT(arg3);
	glVertex3s(x,y,z);
	return Qnil;
}

static VALUE
gl_Vertex4d(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble w;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	w = (GLdouble)NUM2DBL(arg4);
	glVertex4d(x,y,z,w);
	return Qnil;
}

static VALUE
gl_Vertex4f(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	w = (GLfloat)NUM2DBL(arg4);
	glVertex4f(x,y,z,w);
	return Qnil;
}

static VALUE
gl_Vertex4i(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint x;
	GLint y;
	GLint z;
	GLint w;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	z = (GLint)NUM2INT(arg3);
	w = (GLint)NUM2INT(arg4);
	glVertex4i(x,y,z,w);
	return Qnil;
}

static VALUE
gl_Vertex4s(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLshort x;
	GLshort y;
	GLshort z;
	GLshort w;
	x = (GLshort)NUM2INT(arg1);
	y = (GLshort)NUM2INT(arg2);
	z = (GLshort)NUM2INT(arg3);
	w = (GLshort)NUM2INT(arg4);
	glVertex4s(x,y,z,w);
	return Qnil;
}

static VALUE
gl_ClipPlane(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum plane;
	GLdouble equation[4];
	plane = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	ary2cdbl(arg2, equation, 4);
	glClipPlane(plane,equation);
	return Qnil;
}

static VALUE
gl_ColorMaterial(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum face;
	GLenum mode;
	face = (GLenum)NUM2INT(arg1);
	mode = (GLenum)NUM2INT(arg2);
	glColorMaterial(face,mode);
	return Qnil;
}

static VALUE
gl_CullFace(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glCullFace(mode);
	return Qnil;
}

static VALUE
gl_Fogf(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLfloat)NUM2DBL(arg2);
	glFogf(pname,param);
	return Qnil;
}

static VALUE
gl_Fogfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	ary2cflt(arg2,params,4);
	glFogfv(pname,params);
	return Qnil;
}

static VALUE
gl_Fogi(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLint)NUM2INT(arg2);
	glFogi(pname,param);
	return Qnil;
}

static VALUE
gl_Fogiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	ary2cint(arg2,params,4);
	glFogiv(pname,params);
	return Qnil;
}

static VALUE
gl_FrontFace(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glFrontFace(mode);
	return Qnil;
}

static VALUE
gl_Hint(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum mode;
	target = (GLenum)NUM2UINT(arg1);
	mode = (GLenum)NUM2UINT(arg2);
	glHint(target,mode);
	return Qnil;
}

static VALUE
gl_Lightf(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum light;
	GLenum pname;
	GLfloat param;
	light = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLfloat)NUM2DBL(arg3);
	glLightf(light,pname,param);
	return Qnil;
}

static VALUE
gl_Lightfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum light;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	light = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	glLightfv(light,pname,params);
	return Qnil;
}

static VALUE
gl_Lighti(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum light;
	GLenum pname;
	GLint param;
	light = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLint)NUM2INT(arg3);
	glLighti(light,pname,param);
	return Qnil;
}

static VALUE
gl_Lightiv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum light;
	GLenum pname;
	GLint params[4]={0,0,0,0};
	light = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	glLightiv(light,pname,params);
	return Qnil;
}

static VALUE
gl_LightModelf(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLfloat)NUM2DBL(arg2);
	glLightModelf(pname,param);
	return Qnil;
}

static VALUE
gl_LightModelfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	ary2cflt(arg2,params,4);
	glLightModelfv(pname,params);
	return Qnil;
}

static VALUE
gl_LightModeli(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLint)NUM2INT(arg2);
	glLightModeli(pname,param);
	return Qnil;
}

static VALUE
gl_LightModeliv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	pname = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	ary2cint(arg2,params,4);
	glLightModeliv(pname,params);
	return Qnil;
}

static VALUE
gl_LineStipple(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLint factor;
	GLushort pattern;
	factor = (GLint)NUM2INT(arg1);
	pattern = (GLushort)(NUM2INT(arg2) & 0xFFFF);
	glLineStipple(factor,pattern);
	return Qnil;
}

static VALUE
gl_LineWidth(obj,arg1)
VALUE obj,arg1;
{
	GLfloat width;
	width = (GLfloat)NUM2DBL(arg1);
	glLineWidth(width);
	return Qnil;
}

static VALUE
gl_Materialf(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum face;
	GLenum pname;
	GLfloat param;
	face = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLfloat)NUM2DBL(arg3);
	glMaterialf(face,pname,param);
	return Qnil;
}

static VALUE
gl_Materialfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum face;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	face = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	glMaterialfv(face,pname,params);
	return Qnil;
}

static VALUE
gl_Materiali(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum face;
	GLenum pname;
	GLint param;
	face = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLint)NUM2INT(arg3);
	glMateriali(face,pname,param);
	return Qnil;
}

static VALUE
gl_Materialiv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum face;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	face = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	glMaterialiv(face,pname,params);
	return Qnil;
}

static VALUE
gl_PointSize(obj,arg1)
VALUE obj,arg1;
{
	GLfloat size;
	size = (GLfloat)NUM2DBL(arg1);
	glPointSize(size);
	return Qnil;
}

static VALUE
gl_PolygonMode(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum face;
	GLenum mode;
	face = (GLenum)NUM2INT(arg1);
	mode = (GLenum)NUM2INT(arg2);
	glPolygonMode(face,mode);
	return Qnil;
}

static VALUE
gl_PolygonStipple(obj,arg1)
VALUE obj,arg1;
{
	GLubyte mask[128];
	memset(mask, 0x0, sizeof(GLubyte)*128);
	if (TYPE(arg1) == T_ARRAY) {
		ary2cubyte(arg1,mask,128);
	}
	else if (TYPE(arg1) == T_STRING) {
		if (RSTRING(arg1)->len < 128)
			rb_raise(rb_eArgError, "string length:%d", RSTRING(arg1)->len);
		memcpy(mask, RSTRING(arg1)->ptr, 128);
	}
	else
		Check_Type(arg1,T_STRING); /* force exception */
	glPolygonStipple(mask);
	return Qnil;
}

static VALUE
gl_Scissor(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	height = (GLsizei)NUM2UINT(arg4);
	glScissor(x,y,width,height);
	return Qnil;
}

static VALUE
gl_ShadeModel(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glShadeModel(mode);
	return Qnil;
}

static VALUE
gl_TexParameterf(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat param;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLfloat)NUM2DBL(arg3);
	glTexParameterf(target,pname,param);
	return Qnil;
}

static VALUE
gl_TexParameterfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	glTexParameterfv(target,pname,params);
	return Qnil;
}

static VALUE
gl_TexParameteri(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint param;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLint)NUM2INT(arg3);
	glTexParameteri(target,pname,param);
	return Qnil;
}

static VALUE
gl_TexParameteriv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	glTexParameteriv(target,pname,params);
	return Qnil;
}

/* FIXME: spatna funkce ? */
static VALUE
gl_TexImage1D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8;
{
	GLenum target;
	GLint level;
	GLint components;
	GLsizei width;
	GLint border;
	GLenum format;
	GLenum type;
	const char *pixels;
	int size;
	int type_size;
	int format_size;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	components = (GLint)NUM2INT(arg3);
	width = (GLsizei)NUM2UINT(arg4);
	border = (GLint)NUM2INT(arg5);
	format = (GLenum)NUM2INT(arg6);
	type = (GLenum)NUM2INT(arg7);
	type_size = gltype_size(type);
	format_size = glformat_size(format);

	if (type_size == -1 || format_size == -1)
		return Qnil;
	if (type==GL_BITMAP)
		size = format_size*(width/8);
	else
		size = type_size*format_size*width;

	if (target == GL_PROXY_TEXTURE_1D || NIL_P(arg8)) { /* proxy texture, no data read */
		pixels = NULL;
	} else {
		if (TYPE(arg8) == T_FIXNUM || TYPE(arg8) == T_BIGNUM) { /* arg8 is offset to unpack buffer */
			pixels = (const char *)NUM2UINT(arg8);
		} else if (TYPE(arg8) == T_STRING) { /* image data */
			if (RSTRING(arg8)->len < size)
				rb_raise(rb_eArgError, "string length:%d",RSTRING(arg8)->len);
			pixels = RSTRING(arg8)->ptr;
		} else {
			Check_Type(arg8,T_STRING); /* force exception */
			return Qnil;
		}
	}
	glTexImage1D(target,level,components,width,border,format,type,pixels);
	return Qnil;
}

static VALUE
gl_TexImage2D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9;
{
	GLenum target;
	GLint level;
	GLint components;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLenum format;
	GLenum type;
	const char *pixels;
	int size;
	int type_size;
	int format_size;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	components = (GLint)NUM2INT(arg3);
	width = (GLsizei)NUM2UINT(arg4);
	height = (GLsizei)NUM2UINT(arg5);
	border = (GLint)NUM2INT(arg6);
	format = (GLenum)NUM2INT(arg7);
	type = (GLenum)NUM2INT(arg8);
	type_size = gltype_size(type);
	format_size = glformat_size(format);

	if (type_size == -1 || format_size == -1)
		return Qnil;
	if (type==GL_BITMAP)
		size = format_size*((height*width)/8);
	else
		size = type_size*format_size*height*width;
	
	if (target == GL_PROXY_TEXTURE_2D || target == GL_PROXY_TEXTURE_CUBE_MAP || NIL_P(arg9)) { /* proxy texture, no data read */
		pixels = NULL;
	} else {
		if (TYPE(arg9) == T_FIXNUM || TYPE(arg9) == T_BIGNUM) { /* arg9 is offset to unpack buffer */
			pixels = (const char *)NUM2UINT(arg9);
		} else if (TYPE(arg9) == T_STRING) { /* image data */
			if (RSTRING(arg9)->len < size)
				rb_raise(rb_eArgError, "string length:%d",RSTRING(arg9)->len);
			pixels = RSTRING(arg9)->ptr;
		} else {
			Check_Type(arg9,T_STRING); /* force exception */
			return Qnil;
		}
	}
	glTexImage2D(target,level,components,width,height,border,format,type,pixels);
	return Qnil;
}

static VALUE
gl_TexEnvf(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat param;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLfloat)NUM2DBL(arg3);
	glTexEnvf(target,pname,param);
	return Qnil;
}

static VALUE
gl_TexEnvfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	glTexEnvfv(target,pname,params);
	return Qnil;
}

static VALUE
gl_TexEnvi(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint param;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLint)NUM2INT(arg3);
	glTexEnvi(target,pname,param);
	return Qnil;
}

static VALUE
gl_TexEnviv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	glTexEnviv(target,pname,params);
	return Qnil;
}

static VALUE
gl_TexGend(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum coord;
	GLenum pname;
	GLdouble param;
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLdouble)NUM2DBL(arg3);
	glTexGend(coord,pname,param);
	return Qnil;
}

static VALUE
gl_TexGendv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum coord;
	GLenum pname;
	GLdouble params[4] = {0.0,0.0,0.0,0.0};
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cdbl(arg3,params,4);
	glTexGendv(coord,pname,params);
	return Qnil;
}


static VALUE
gl_TexGenf(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum coord;
	GLenum pname;
	GLfloat param;
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLfloat)NUM2DBL(arg3);
	glTexGenf(coord,pname,param);
	return Qnil;
}

static VALUE
gl_TexGenfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum coord;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cflt(arg3,params,4);
	glTexGenfv(coord,pname,params);
	return Qnil;
}

static VALUE
gl_TexGeni(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum coord;
	GLenum pname;
	GLint param;
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	param = (GLint)NUM2INT(arg3);
	glTexGeni(coord,pname,param);
	return Qnil;
}

static VALUE
gl_TexGeniv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum coord;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	Check_Type(arg3,T_ARRAY);
	ary2cint(arg3,params,4);
	glTexGeniv(coord,pname,params);
	return Qnil;
}
static VALUE g_current_feed_buffer;
static VALUE
gl_FeedbackBuffer(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLsizei size;
	GLenum type;
	size = (GLsizei)NUM2UINT(arg1);
	type = (GLenum)NUM2INT(arg2);
	g_current_feed_buffer = allocate_buffer_with_string(sizeof(GLfloat)*size);
	rb_str_freeze(g_current_feed_buffer);
	glFeedbackBuffer(size, type, (GLfloat*)RSTRING(g_current_feed_buffer)->ptr);
	return g_current_feed_buffer;
}

static VALUE g_current_sel_buffer;
static VALUE
gl_SelectBuffer(obj,arg1)
VALUE obj,arg1;
{
	GLsizei size;
	size = (GLsizei)NUM2UINT(arg1);
	g_current_sel_buffer = allocate_buffer_with_string(sizeof(GLuint)*size);
	rb_str_freeze(g_current_sel_buffer);
	glSelectBuffer(size, (GLuint*)RSTRING(g_current_sel_buffer)->ptr);
	return g_current_sel_buffer;
}

static VALUE
gl_RenderMode(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	GLint ret;
	mode = (GLenum)NUM2INT(arg1);
	ret = glRenderMode(mode);
	return INT2NUM(ret);
}

static VALUE
gl_InitNames(obj)
VALUE obj;
{
	glInitNames();
	return Qnil;
}

static VALUE
gl_LoadName(obj,arg1)
VALUE obj,arg1;
{
	GLuint name;
	name = (GLuint)NUM2UINT(arg1);
	glLoadName(name);
	return Qnil;
}

static VALUE
gl_PassThrough(obj,arg1)
VALUE obj,arg1;
{
	GLfloat token;
	token = (GLfloat)NUM2DBL(arg1);
	glPassThrough(token);
	return Qnil;
}

static VALUE
gl_PopName(obj)
VALUE obj;
{
	glPopName();
	return Qnil;
}

static VALUE
gl_PushName(obj,arg1)
VALUE obj,arg1;
{
	GLuint name;
	name = (GLuint)NUM2UINT(arg1);
	glPushName(name);
	return Qnil;
}

static VALUE
gl_DrawBuffer(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glDrawBuffer(mode);
	return Qnil;
}

static VALUE
gl_Clear(obj,arg1)
VALUE obj,arg1;
{
	GLbitfield mask;
	mask = (GLbitfield)NUM2UINT(arg1);
	glClear(mask);
	return Qnil;
}

static VALUE
gl_ClearAccum(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
	red = (GLfloat)NUM2DBL(arg1);
	green = (GLfloat)NUM2DBL(arg2);
	blue = (GLfloat)NUM2DBL(arg3);
	alpha = (GLfloat)NUM2DBL(arg4);
	glClearAccum(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_ClearIndex(obj,arg1)
VALUE obj,arg1;
{
	GLfloat c;
	c = (GLfloat)NUM2DBL(arg1);
	glClearIndex(c);
	return Qnil;
}

static VALUE
gl_ClearColor(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
	red = (GLclampf)NUM2DBL(arg1);
	green = (GLclampf)NUM2DBL(arg2);
	blue = (GLclampf)NUM2DBL(arg3);
	alpha = (GLclampf)NUM2DBL(arg4);
	glClearColor(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_ClearStencil(obj,arg1)
VALUE obj,arg1;
{
	GLint s;
	s = (GLint)NUM2INT(arg1);
	glClearStencil(s);
	return Qnil;
}

static VALUE
gl_ClearDepth(obj,arg1)
VALUE obj,arg1;
{
	GLclampd depth;
	depth = (GLclampd)NUM2DBL(arg1);
	glClearDepth(depth);
	return Qnil;
}

static VALUE
gl_StencilMask(obj,arg1)
VALUE obj,arg1;
{
	GLuint mask;
	mask = (GLuint)NUM2UINT(arg1);
	glStencilMask(mask);
	return Qnil;
}

static VALUE
gl_ColorMask(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLboolean red;
	GLboolean green;
	GLboolean blue;
	GLboolean alpha;
	red = (GLboolean)NUM2INT(arg1);
	green = (GLboolean)NUM2INT(arg2);
	blue = (GLboolean)NUM2INT(arg3);
	alpha = (GLboolean)NUM2INT(arg4);
	glColorMask(red,green,blue,alpha);
	return Qnil;
}

static VALUE
gl_DepthMask(obj,arg1)
VALUE obj,arg1;
{
	GLboolean flag;
	flag = (GLboolean)NUM2INT(arg1);
	glDepthMask(flag);
	return Qnil;
}

static VALUE
gl_IndexMask(obj,arg1)
VALUE obj,arg1;
{
	GLuint mask;
	mask = (GLuint)NUM2UINT(arg1);
	glIndexMask(mask);
	return Qnil;
}

static VALUE
gl_Accum(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum op;
	GLfloat value;
	op = (GLenum)NUM2INT(arg1);
	value = (GLfloat)NUM2DBL(arg2);
	glAccum(op,value);
	return Qnil;
}

static VALUE
gl_Disable(obj,arg1)
VALUE obj,arg1;
{
	GLenum cap;
	cap = (GLenum)NUM2INT(arg1);
	glDisable(cap);
	return Qnil;
}

static VALUE
gl_Enable(obj,arg1)
VALUE obj,arg1;
{
	GLenum cap;
	cap = (GLenum)NUM2INT(arg1);
	glEnable(cap);
	return Qnil;
}

static VALUE
gl_Finish(obj)
VALUE obj;
{
	glFinish();
	return Qnil;
}

static VALUE
gl_Flush(obj)
VALUE obj;
{
	glFlush();
	return Qnil;
}

static VALUE
gl_PopAttrib(obj)
VALUE obj;
{
	glPopAttrib();
	return Qnil;
}

static VALUE
gl_PushAttrib(obj,arg1)
VALUE obj,arg1;
{
	GLbitfield mask;
	mask = (GLbitfield)NUM2UINT(arg1);
	glPushAttrib(mask);
	return Qnil;
}

static VALUE
gl_Map1d(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLdouble u1;
	GLdouble u2;
	GLint stride;
	GLint order;
	GLdouble *points;

	VALUE work_ary;

	target = (GLenum)NUM2INT(arg1);
	u1 = (GLdouble)NUM2DBL(arg2);
	u2 = (GLdouble)NUM2DBL(arg3);
	stride = (GLint)NUM2INT(arg4);
	order = (GLint)NUM2INT(arg5);
	points = ALLOC_N(GLdouble, order*stride);
	work_ary = rb_ary_new();
	mary2ary(arg6, work_ary);
	ary2cdbl(work_ary, points, order*stride);
	glMap1d(target, u1, u2, stride, order, points);
	xfree(points);
	return Qnil;
}

static VALUE
gl_Map1f(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLfloat u1;
	GLfloat u2;
	GLint stride;
	GLint order;
	GLfloat *points;

	VALUE work_ary;

	target = (GLenum)NUM2INT(arg1);
	u1 = (GLfloat)NUM2DBL(arg2);
	u2 = (GLfloat)NUM2DBL(arg3);
	stride = (GLint)NUM2INT(arg4);
	order = (GLint)NUM2INT(arg5);
	points = ALLOC_N(GLfloat, order*stride);
	work_ary = rb_ary_new();
	mary2ary(arg6, work_ary);
	ary2cflt(work_ary, points, order*stride);
	glMap1f(target, u1, u2, stride, order, points);
	xfree(points);
	return Qnil;
}

static VALUE
gl_Map2d(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	GLenum target;
	GLdouble u1;
	GLdouble u2;
	GLint ustride;
	GLint uorder;
	GLdouble v1;
	GLdouble v2;
	GLint vstride;
	GLint vorder;
	GLdouble *points;

	VALUE work_ary;

	target = (GLenum)NUM2INT(arg1);
	u1 = (GLdouble)NUM2INT(arg2);
	u2 = (GLdouble)NUM2INT(arg3);
	ustride = (GLint)NUM2INT(arg4);
	uorder = (GLint)NUM2INT(arg5);
	v1 = (GLdouble)NUM2INT(arg6);
	v2 = (GLdouble)NUM2INT(arg7);
	vstride = (GLint)NUM2INT(arg8);
	vorder = (GLint)NUM2INT(arg9);
	points = ALLOC_N(GLdouble, MAX(ustride*uorder, vstride*vorder));
	work_ary = rb_ary_new();
	mary2ary(arg10, work_ary);
	ary2cdbl(arg10, points, MAX(ustride*uorder, vstride*vorder));
	glMap2d(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	xfree(points);
	return Qnil;
}

static VALUE
gl_Map2f(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	GLenum target;
	GLfloat u1;
	GLfloat u2;
	GLint ustride;
	GLint uorder;
	GLfloat v1;
	GLfloat v2;
	GLint vstride;
	GLint vorder;
	GLfloat *points;

	VALUE work_ary;

	target = (GLenum)NUM2INT(arg1);
	u1 = (GLfloat)NUM2INT(arg2);
	u2 = (GLfloat)NUM2INT(arg3);
	ustride = (GLint)NUM2INT(arg4);
	uorder = (GLint)NUM2INT(arg5);
	v1 = (GLfloat)NUM2INT(arg6);
	v2 = (GLfloat)NUM2INT(arg7);
	vstride = (GLint)NUM2INT(arg8);
	vorder = (GLint)NUM2INT(arg9);
	points = ALLOC_N(GLfloat, MAX(ustride*uorder, vstride*vorder));
	work_ary = rb_ary_new();
	mary2ary(arg10, work_ary);
	ary2cflt(arg10, points, MAX(ustride*uorder, vstride*vorder));
	glMap2f(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	xfree(points);
	return Qnil;
}

static VALUE
gl_MapGrid1d(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint un;
	GLdouble u1;
	GLdouble u2;
	un = (GLint)NUM2INT(arg1);
	u1 = (GLdouble)NUM2DBL(arg2);
	u2 = (GLdouble)NUM2DBL(arg3);
	glMapGrid1d(un,u1,u2);
	return Qnil;
}

static VALUE
gl_MapGrid1f(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLint un;
	GLfloat u1;
	GLfloat u2;
	un = (GLint)NUM2INT(arg1);
	u1 = (GLfloat)NUM2DBL(arg2);
	u2 = (GLfloat)NUM2DBL(arg3);
	glMapGrid1f(un,u1,u2);
	return Qnil;
}

static VALUE
gl_MapGrid2d(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLint un;
	GLdouble u1;
	GLdouble u2;
	GLint vn;
	GLdouble v1;
	GLdouble v2;
	un = (GLint)NUM2INT(arg1);
	u1 = (GLdouble)NUM2DBL(arg2);
	u2 = (GLdouble)NUM2DBL(arg3);
	vn = (GLint)NUM2INT(arg4);
	v1 = (GLdouble)NUM2DBL(arg5);
	v2 = (GLdouble)NUM2DBL(arg6);
	glMapGrid2d(un,u1,u2,vn,v1,v2);
	return Qnil;
}

static VALUE
gl_MapGrid2f(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLint un;
	GLfloat u1;
	GLfloat u2;
	GLint vn;
	GLfloat v1;
	GLfloat v2;
	un = (GLint)NUM2INT(arg1);
	u1 = (GLfloat)NUM2DBL(arg2);
	u2 = (GLfloat)NUM2DBL(arg3);
	vn = (GLint)NUM2INT(arg4);
	v1 = (GLfloat)NUM2DBL(arg5);
	v2 = (GLfloat)NUM2DBL(arg6);
	glMapGrid2f(un,u1,u2,vn,v1,v2);
	return Qnil;
}

static VALUE
gl_EvalCoord1d(obj,arg1)
VALUE obj,arg1;
{
	GLdouble u;
	u = (GLdouble)NUM2DBL(arg1);
	glEvalCoord1d(u);
	return Qnil;
}

static VALUE
gl_EvalCoord1dv(obj,arg1)
VALUE obj,arg1;
{
	GLdouble params[1] = {0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cdbl(arg1,params,1);
	glEvalCoord1dv(params);
	return Qnil;
}

static VALUE
gl_EvalCoord1f(obj,arg1)
VALUE obj,arg1;
{
	GLfloat u;
	u = (GLfloat)NUM2DBL(arg1);
	glEvalCoord1f(u);
	return Qnil;
}

static VALUE
gl_EvalCoord1fv(obj,arg1)
VALUE obj,arg1;
{
	GLfloat params[1] = {0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cflt(arg1,params,1);
	glEvalCoord1fv(params);
	return Qnil;
}

static VALUE
gl_EvalCoord2d(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLdouble u;
	GLdouble v;
	u = (GLdouble)NUM2DBL(arg1);
	v = (GLdouble)NUM2DBL(arg2);
	glEvalCoord2d(u,v);
	return Qnil;
}

static VALUE
gl_EvalCoord2dv(obj,arg1)
VALUE obj,arg1;
{
	GLdouble params[2] = {0.0,0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cdbl(arg1,params,2);
	glEvalCoord2dv(params);
	return Qnil;
}

static VALUE
gl_EvalCoord2f(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat u;
	GLfloat v;
	u = (GLfloat)NUM2DBL(arg1);
	v = (GLfloat)NUM2DBL(arg2);
	glEvalCoord2f(u,v);
	return Qnil;
}

static VALUE
gl_EvalCoord2fv(obj,arg1)
VALUE obj,arg1;
{
	GLfloat params[2] = {0.0,0.0};
	Check_Type(arg1,T_ARRAY);
	ary2cflt(arg1,params,2);
	glEvalCoord2fv(params);
	return Qnil;
}

static VALUE
gl_EvalMesh1(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum mode;
	GLint i1;
	GLint i2;
	mode = (GLenum)NUM2INT(arg1);
	i1 = (GLint)NUM2INT(arg2);
	i2 = (GLint)NUM2INT(arg3);
	glEvalMesh1(mode,i1,i2);
	return Qnil;
}

static VALUE
gl_EvalPoint1(obj,arg1)
VALUE obj,arg1;
{
	GLint i;
	i = (GLint)NUM2INT(arg1);
	glEvalPoint1(i);
	return Qnil;
}

static VALUE
gl_EvalMesh2(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLenum mode;
	GLint i1;
	GLint i2;
	GLint j1;
	GLint j2;
	mode = (GLenum)NUM2INT(arg1);
	i1 = (GLint)NUM2INT(arg2);
	i2 = (GLint)NUM2INT(arg3);
	j1 = (GLint)NUM2INT(arg4);
	j2 = (GLint)NUM2INT(arg5);
	glEvalMesh2(mode,i1,i2,j1,j2);
	return Qnil;
}

static VALUE
gl_EvalPoint2(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLint i;
	GLint j;
	i = (GLint)NUM2INT(arg1);
	j = (GLint)NUM2INT(arg2);
	glEvalPoint2(i,j);
	return Qnil;
}

static VALUE
gl_AlphaFunc(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum func;
	GLclampf ref;
	func = (GLenum)NUM2INT(arg1);
	ref = (GLclampf)NUM2DBL(arg2);
	glAlphaFunc(func,ref);
	return Qnil;
}

static VALUE
gl_BlendFunc(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum sfactor;
	GLenum dfactor;
	sfactor = (GLenum)NUM2INT(arg1);
	dfactor = (GLenum)NUM2INT(arg2);
	glBlendFunc(sfactor,dfactor);
	return Qnil;
}

static VALUE
gl_LogicOp(obj,arg1)
VALUE obj,arg1;
{
	GLenum opcode;
	opcode = (GLenum)NUM2INT(arg1);
	glLogicOp(opcode);
	return Qnil;
}

static VALUE
gl_StencilFunc(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum func;
	GLint ref;
	GLuint mask;
	func = (GLenum)NUM2INT(arg1);
	ref = (GLint)NUM2INT(arg2);
	mask = (GLuint)NUM2UINT(arg3);
	glStencilFunc(func,ref,mask);
	return Qnil;
}

static VALUE
gl_StencilOp(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum fail;
	GLenum zfail;
	GLenum zpass;
	fail = (GLenum)NUM2INT(arg1);
	zfail = (GLenum)NUM2INT(arg2);
	zpass = (GLenum)NUM2INT(arg3);
	glStencilOp(fail,zfail,zpass);
	return Qnil;
}

static VALUE
gl_DepthFunc(obj,arg1)
VALUE obj,arg1;
{
	GLenum func;
	func = (GLenum)NUM2INT(arg1);
	glDepthFunc(func);
	return Qnil;
}

static VALUE
gl_PixelZoom(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat xfactor;
	GLfloat yfactor;
	xfactor = (GLfloat)NUM2DBL(arg1);
	yfactor = (GLfloat)NUM2DBL(arg2);
	glPixelZoom(xfactor,yfactor);
	return Qnil;
}

static VALUE
gl_PixelTransferf(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLfloat)NUM2DBL(arg2);
	glPixelTransferf(pname,param);
	return Qnil;
}

static VALUE
gl_PixelTransferi(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLint)NUM2INT(arg2);
	glPixelTransferi(pname,param);
	return Qnil;
}

static VALUE
gl_PixelStoref(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLfloat param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLfloat)NUM2DBL(arg2);
	glPixelStoref(pname,param);
	return Qnil;
}

static VALUE
gl_PixelStorei(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum pname;
	GLint param;
	pname = (GLenum)NUM2INT(arg1);
	param = (GLint)NUM2INT(arg2);
	glPixelStorei(pname,param);
	return Qnil;
}

static VALUE
gl_PixelMapfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum map;
	GLfloat *values;
	GLsizei size;
	map = (GLenum)NUM2INT(arg1);	
	Check_Type(arg2,T_ARRAY);
	size = RARRAY(arg2)->len;
	values = ALLOC_N(GLfloat,size);
	ary2cflt(arg2,values,size);
	glPixelMapfv(map,size,values);
	xfree(values);
	return Qnil;
}

static VALUE
gl_PixelMapuiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum map;
	GLuint *values;
	GLsizei size;
	map = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	size = RARRAY(arg2)->len;
	values = ALLOC_N(GLuint,size);
	ary2cuint(arg2,values,size);
	glPixelMapuiv(map,size,values);
	xfree(values);
	return Qnil;
}

static VALUE
gl_PixelMapusv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum map;
	GLushort *values;
	GLsizei size;
	map = (GLenum)NUM2INT(arg1);
	Check_Type(arg2,T_ARRAY);
	size = RARRAY(arg2)->len;
	values = ALLOC_N(GLushort,size);
	ary2cushort(arg2,values,size);
	glPixelMapusv(map,size,values);
	xfree(values);
	return Qnil;
}

static VALUE
gl_ReadBuffer(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glReadBuffer(mode);
	return Qnil;
}

static VALUE
gl_CopyPixels(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLenum type;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	height = (GLsizei)NUM2UINT(arg4);
	type = (GLenum)NUM2INT(arg5);
	glCopyPixels(x,y,width,height,type);
	return Qnil;
}

static VALUE
gl_ReadPixels(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	int format;
	int type;
	VALUE pixels;
	GLsizei type_size;
	GLsizei format_size;
	GLsizei size;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	height = (GLsizei)NUM2UINT(arg4);
	format = NUM2INT(arg5);
	type = NUM2INT(arg6);
	type_size = gltype_size(type);
	format_size = glformat_size(format);
	if (type_size == -1 || format_size == -1) 
		return Qnil;

	if (type==GL_BITMAP)
		size = format_size*((width*height)/8);
	else
		size = width*height*format_size*type_size;
	pixels = allocate_buffer_with_string(size);
	FORCE_PIXEL_STORE_MODE
	glReadPixels(x,y,width,height,format,type,(GLvoid*)RSTRING(pixels)->ptr);
	RESTORE_PIXEL_STORE_MODE
	return pixels;
}

static VALUE
gl_DrawPixels(obj,arg1,arg2,arg3,arg4,arg5)
VALUE obj,arg1,arg2,arg3,arg4,arg5;
{
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	const char *pixels;
	GLsizei size;
	GLsizei type_size;
	GLsizei format_size;
	width = (GLsizei)NUM2UINT(arg1);
	height = (GLsizei)NUM2UINT(arg2);
	format = (GLenum)NUM2INT(arg3);
	type = (GLenum)NUM2INT(arg4);
	Check_Type(arg5,T_STRING);
	type_size = gltype_size(type);
	format_size = glformat_size(format);
	if (type_size == -1 || format_size == -1)
		rb_raise(rb_eArgError, "type/format mismatch");
	if (type==GL_BITMAP)
		size = format_size*(width/8);
	else
		size = type_size*format_size*width*height;
	if (RSTRING(arg5)->len < size)
		rb_raise(rb_eArgError, "string length:%d", RSTRING(arg5)->len);
	pixels = RSTRING(arg5)->ptr;
	glDrawPixels(width,height,format,type,pixels);
	return Qnil;
}

static VALUE
gl_GetClipPlane(obj,arg1)
VALUE obj,arg1;
{
	GLenum plane;
	GLdouble equation[4] = {0.0,0.0,0.0,0.0};
	VALUE retary;
	int i;
	plane = (GLenum)NUM2INT(arg1);
	glGetClipPlane(plane,equation);
	retary = rb_ary_new2(4);
	for(i=0;i<4;i++)
		rb_ary_push(retary, rb_float_new(equation[i]));
	return retary;
}

#define GLGET_FUNC(_name_,_type_,_conv_) \
static VALUE \
gl_Get##_name_##v(obj,arg1) \
VALUE obj,arg1; \
{ \
	GLenum pname; \
	int nitems; \
	VALUE ary, ary2; \
	int i,j; \
	_type_ items[32];  \
	pname = NUM2INT(arg1); \
	switch(pname) { \
	case GL_ACCUM_CLEAR_VALUE: \
	case GL_BLEND_COLOR: \
	case GL_COLOR_CLEAR_VALUE: \
	case GL_COLOR_WRITEMASK: \
	case GL_CURRENT_COLOR: \
	case GL_CURRENT_RASTER_COLOR: \
	case GL_CURRENT_RASTER_POSITION: \
	case GL_CURRENT_RASTER_SECONDARY_COLOR: \
	case GL_CURRENT_RASTER_TEXTURE_COORDS: \
	case GL_CURRENT_TEXTURE_COORDS: \
	case GL_FOG_COLOR: \
	case GL_LIGHT_MODEL_AMBIENT: \
	case GL_MAP2_GRID_DOMAIN: \
	case GL_CURRENT_SECONDARY_COLOR: \
	case GL_SCISSOR_BOX: \
	case GL_TEXTURE_ENV_COLOR: \
	case GL_VIEWPORT: \
		nitems = 4; \
		break; \
	case GL_CURRENT_NORMAL: \
	case GL_POINT_DISTANCE_ATTENUATION: \
		nitems = 3; \
		break; \
	case GL_DEPTH_RANGE:	 \
	case GL_LINE_WIDTH_RANGE: \
	case GL_MAP1_GRID_DOMAIN: \
	case GL_MAP2_GRID_SEGMENTS: \
	case GL_MAX_VIEWPORT_DIMS: \
	case GL_POINT_SIZE_RANGE: \
	case GL_POLYGON_MODE: \
	case GL_ALIASED_LINE_WIDTH_RANGE: \
	case GL_ALIASED_POINT_SIZE_RANGE: \
		nitems = 2; \
		break; \
	case GL_MODELVIEW_MATRIX: \
	case GL_PROJECTION_MATRIX: \
	case GL_TEXTURE_MATRIX: \
		glGet##_name_##v(pname, items); \
		ary = rb_ary_new2(4); \
		for (i = 0; i < 4; i++) { \
			ary2 = rb_ary_new2(4); \
			rb_ary_push(ary, ary2); \
			for (j = 0; j < 4; j++) \
				rb_ary_push(ary2, _conv_(items[i*4+j])); \
		} \
		return ary; \
	case GL_POLYGON_STIPPLE: \
		glGet##_name_##v(pname, items); \
		return rb_str_new((const char*)items, 32); \
	default: /* size=1 */ \
		glGet##_name_##v(pname, items); \
		return _conv_(items[0]); \
	} \
	glGet##_name_##v(pname, items); \
	ary = rb_ary_new2(nitems); \
	for (i = 0; i < nitems; i++) \
		rb_ary_push(ary, _conv_(items[i])); \
	return ary; \
}

GLGET_FUNC(Double,GLdouble,rb_float_new)
GLGET_FUNC(Float,GLfloat,rb_float_new)
GLGET_FUNC(Integer,GLint,INT2NUM)
GLGET_FUNC(Boolean,GLboolean,INT2NUM)

#undef GLGET_FUNC

static VALUE
gl_GetError(obj)
VALUE obj;
{
	GLenum ret;
	ret = glGetError();
	return INT2NUM(ret);
}

static VALUE
gl_GetLightfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum light;
	GLenum pname;
	GLsizei size;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	VALUE retary;
	int i;
	light = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_AMBIENT:
		case GL_DIFFUSE:
		case GL_SPECULAR:
		case GL_POSITION:
			size = 4;
			break;
		case GL_SPOT_DIRECTION:
			size = 3;
			break;
		case GL_CONSTANT_ATTENUATION:
		case GL_LINEAR_ATTENUATION:
		case GL_QUADRATIC_ATTENUATION:
		case GL_SPOT_EXPONENT:
		case GL_SPOT_CUTOFF:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetLightfv(light,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static VALUE
gl_GetLightiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum light;
	GLenum pname;
	GLsizei size;
	GLint params[4] = {0,0,0,0};
	VALUE retary;
	int i;
	light = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_AMBIENT:
		case GL_DIFFUSE:
		case GL_SPECULAR:
		case GL_POSITION:
			size = 4;
			break;
		case GL_SPOT_DIRECTION:
			size = 3;
			break;
		case GL_CONSTANT_ATTENUATION:
		case GL_LINEAR_ATTENUATION:
		case GL_QUADRATIC_ATTENUATION:
		case GL_SPOT_EXPONENT:
		case GL_SPOT_CUTOFF:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetLightiv(light,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}

static VALUE
gl_GetMapdv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum query;
	int dims;
	int pointsize;
	int size=0;
	int i;
	GLdouble *points;
	VALUE retary;
	GLint order[2] = {0,0}; /* for GL_COEFF, [order] or [uorder,vorder] (MAP1/MAP2) */
	target = (GLenum)NUM2INT(arg1);
	query = (GLenum)NUM2INT(arg2);
	switch(target) {
		case GL_MAP1_INDEX:
		case GL_MAP1_TEXTURE_COORD_1: dims=1; pointsize=1; break;
		case GL_MAP1_TEXTURE_COORD_2: dims=1; pointsize=2; break;
		case GL_MAP1_VERTEX_3:
		case GL_MAP1_NORMAL: 
		case GL_MAP1_TEXTURE_COORD_3: dims=1; pointsize=3; break;
		case GL_MAP1_COLOR_4: 
		case GL_MAP1_TEXTURE_COORD_4:
		case GL_MAP1_VERTEX_4: dims=1; pointsize=4; break;
		case GL_MAP2_INDEX:
		case GL_MAP2_TEXTURE_COORD_1: dims=2; pointsize=1; break;
		case GL_MAP2_TEXTURE_COORD_2: dims=2; pointsize=2; break;
		case GL_MAP2_VERTEX_3:
		case GL_MAP2_NORMAL: 
		case GL_MAP2_TEXTURE_COORD_3: dims=2; pointsize=3; break;
		case GL_MAP2_COLOR_4: 
		case GL_MAP2_TEXTURE_COORD_4:
		case GL_MAP2_VERTEX_4: dims=2; pointsize=4; break;
		default:
			rb_raise(rb_eArgError, "unknown target:%d",target);
			break; /* not reached */
	}
	switch(query) {
		case GL_ORDER: size = dims;	break;
		case GL_DOMAIN: size = dims*2; break;
		case GL_COEFF:
			glGetMapiv(target,GL_ORDER,order);
			if (dims==1)
				size = order[0] * pointsize;
			else
				size = (order[0]*order[1]) * pointsize;
			break;
		default:
			rb_raise(rb_eArgError, "unknown target:%d",target);
			break; /* not reached */
	}
	points = ALLOC_N(GLdouble,size);
	glGetMapdv(target,query,points);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(points[i]));
	xfree(points);
	return retary;
}

static VALUE
gl_GetMapfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum query;
	int dims;
	int pointsize;
	int size=0;
	int i;
	GLfloat *points;
	VALUE retary;
	GLint order[2] = {0,0}; /* for GL_COEFF, [order] or [uorder,vorder] (MAP1/MAP2) */
	target = (GLenum)NUM2INT(arg1);
	query = (GLenum)NUM2INT(arg2);
	switch(target) {
		case GL_MAP1_INDEX:
		case GL_MAP1_TEXTURE_COORD_1: dims=1; pointsize=1; break;
		case GL_MAP1_TEXTURE_COORD_2: dims=1; pointsize=2; break;
		case GL_MAP1_VERTEX_3:
		case GL_MAP1_NORMAL: 
		case GL_MAP1_TEXTURE_COORD_3: dims=1; pointsize=3; break;
		case GL_MAP1_COLOR_4: 
		case GL_MAP1_TEXTURE_COORD_4:
		case GL_MAP1_VERTEX_4: dims=1; pointsize=4; break;
		case GL_MAP2_INDEX:
		case GL_MAP2_TEXTURE_COORD_1: dims=2; pointsize=1; break;
		case GL_MAP2_TEXTURE_COORD_2: dims=2; pointsize=2; break;
		case GL_MAP2_VERTEX_3:
		case GL_MAP2_NORMAL: 
		case GL_MAP2_TEXTURE_COORD_3: dims=2; pointsize=3; break;
		case GL_MAP2_COLOR_4: 
		case GL_MAP2_TEXTURE_COORD_4:
		case GL_MAP2_VERTEX_4: dims=2; pointsize=4; break;
		default:
			rb_raise(rb_eArgError, "unknown target:%d",target);
			break; /* not reached */
	}
	switch(query) {
		case GL_ORDER: size = dims;	break;
		case GL_DOMAIN: size = dims*2; break;
		case GL_COEFF:
			glGetMapiv(target,GL_ORDER,order);
			if (dims==1)
				size = order[0] * pointsize;
			else
				size = (order[0]*order[1]) * pointsize;
			break;
		default:
			rb_raise(rb_eArgError, "unknown target:%d",target);
			break; /* not reached */
	}
	points = ALLOC_N(GLfloat,size);
	glGetMapfv(target,query,points);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(points[i]));
	xfree(points);
	return retary;
}

static VALUE
gl_GetMapiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum query;
	int dims;
	int pointsize;
	int size=0;
	int i;
	GLint *points;
	VALUE retary;
	GLint order[2] = {0,0}; /* for GL_COEFF, [order] or [uorder,vorder] (MAP1/MAP2) */
	target = (GLenum)NUM2INT(arg1);
	query = (GLenum)NUM2INT(arg2);
	switch(target) {
		case GL_MAP1_INDEX:
		case GL_MAP1_TEXTURE_COORD_1: dims=1; pointsize=1; break;
		case GL_MAP1_TEXTURE_COORD_2: dims=1; pointsize=2; break;
		case GL_MAP1_VERTEX_3:
		case GL_MAP1_NORMAL: 
		case GL_MAP1_TEXTURE_COORD_3: dims=1; pointsize=3; break;
		case GL_MAP1_COLOR_4: 
		case GL_MAP1_TEXTURE_COORD_4:
		case GL_MAP1_VERTEX_4: dims=1; pointsize=4; break;
		case GL_MAP2_INDEX:
		case GL_MAP2_TEXTURE_COORD_1: dims=2; pointsize=1; break;
		case GL_MAP2_TEXTURE_COORD_2: dims=2; pointsize=2; break;
		case GL_MAP2_VERTEX_3:
		case GL_MAP2_NORMAL: 
		case GL_MAP2_TEXTURE_COORD_3: dims=2; pointsize=3; break;
		case GL_MAP2_COLOR_4: 
		case GL_MAP2_TEXTURE_COORD_4:
		case GL_MAP2_VERTEX_4: dims=2; pointsize=4; break;
		default:
			rb_raise(rb_eArgError, "unknown target:%d",target);
			break; /* not reached */
	}
	switch(query) {
		case GL_ORDER: size = dims;	break;
		case GL_DOMAIN: size = dims*2; break;
		case GL_COEFF:
			glGetMapiv(target,GL_ORDER,order);
			if (dims==1)
				size = order[0] * pointsize;
			else
				size = (order[0]*order[1]) * pointsize;
			break;
		default:
			rb_raise(rb_eArgError, "unknown target:%d",target);
			break; /* not reached */
	}
	points = ALLOC_N(GLint,size);
	glGetMapiv(target,query,points);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(points[i]));
	xfree(points);
	return retary;
}

static VALUE
gl_GetMaterialfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum face;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	int size;
	VALUE retary;
	int i;
	face = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_AMBIENT:
		case GL_DIFFUSE:
		case GL_SPECULAR:
		case GL_EMISSION:
			size = 4;
			break;
		case GL_COLOR_INDEXES:
			size = 3;
			break;
		case GL_SHININESS:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetMaterialfv(face,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static VALUE
gl_GetMaterialiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum face;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	int size;
	VALUE retary;
	int i;
	face = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_AMBIENT:
		case GL_DIFFUSE:
		case GL_SPECULAR:
		case GL_EMISSION:
			size = 4;
			break;
		case GL_COLOR_INDEXES:
			size = 3;
			break;
		case GL_SHININESS:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetMaterialiv(face,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}

static VALUE
gl_GetPixelMapfv(obj,arg1)
VALUE obj,arg1;
{
	GLenum map;
	GLenum map_size;
	GLint size = 0;
	GLfloat *values;
	VALUE retary;
	int i;
	map = (GLenum)NUM2INT(arg1);
	switch(map) {
		case GL_PIXEL_MAP_I_TO_I: map_size=GL_PIXEL_MAP_I_TO_I_SIZE; break;
		case GL_PIXEL_MAP_S_TO_S: map_size=GL_PIXEL_MAP_S_TO_S_SIZE; break;
		case GL_PIXEL_MAP_I_TO_R: map_size=GL_PIXEL_MAP_I_TO_R_SIZE; break;
		case GL_PIXEL_MAP_I_TO_G: map_size=GL_PIXEL_MAP_I_TO_G_SIZE; break;
		case GL_PIXEL_MAP_I_TO_B: map_size=GL_PIXEL_MAP_I_TO_B_SIZE; break;
		case GL_PIXEL_MAP_I_TO_A: map_size=GL_PIXEL_MAP_I_TO_A_SIZE; break;
		case GL_PIXEL_MAP_R_TO_R: map_size=GL_PIXEL_MAP_R_TO_R_SIZE; break;
		case GL_PIXEL_MAP_G_TO_G: map_size=GL_PIXEL_MAP_G_TO_G_SIZE; break;
		case GL_PIXEL_MAP_B_TO_B: map_size=GL_PIXEL_MAP_B_TO_B_SIZE; break;
		case GL_PIXEL_MAP_A_TO_A: map_size=GL_PIXEL_MAP_A_TO_A_SIZE; break;
		default:
			rb_raise(rb_eArgError, "unknown map:%d",map);
			break; /* not reached */
	}
	glGetIntegerv(map_size,&size);
	values = ALLOC_N(GLfloat,size);
	glGetPixelMapfv(map,values);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(values[i]));
	xfree(values);
	return retary;
}

static VALUE
gl_GetPixelMapuiv(obj,arg1)
VALUE obj,arg1;
{
	GLenum map;
	GLenum map_size;
	GLint size = 0;
	GLuint *values;
	VALUE retary;
	int i;
	map = (GLenum)NUM2INT(arg1);
	switch(map) {
		case GL_PIXEL_MAP_I_TO_I: map_size=GL_PIXEL_MAP_I_TO_I_SIZE; break;
		case GL_PIXEL_MAP_S_TO_S: map_size=GL_PIXEL_MAP_S_TO_S_SIZE; break;
		case GL_PIXEL_MAP_I_TO_R: map_size=GL_PIXEL_MAP_I_TO_R_SIZE; break;
		case GL_PIXEL_MAP_I_TO_G: map_size=GL_PIXEL_MAP_I_TO_G_SIZE; break;
		case GL_PIXEL_MAP_I_TO_B: map_size=GL_PIXEL_MAP_I_TO_B_SIZE; break;
		case GL_PIXEL_MAP_I_TO_A: map_size=GL_PIXEL_MAP_I_TO_A_SIZE; break;
		case GL_PIXEL_MAP_R_TO_R: map_size=GL_PIXEL_MAP_R_TO_R_SIZE; break;
		case GL_PIXEL_MAP_G_TO_G: map_size=GL_PIXEL_MAP_G_TO_G_SIZE; break;
		case GL_PIXEL_MAP_B_TO_B: map_size=GL_PIXEL_MAP_B_TO_B_SIZE; break;
		case GL_PIXEL_MAP_A_TO_A: map_size=GL_PIXEL_MAP_A_TO_A_SIZE; break;
		default:
			rb_raise(rb_eArgError, "unknown map:%d",map);
			break; /* not reached */
	}
	glGetIntegerv(map_size,&size);
	values = ALLOC_N(GLuint,size);
	glGetPixelMapuiv(map,values);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(values[i]));
	xfree(values);
	return retary;
}

static VALUE
gl_GetPixelMapusv(obj,arg1)
VALUE obj,arg1;
{
	GLenum map;
	GLenum map_size;
	GLint size = 0;
	GLushort *values;
	VALUE retary;
	int i;
	map = (GLenum)NUM2INT(arg1);
	switch(map) {
		case GL_PIXEL_MAP_I_TO_I: map_size=GL_PIXEL_MAP_I_TO_I_SIZE; break;
		case GL_PIXEL_MAP_S_TO_S: map_size=GL_PIXEL_MAP_S_TO_S_SIZE; break;
		case GL_PIXEL_MAP_I_TO_R: map_size=GL_PIXEL_MAP_I_TO_R_SIZE; break;
		case GL_PIXEL_MAP_I_TO_G: map_size=GL_PIXEL_MAP_I_TO_G_SIZE; break;
		case GL_PIXEL_MAP_I_TO_B: map_size=GL_PIXEL_MAP_I_TO_B_SIZE; break;
		case GL_PIXEL_MAP_I_TO_A: map_size=GL_PIXEL_MAP_I_TO_A_SIZE; break;
		case GL_PIXEL_MAP_R_TO_R: map_size=GL_PIXEL_MAP_R_TO_R_SIZE; break;
		case GL_PIXEL_MAP_G_TO_G: map_size=GL_PIXEL_MAP_G_TO_G_SIZE; break;
		case GL_PIXEL_MAP_B_TO_B: map_size=GL_PIXEL_MAP_B_TO_B_SIZE; break;
		case GL_PIXEL_MAP_A_TO_A: map_size=GL_PIXEL_MAP_A_TO_A_SIZE; break;
		default:
			rb_raise(rb_eArgError, "unknown map:%d",map);
			break; /* not reached */
	}
	glGetIntegerv(map_size,&size);
	values = ALLOC_N(GLushort,size);
	glGetPixelMapusv(map,values);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(values[i]));
	xfree(values);
	return retary;
}

static VALUE
gl_GetPolygonStipple(obj)
VALUE obj;
{
	GLubyte mask[128];
	memset(mask, 0x0, sizeof(GLubyte)*128);
	FORCE_PIXEL_STORE_MODE
	glGetPolygonStipple(mask);
	RESTORE_PIXEL_STORE_MODE
	return rb_str_new((const char*)mask, 128);
}

static VALUE
gl_GetString(obj,arg1)
VALUE obj,arg1;
{
	GLenum name;
	const GLubyte *ret;
	name = (GLenum)NUM2INT(arg1);
	ret = glGetString(name);
	return rb_str_new2((const char*)ret);
}

static VALUE
gl_GetTexEnvfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	int size;
	VALUE retary;
	int i;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_TEXTURE_ENV_COLOR:
			size = 4;
			break;
		case GL_TEXTURE_ENV_MODE:
		case GL_COORD_REPLACE:
		case GL_TEXTURE_LOD_BIAS:
		case GL_COMBINE_RGB:
		case GL_COMBINE_ALPHA:
		case GL_SRC0_RGB:
		case GL_SRC1_RGB:
		case GL_SRC2_RGB:
		case GL_SRC0_ALPHA:
		case GL_SRC1_ALPHA:
		case GL_SRC2_ALPHA:
		case GL_OPERAND0_RGB:
		case GL_OPERAND1_RGB:
		case GL_OPERAND2_RGB:
		case GL_OPERAND0_ALPHA:
		case GL_OPERAND1_ALPHA:
		case GL_OPERAND2_ALPHA:
		case GL_RGB_SCALE:
		case GL_ALPHA_SCALE:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetTexEnvfv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static VALUE
gl_GetTexEnviv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	int size;
	VALUE retary;
	int i;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_TEXTURE_ENV_COLOR:
			size = 4;
			break;
		case GL_TEXTURE_ENV_MODE:
		case GL_COORD_REPLACE:
		case GL_TEXTURE_LOD_BIAS:
		case GL_COMBINE_RGB:
		case GL_COMBINE_ALPHA:
		case GL_SRC0_RGB:
		case GL_SRC1_RGB:
		case GL_SRC2_RGB:
		case GL_SRC0_ALPHA:
		case GL_SRC1_ALPHA:
		case GL_SRC2_ALPHA:
		case GL_OPERAND0_RGB:
		case GL_OPERAND1_RGB:
		case GL_OPERAND2_RGB:
		case GL_OPERAND0_ALPHA:
		case GL_OPERAND1_ALPHA:
		case GL_OPERAND2_ALPHA:
		case GL_RGB_SCALE:
		case GL_ALPHA_SCALE:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetTexEnviv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}

static VALUE
gl_GetTexGendv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum coord;
	GLenum pname;
	GLdouble params[4] = {0.0,0.0,0.0,0.0};
	int size;
	VALUE retary;
	int i;
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_OBJECT_PLANE:
		case GL_EYE_PLANE:
			size = 4;
			break;
		case GL_TEXTURE_GEN_MODE:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetTexGendv(coord,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static VALUE
gl_GetTexGenfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum coord;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	int size;
	VALUE retary;
	int i;
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_OBJECT_PLANE:
		case GL_EYE_PLANE:
			size = 4;
			break;
		case GL_TEXTURE_GEN_MODE:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetTexGenfv(coord,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static VALUE
gl_GetTexGeniv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum coord;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	int size;
	VALUE retary;
	int i;
	coord = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_OBJECT_PLANE:
		case GL_EYE_PLANE:
			size = 4;
			break;
		case GL_TEXTURE_GEN_MODE:
			size = 1;
			break;
		default:
			rb_raise(rb_eArgError, "unknown pname:%d",pname);
			break; /* not reached */
	}
	glGetTexGeniv(coord,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}


static VALUE
gl_GetTexImage(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum tex;
	GLint lod;
	GLenum format;
	GLenum type;
	GLint width = 0;
	GLint height = 0;
	GLint depth = 0;
	GLint size;
	GLint format_size;
	GLint type_size;
	VALUE pixels;
	tex = (GLenum)NUM2INT(arg1);
	lod = (GLint)NUM2INT(arg2);
	format = (GLenum)NUM2INT(arg3);
	type = (GLenum)NUM2INT(arg4);
	format_size = glformat_size(format);
	type_size = gltype_size(type);
	if (type_size == -1 && format_size == -1)
		return Qnil;

	size = 1;
	switch(tex) {
		case GL_TEXTURE_3D:
			glGetTexLevelParameteriv(tex,lod,GL_TEXTURE_DEPTH,&depth);
			size *= depth;
			/* fall through */
		case GL_TEXTURE_2D:
			glGetTexLevelParameteriv(tex,lod,GL_TEXTURE_HEIGHT,&height);
			size *= height;
			/* fall through */
		case GL_TEXTURE_1D:
			glGetTexLevelParameteriv(tex,lod,GL_TEXTURE_WIDTH,&width);
			size *= width;
			break;
		default:
			return Qnil;
	}
	size *=	format_size*type_size;
	pixels = allocate_buffer_with_string(size);

	FORCE_PIXEL_STORE_MODE
	glGetTexImage(tex,lod,format,type,(GLvoid*)RSTRING(pixels)->ptr);
	RESTORE_PIXEL_STORE_MODE
	return pixels;
}


static VALUE
gl_GetTexParameterfv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLfloat params[4] = {0.0,0.0,0.0,0.0};
	int size;
	VALUE retary;
	int i;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_TEXTURE_BORDER_COLOR:
			size = 4;
			break;
		default:
			size = 1;
			break;
	}
	glGetTexParameterfv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, rb_float_new(params[i]));
	return retary;
}

static VALUE
gl_GetTexParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params[4] = {0,0,0,0};
	int size;
	VALUE retary;
	int i;
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	switch(pname) {
		case GL_TEXTURE_BORDER_COLOR:
			size = 4;
			break;
		default:
			size = 1;
			break;
	}
	glGetTexParameteriv(target,pname,params);
	retary = rb_ary_new2(size);
	for(i=0;i<size;i++)
		rb_ary_push(retary, INT2NUM(params[i]));
	return retary;
}

static VALUE
gl_GetTexLevelParameterfv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLint level;
	GLenum pname;
	GLfloat params = 0.0; 
	VALUE retary;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	pname = (GLenum)NUM2INT(arg3);
	glGetTexLevelParameterfv(target,level,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, rb_float_new(params));
	return retary;
}

static VALUE
gl_GetTexLevelParameteriv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLint level;
	GLenum pname;
	GLint params = 0; 
	VALUE retary;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	pname = (GLenum)NUM2INT(arg3);
	glGetTexLevelParameteriv(target,level,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, INT2NUM(params));
	return retary;
}

static VALUE
gl_IsEnabled(obj,arg1)
VALUE obj,arg1;
{
	GLenum cap;
	GLboolean ret;
	cap = (GLenum)NUM2INT(arg1);
	ret = glIsEnabled(cap);
	return INT2NUM(ret);
}

static VALUE
gl_IsList(obj,arg1)
VALUE obj,arg1;
{
	GLuint list;
	GLboolean ret;
	list = (GLuint)NUM2UINT(arg1);
	ret = glIsList(list);
	return INT2NUM(ret);
}

static VALUE
gl_DepthRange(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLclampd near_val;
	GLclampd far_val;
	near_val = (GLclampd)NUM2DBL(arg1);
	far_val = (GLclampd)NUM2DBL(arg2);
	glDepthRange(near_val,far_val);
	return Qnil;
}

static VALUE
gl_Frustum(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLdouble left;
	GLdouble right;
	GLdouble bottom;
	GLdouble top;
	GLdouble near_val;
	GLdouble far_val;
	left = (GLdouble)NUM2DBL(arg1);
	right = (GLdouble)NUM2DBL(arg2);
	bottom = (GLdouble)NUM2DBL(arg3);
	top = (GLdouble)NUM2DBL(arg4);
	near_val = (GLdouble)NUM2DBL(arg5);
	far_val = (GLdouble)NUM2DBL(arg6);
	glFrustum(left,right,bottom,top,near_val,far_val);
	return Qnil;
}

static VALUE
gl_LoadIdentity(obj)
VALUE obj;
{
	glLoadIdentity();
	return Qnil;
}

static VALUE
gl_LoadMatrixf(obj,arg1)
VALUE obj,arg1;
{
	GLfloat m[4*4];
	ary2cmat4x4flt(arg1, m);
	glLoadMatrixf(m);
	return Qnil;
}

static VALUE
gl_LoadMatrixd(obj,arg1)
VALUE obj,arg1;
{
	GLdouble m[4*4];
	ary2cmat4x4dbl(arg1, m);
	glLoadMatrixd(m);
	return Qnil;
}

static VALUE
gl_MatrixMode(obj,arg1)
VALUE obj,arg1;
{
	GLenum mode;
	mode = (GLenum)NUM2INT(arg1);
	glMatrixMode(mode);
	return Qnil;
}

static VALUE
gl_MultMatrixf(obj,arg1)
VALUE obj,arg1;
{
	GLfloat m[4*4];
	ary2cmat4x4flt(arg1, m);
	glMultMatrixf(m);
	return Qnil;
}

static VALUE
gl_MultMatrixd(obj,arg1)
VALUE obj,arg1;
{
	GLdouble m[4*4];
	ary2cmat4x4dbl(arg1, m);
	glMultMatrixd(m);
	return Qnil;
}

static VALUE
gl_Ortho(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLdouble left;
	GLdouble right;
	GLdouble bottom;
	GLdouble top;
	GLdouble near_val;
	GLdouble far_val;
	left = (GLdouble)NUM2DBL(arg1);
	right = (GLdouble)NUM2DBL(arg2);
	bottom = (GLdouble)NUM2DBL(arg3);
	top = (GLdouble)NUM2DBL(arg4);
	near_val = (GLdouble)NUM2DBL(arg5);
	far_val = (GLdouble)NUM2DBL(arg6);
	glOrtho(left,right,bottom,top,near_val,far_val);
	return Qnil;
}

static VALUE
gl_PopMatrix(obj)
VALUE obj;
{
	glPopMatrix();
	return Qnil;
}

static VALUE
gl_PushMatrix(obj)
VALUE obj;
{
	glPushMatrix();
	return Qnil;
}

static VALUE
gl_Rotated(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLdouble angle;
	GLdouble x;
	GLdouble y;
	GLdouble z;
	angle = (GLdouble)NUM2DBL(arg1);
	x = (GLdouble)NUM2DBL(arg2);
	y = (GLdouble)NUM2DBL(arg3);
	z = (GLdouble)NUM2DBL(arg4);
	glRotated(angle,x,y,z);
	return Qnil;
}

static VALUE
gl_Rotatef(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLfloat angle;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	angle = (GLfloat)NUM2DBL(arg1);
	x = (GLfloat)NUM2DBL(arg2);
	y = (GLfloat)NUM2DBL(arg3);
	z = (GLfloat)NUM2DBL(arg4);
	glRotatef(angle,x,y,z);
	return Qnil;
}

static VALUE
gl_Scaled(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	glScaled(x,y,z);
	return Qnil;
}

static VALUE
gl_Scalef(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	glScalef(x,y,z);
	return Qnil;
}

static VALUE
gl_Translated(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	x = (GLdouble)NUM2DBL(arg1);
	y = (GLdouble)NUM2DBL(arg2);
	z = (GLdouble)NUM2DBL(arg3);
	glTranslated(x,y,z);
	return Qnil;
}

static VALUE
gl_Translatef(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	x = (GLfloat)NUM2DBL(arg1);
	y = (GLfloat)NUM2DBL(arg2);
	z = (GLfloat)NUM2DBL(arg3);
	glTranslatef(x,y,z);
	return Qnil;
}

static VALUE
gl_Viewport(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	x = (GLint)NUM2INT(arg1);
	y = (GLint)NUM2INT(arg2);
	width = (GLsizei)NUM2UINT(arg3);
	height = (GLsizei)NUM2UINT(arg4);
	glViewport(x,y,width,height);
	return Qnil;
}

static VALUE
gl_ArrayElement(obj,arg1)
VALUE obj,arg1;
{
	GLint i;
	i = (GLint)NUM2INT(arg1);
	glArrayElement(i);
	return Qnil;
}

static VALUE g_Vertex_ptr;
static VALUE g_Normal_ptr;
static VALUE g_Color_ptr; 
static VALUE g_Index_ptr; 
static VALUE g_TexCoord_ptr; 
static VALUE g_EdgeFlag_ptr; 
VALUE g_FogCoord_ptr; /* OpenGL 1.4 */
VALUE g_SecondaryColor_ptr; /* OpenGL 1.4 */
#define POINTER_FUNC(_func_) \
static VALUE \
gl_##_func_##Pointer(obj, arg1, arg2, arg3, arg4) \
VALUE obj, arg1, arg2, arg3, arg4; \
{ \
	GLint size; \
	GLenum type; \
	GLsizei stride; \
	size = (GLint)NUM2INT(arg1); \
	type = (GLenum)NUM2INT(arg2); \
	stride = (GLsizei)NUM2UINT(arg3); \
	Check_Type(arg4, T_STRING); \
	rb_str_freeze(arg4); \
	g_##_func_##_ptr = arg4; \
	gl##_func_##Pointer(size, type, stride, (const GLvoid*)RSTRING(arg4)->ptr); \
	return Qnil; \
}

POINTER_FUNC(Vertex)
POINTER_FUNC(Color)
POINTER_FUNC(TexCoord)

#undef POINTER_FUNC

static VALUE
gl_DisableClientState(obj,arg1)
VALUE obj,arg1;
{
	GLenum cap;
	cap = (GLenum)NUM2INT(arg1);
	glDisableClientState(cap);
	return Qnil;
}

static VALUE
gl_DrawArrays(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum mode;
	GLint first;
	GLsizei count;
	mode = (GLenum)NUM2INT(arg1);
	first = (GLint)NUM2INT(arg2);
	count = (GLsizei)NUM2UINT(arg3);
	glDrawArrays(mode,first,count);
	return Qnil;
}

static VALUE
gl_DrawElements(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum mode;
	GLsizei count;
	GLenum type;
	mode = (GLenum)NUM2INT(arg1);
	count = (GLsizei)NUM2UINT(arg2);
	type = (GLenum)NUM2INT(arg3);
	Check_Type(arg4, T_STRING);
	glDrawElements(mode, count, type, (const GLvoid*)RSTRING(arg4)->ptr);
	return Qnil;
}

static VALUE
gl_EdgeFlagPointer(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLsizei stride;
	stride = (GLsizei)NUM2UINT(arg1);
	Check_Type(arg2, T_STRING);
	rb_str_freeze(arg2);
	g_EdgeFlag_ptr = arg2;
	glEdgeFlagPointer(stride, (const GLboolean*)RSTRING(arg2)->ptr);
	return Qnil;
}

static VALUE
gl_EnableClientState(obj,arg1)
VALUE obj,arg1;
{
	GLenum cap;
	cap = (GLenum)NUM2INT(arg1);
	glEnableClientState(cap);
	return Qnil;
}

static VALUE
gl_GetPointerv(obj,arg1)
VALUE obj,arg1;
{
	GLenum pname;
	pname = (GLenum)NUM2INT(arg1);
	switch (pname) {
		case GL_VERTEX_ARRAY_POINTER: 
			return g_Vertex_ptr;
		case GL_NORMAL_ARRAY_POINTER: 
			return g_Normal_ptr;
		case GL_COLOR_ARRAY_POINTER: 
			return g_Color_ptr;
		case GL_INDEX_ARRAY_POINTER: 
			return g_Index_ptr;
		case GL_TEXTURE_COORD_ARRAY_POINTER: 
			return g_TexCoord_ptr;
		case GL_EDGE_FLAG_ARRAY_POINTER: 
			return g_EdgeFlag_ptr;
		case GL_FOG_COORD_ARRAY_POINTER:
			return g_FogCoord_ptr;
		case GL_SECONDARY_COLOR_ARRAY_POINTER:
			return g_SecondaryColor_ptr;
		case GL_FEEDBACK_BUFFER_POINTER:
			return g_current_feed_buffer;
		case GL_SELECTION_BUFFER_POINTER:
			return g_current_sel_buffer;			
		default:
			rb_raise(rb_eArgError, "Invalid pname %d",pname);
			break; /* not reached */
	}
	return Qnil;
}

static VALUE
gl_IndexPointer(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum type;
	GLsizei stride;
	type = (GLenum)NUM2INT(arg1);
	stride = (GLsizei)NUM2UINT(arg2);
	Check_Type(arg3, T_STRING);
	rb_str_freeze(arg3);
	g_Index_ptr = arg3;
	glIndexPointer(type, stride, (const GLvoid*)RSTRING(arg3)->ptr);
	return Qnil;
}

static VALUE
gl_InterleavedArrays(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum format;
	GLsizei stride;
	format = (GLenum)NUM2INT(arg1);
	stride = (GLsizei)NUM2UINT(arg2);
	Check_Type(arg3, T_STRING);
	glInterleavedArrays(format, stride, (const GLvoid*)RSTRING(arg3)->ptr);
	return Qnil;
}

static VALUE
gl_NormalPointer(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum type;
	GLsizei stride;
	type = (GLenum)NUM2INT(arg1);
	stride = (GLsizei)NUM2UINT(arg2);
	Check_Type(arg3, T_STRING);
	rb_str_freeze(arg3);
	g_Normal_ptr = arg3;
	glNormalPointer(type, stride, (const GLvoid*)RSTRING(arg3)->ptr);
	return Qnil;
}

static VALUE
gl_PolygonOffset(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLfloat factor;
	GLfloat units;
	factor = (GLfloat)NUM2DBL(arg1);
	units = (GLfloat)NUM2DBL(arg2);
	glPolygonOffset(factor,units);
	return Qnil;
}

static VALUE
gl_CopyTexImage1D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7;
{
	GLenum target;
	GLint level;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	GLint border;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	internalformat = (GLenum)NUM2INT(arg3);
	x = (GLint)NUM2INT(arg4);
	y = (GLint)NUM2INT(arg5);
	width = (GLsizei)NUM2UINT(arg6);
	border = (GLint)NUM2INT(arg7);
	glCopyTexImage1D(target,level,internalformat,x,y,width,border);
	return Qnil;
}

static VALUE
gl_CopyTexImage2D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8;
{
	GLenum target;
	GLint level;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLint border;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	internalformat = (GLenum)NUM2INT(arg3);
	x = (GLint)NUM2INT(arg4);
	y = (GLint)NUM2INT(arg5);
	width = (GLsizei)NUM2UINT(arg6);
	height = (GLsizei)NUM2UINT(arg7);
	border = (GLint)NUM2INT(arg8);
	glCopyTexImage2D(target,level,internalformat,x,y,width,height,border);
	return Qnil;
}

static VALUE
gl_CopyTexSubImage1D(obj,arg1,arg2,arg3,arg4,arg5,arg6)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6;
{
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint x;
	GLint y;
	GLsizei width;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	xoffset = (GLint)NUM2INT(arg3);
	x = (GLint)NUM2INT(arg4);
	y = (GLint)NUM2INT(arg5);
	width = (GLsizei)NUM2UINT(arg6);
	glCopyTexSubImage1D(target,level,xoffset,x,y,width);
	return Qnil;
}

static VALUE
gl_CopyTexSubImage2D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8;
{
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	xoffset = (GLint)NUM2INT(arg3);
	yoffset = (GLint)NUM2INT(arg4);
	x = (GLint)NUM2INT(arg5);
	y = (GLint)NUM2INT(arg6);
	width = (GLsizei)NUM2UINT(arg7);
	height = (GLsizei)NUM2UINT(arg8);
	glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height);
	return Qnil;
}

static VALUE
gl_TexSubImage1D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7;
{
	GLenum target;
	GLint level;
	GLint xoffset;
	GLsizei width;
	GLenum format;
	GLenum type;
	const char *pixels;
	int size;
	int type_size;
	int format_size;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	xoffset = (GLint)NUM2INT(arg3);
	width = (GLsizei)NUM2UINT(arg4);
	format = (GLenum)NUM2INT(arg5);
	type = (GLenum)NUM2INT(arg6);
	type_size = gltype_size(type);
	format_size = glformat_size(format);
	if (type_size == -1 || format_size == -1)
		return Qnil;

	if (type==GL_BITMAP)
		size = format_size*(width/8);
	else
		size = type_size*format_size*width;

	if (TYPE(arg7) == T_STRING) {
		if (RSTRING(arg7)->len < size)
			rb_raise(rb_eArgError, "string length:%d",RSTRING(arg7)->len);
		 pixels = RSTRING(arg7)->ptr;
	} else if (TYPE(arg7) == T_FIXNUM || TYPE(arg7) == T_BIGNUM) { /* arg7 is offset to unpack buffer */
			pixels = (const char *)NUM2UINT(arg7);
	} else {
		Check_Type(arg7,T_STRING); /* force exception */
		return Qnil;
	}
	glTexSubImage1D(target,level,xoffset,width,format,type,pixels);
	return Qnil;
}

static VALUE
gl_TexSubImage2D(obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)
VALUE obj,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9;
{
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	const char *pixels;
	int size;
	int type_size;
	int format_size;
	target = (GLenum)NUM2INT(arg1);
	level = (GLint)NUM2INT(arg2);
	xoffset = (GLint)NUM2INT(arg3);
	yoffset = (GLint)NUM2INT(arg4);
	width = (GLsizei)NUM2UINT(arg5);
	height = (GLsizei)NUM2UINT(arg6);
	format = (GLenum)NUM2INT(arg7);
	type = (GLenum)NUM2INT(arg8);
	type_size = gltype_size(type);
	format_size = glformat_size(format);
	if (type_size == -1 || format_size == -1)
		return Qnil;
	if (type==GL_BITMAP)
		size = format_size*((height*width)/8);
	else
		size = type_size*format_size*height*width;
	if (TYPE(arg9) == T_STRING) {
		if (RSTRING(arg9)->len < size)
			rb_raise(rb_eArgError, "string length:%d",RSTRING(arg9)->len);
		 pixels = RSTRING(arg9)->ptr;
	} else if (TYPE(arg9) == T_FIXNUM || TYPE(arg9) == T_BIGNUM) { /* arg9 is offset to unpack buffer */
			pixels = (const char *)NUM2UINT(arg9);
	} else {
		Check_Type(arg9,T_STRING); /* force exception */
		return Qnil;
	}
	glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels);
	return Qnil;
}

static VALUE
gl_AreTexturesResident(obj,arg1)
VALUE obj,arg1;
{
	GLuint *textures;
	GLboolean *residences;
	GLsizei size;
	GLboolean r;
	VALUE retary;
	int i;
	Check_Type(arg1,T_ARRAY);
	size = RARRAY(arg1)->len;
	textures = ALLOC_N(GLuint,size);
	residences = ALLOC_N(GLboolean,size);
	ary2cuint(arg1,textures,size);	
	r = glAreTexturesResident(size,textures,residences);
	retary = rb_ary_new2(size);
	if (r==GL_TRUE) { /* all are resident */
		for(i=0;i<size;i++)
			rb_ary_push(retary, INT2NUM(GL_TRUE));
	} else {
		for(i=0;i<size;i++)
			rb_ary_push(retary, INT2NUM(residences[i]));
	}
	xfree(textures);
	xfree(residences);
	return retary;
}

static VALUE
gl_BindTexture(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLuint texture;
	target = (GLenum)NUM2INT(arg1);
	texture = (GLuint)NUM2UINT(arg2);
	glBindTexture(target,texture);
	return Qnil;
}

static VALUE
gl_DeleteTextures(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLuint *textures;
	Check_Type(arg1,T_ARRAY);
	n = RARRAY(arg1)->len;
	textures = ALLOC_N(GLuint,n);
	ary2cuint(arg1,textures,n); 
	glDeleteTextures( n, textures);
	xfree(textures);
	return Qnil;
}

static VALUE
gl_GenTextures(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLuint *textures;
	RArray *ret;
	int i;
	n = (GLsizei)NUM2UINT(arg1);
	textures = ALLOC_N(GLuint, n);
	glGenTextures(n, textures);
	ret = RARRAY(rb_ary_new2(n));
	for (i = 0; i < n; i++)
		rb_ary_push((VALUE)ret, INT2NUM(textures[i]));
	xfree(textures);
	return (VALUE)ret;
}

static VALUE
gl_IsTexture(obj,arg1)
VALUE obj,arg1;
{
	GLuint texture;
	GLboolean ret;
	texture = (GLuint)NUM2UINT(arg1);
	ret = glIsTexture(texture);
	return INT2NUM(ret);
}

static VALUE
gl_PrioritizeTextures(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint *textures;
	GLclampf *priorities;
	GLsizei size;
	Check_Type(arg1,T_ARRAY);
	Check_Type(arg2,T_ARRAY);
	if ((size = RARRAY(arg1)->len) != RARRAY(arg2)->len)
		rb_raise(rb_eArgError, "passed arrays must have the same length");
	textures = ALLOC_N(GLuint,size);
	priorities = ALLOC_N(GLclampf,size);
	ary2cuint(arg1,textures,size);	
	ary2cflt(arg2,priorities,size);	
	glPrioritizeTextures(size,textures,priorities);
	xfree(textures);
	xfree(priorities);
	return Qnil;
}

static VALUE
gl_Indexub(obj,arg1)
VALUE obj,arg1;
{
	GLubyte c;
	c = (GLubyte)NUM2INT(arg1);
	glIndexub(c);
	return Qnil;
}

static VALUE
gl_Indexubv(obj,arg1)
VALUE obj,arg1;
{
	GLubyte c[1] = {0};
	Check_Type(arg1,T_ARRAY);
	ary2cubyte(arg1,c,1);
	glIndexubv(c);
	return Qnil;
}

static VALUE
gl_PopClientAttrib(obj)
VALUE obj;
{
	glPopClientAttrib();
	return Qnil;
}

static VALUE
gl_PushClientAttrib(obj,arg1)
VALUE obj,arg1;
{
	GLbitfield mask;
	mask = (GLbitfield)NUM2UINT(arg1);
	glPushClientAttrib(mask);
	return Qnil;
}

/* additional functions */

#define GLCOLOR_VFUNC(_type_) \
static VALUE \
gl_Color##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[4]; \
	RArray *ary; \
	switch (num = rb_scan_args(argc, argv, "13", &args[0], &args[1], &args[2], &args[3])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 3: \
			gl_Color3##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2]); \
			break; \
			case 4: \
			gl_Color4##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2],ary->ptr[3]); \
			break; \
			default: \
			rb_raise(rb_eArgError, "array length:%d", ary->len); \
		} \
		} \
		else \
			Check_Type(args[0],T_ARRAY); /* force exception */ \
		break; \
	case 3: \
		gl_Color3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	case 4: \
		gl_Color4##_type_(obj,args[0], args[1], args[2], args[3]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "arg length:%d", num); \
	} \
	return Qnil; \
}

GLCOLOR_VFUNC(b)
GLCOLOR_VFUNC(d)
GLCOLOR_VFUNC(f)
GLCOLOR_VFUNC(i)
GLCOLOR_VFUNC(s)
GLCOLOR_VFUNC(ub)
GLCOLOR_VFUNC(ui)
GLCOLOR_VFUNC(us)
#undef GLCOLOR_VFUNC

#define GLNORMAL_VFUNC(_type_) \
static VALUE \
gl_Normal##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[3]; \
	RArray* ary; \
	switch (num = rb_scan_args(argc, argv, "12", &args[0], &args[1], &args[2])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 3: \
			gl_Normal3##_type_(obj,ary->ptr[0], ary->ptr[1],ary->ptr[2]); \
			break; \
			default: \
			rb_raise(rb_eArgError, "array length:%d", ary->len); \
		} \
		} \
		else \
			Check_Type(args[0],T_ARRAY); /* force exception */ \
		break; \
	case 3: \
		gl_Normal3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "arg length:%d",num); \
	} \
	return Qnil; \
}

GLNORMAL_VFUNC(b)
GLNORMAL_VFUNC(d)
GLNORMAL_VFUNC(f)
GLNORMAL_VFUNC(i)
GLNORMAL_VFUNC(s)
#undef GLNORMAL_VFUNC

#define GLRASTERPOS_VFUNC(_type_) \
static VALUE \
gl_RasterPos##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[4]; \
	RArray *ary; \
	switch (num = rb_scan_args(argc, argv, "13", &args[0], &args[1], &args[2], &args[3])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 2: \
			gl_RasterPos2##_type_(obj,ary->ptr[0],ary->ptr[1]); \
			break; \
			case 3: \
			gl_RasterPos3##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2]); \
			break; \
			case 4: \
			gl_RasterPos4##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2],ary->ptr[3]); \
			break; \
			default: \
			rb_raise(rb_eArgError, "array length:%d", num); \
		} \
		} \
		else \
			Check_Type(args[0],T_ARRAY); /* force exception */ \
		break; \
	case 2: \
		gl_RasterPos2##_type_(obj,args[0], args[1]); \
		break; \
	case 3: \
		gl_RasterPos3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	case 4: \
		gl_RasterPos4##_type_(obj,args[0], args[1], args[2], args[3]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "too many arguments"); \
	} \
	return Qnil; \
}

GLRASTERPOS_VFUNC(d)
GLRASTERPOS_VFUNC(f)
GLRASTERPOS_VFUNC(i)
GLRASTERPOS_VFUNC(s)
#undef GLRASTERPOS_VFUNC

#define GLRECT_VFUNC(_type_) \
static VALUE \
gl_Rect##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[4]; \
	RArray *ary,*ary2; \
	switch (num = rb_scan_args(argc, argv, "22", &args[0], &args[1], &args[2], &args[3])) { \
	case 2: \
		if (TYPE(args[0]) == T_ARRAY && TYPE(args[1]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		ary2 = RARRAY(args[1]); \
		switch (ary->len) { \
			case 2: \
			gl_Rect##_type_(obj,ary->ptr[0],ary->ptr[1],ary2->ptr[0],ary2->ptr[1]); \
			break; \
			default: \
			rb_raise(rb_eArgError, "array length:%d", ary->len); \
		} \
		} \
		else \
			Check_Type(args[0],T_ARRAY); /* force exception */ \
		break; \
	case 4: \
		gl_Rect##_type_(obj,args[0], args[1], args[2], args[3]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "arg length:%d", num); \
	} \
	return Qnil; \
}

GLRECT_VFUNC(d)
GLRECT_VFUNC(f)
GLRECT_VFUNC(i)
GLRECT_VFUNC(s)
#undef GLRECT_VFUNC

#define GLTEXCOORD_VFUNC(_type_) \
static VALUE \
gl_TexCoord##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	int num; \
	VALUE args[4]; \
	RArray *ary; \
	switch (num = rb_scan_args(argc, argv, "13", &args[0], &args[1], &args[2], &args[3])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 1: \
			gl_TexCoord1##_type_(obj,ary->ptr[0]); \
			break; \
			case 2: \
			gl_TexCoord2##_type_(obj,ary->ptr[0],ary->ptr[1]); \
			break; \
			case 3: \
			gl_TexCoord3##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2]); \
			break; \
			case 4: \
			gl_TexCoord4##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2],ary->ptr[3]); \
			break; \
			default: \
			rb_raise(rb_eArgError, "array length:%d", num); \
		} \
		} \
		else \
			gl_TexCoord1##_type_(obj,args[0]); \
		break; \
	case 2: \
		gl_TexCoord2##_type_(obj,args[0], args[1]); \
		break; \
	case 3: \
		gl_TexCoord3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	case 4: \
		gl_TexCoord4##_type_(obj,args[0], args[1], args[2], args[3]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "too many arguments"); \
		break; \
	} \
	return Qnil; \
}

GLTEXCOORD_VFUNC(d)
GLTEXCOORD_VFUNC(f)
GLTEXCOORD_VFUNC(i)
GLTEXCOORD_VFUNC(s)
#undef GLTEXCOORD_VFUNC

#define GLVERTEX_VFUNC(_type_) \
static VALUE \
gl_Vertex##_type_##v(argc,argv,obj) \
int argc; \
VALUE *argv; \
VALUE obj; \
{ \
	VALUE args[4]; \
	RArray *ary; \
	switch (rb_scan_args(argc, argv, "13", &args[0], &args[1], &args[2], &args[3])) { \
	case 1: \
		if (TYPE(args[0]) == T_ARRAY) { \
		ary = RARRAY(args[0]); \
		switch (ary->len) { \
			case 2: \
			gl_Vertex2##_type_(obj,ary->ptr[0],ary->ptr[1]); \
			break; \
			case 3: \
			gl_Vertex3##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2]); \
			break; \
			case 4: \
			gl_Vertex4##_type_(obj,ary->ptr[0],ary->ptr[1],ary->ptr[2],ary->ptr[3]); \
			break; \
			default: \
			rb_raise(rb_eRuntimeError, "glVertex vertex num error!:%d", ary->len); \
		} \
		} \
		else \
			Check_Type(args[0],T_ARRAY); /* force exception */ \
		break; \
	case 2: \
		gl_Vertex2##_type_(obj,args[0], args[1]); \
		break; \
	case 3: \
		gl_Vertex3##_type_(obj,args[0], args[1], args[2]); \
		break; \
	case 4: \
		gl_Vertex4##_type_(obj,args[0], args[1], args[2], args[3]); \
		break; \
	default: \
		rb_raise(rb_eArgError, "too many arguments"); \
		break; \
	} \
	return Qnil; \
}

GLVERTEX_VFUNC(d)
GLVERTEX_VFUNC(f)
GLVERTEX_VFUNC(i)
GLVERTEX_VFUNC(s)
#undef GLVERTEX_VFUNC

/* */

static VALUE
gl_LightModel(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	if (TYPE(arg2) == T_ARRAY)
		gl_LightModelfv(obj,arg1,arg2);
	else
		gl_LightModelf(obj,arg1,arg2);

	return Qnil; 
}

static VALUE
gl_Material(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	if (TYPE(arg3) == T_ARRAY)
		gl_Materialfv(obj,arg1,arg2,arg3);
	else
		gl_Materialf(obj,arg1,arg2,arg3);

	return Qnil;
}

static VALUE
gl_Fog(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	if (TYPE(arg2) == T_ARRAY)
		gl_Fogfv(obj,arg1,arg2);
	else
		gl_Fogf(obj,arg1,arg2);

	return Qnil; 
}

static VALUE
gl_Light(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	if (TYPE(arg3) == T_ARRAY)
		gl_Lightfv(obj,arg1,arg2,arg3);
	else
		gl_Lightf(obj,arg1,arg2,arg3);

	return Qnil; 
}

static VALUE
gl_TexParameter(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	if (TYPE(arg3) == T_ARRAY)
		gl_TexParameterfv(obj,arg1,arg2,arg3);
	else
		gl_TexParameterf(obj,arg1,arg2,arg3);

	return Qnil; 
}

static VALUE
gl_TexEnv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	if (TYPE(arg3) == T_ARRAY)
		gl_TexEnvfv(obj,arg1,arg2,arg3);
	else
		gl_TexEnvf(obj,arg1,arg2,arg3);

	return Qnil; 
}

static VALUE
gl_TexGen(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	if (TYPE(arg3) == T_ARRAY)
		gl_TexGenfv(obj,arg1,arg2,arg3);
	else
		gl_TexGenf(obj,arg1,arg2,arg3);

	return Qnil; 
}

/* init */
void gl_init_functions_1_0__1_1(VALUE module)
{
	g_FogCoord_ptr = 0; /* for use in gl-1.4.c */
	g_SecondaryColor_ptr = 0; /* for use in gl-1.4.c */

	/* OpenGL 1.0 functions */
	rb_define_module_function(module, "glNewList", gl_NewList, 2);
	rb_define_module_function(module, "glEndList", gl_EndList, 0);
	rb_define_module_function(module, "glCallList", gl_CallList, 1);
	rb_define_module_function(module, "glCallLists", gl_CallLists, 1);
	rb_define_module_function(module, "glDeleteLists", gl_DeleteLists, 2);
	rb_define_module_function(module, "glGenLists", gl_GenLists, 1);
	rb_define_module_function(module, "glListBase", gl_ListBase, 1);
	rb_define_module_function(module, "glBegin", gl_Begin, 1);
	rb_define_module_function(module, "glBitmap", gl_Bitmap, 7);
	rb_define_module_function(module, "glColor3b", gl_Color3b, 3);
	rb_define_module_function(module, "glColor3d", gl_Color3d, 3);
	rb_define_module_function(module, "glColor3f", gl_Color3f, 3);
	rb_define_module_function(module, "glColor3i", gl_Color3i, 3);
	rb_define_module_function(module, "glColor3s", gl_Color3s, 3);
	rb_define_module_function(module, "glColor3ub", gl_Color3ub, 3);
	rb_define_module_function(module, "glColor3ui", gl_Color3ui, 3);
	rb_define_module_function(module, "glColor3us", gl_Color3us, 3);
	rb_define_module_function(module, "glColor4b", gl_Color4b, 4);
	rb_define_module_function(module, "glColor4d", gl_Color4d, 4);
	rb_define_module_function(module, "glColor4f", gl_Color4f, 4);
	rb_define_module_function(module, "glColor4i", gl_Color4i, 4);
	rb_define_module_function(module, "glColor4s", gl_Color4s, 4);
	rb_define_module_function(module, "glColor4ub", gl_Color4ub, 4);
	rb_define_module_function(module, "glColor4ui", gl_Color4ui, 4);
	rb_define_module_function(module, "glColor4us", gl_Color4us, 4);
	rb_define_module_function(module, "glEdgeFlag", gl_EdgeFlag, 1);
	rb_define_module_function(module, "glEdgeFlagv", gl_EdgeFlagv, 1);
	rb_define_module_function(module, "glEnd", gl_End, 0);
	rb_define_module_function(module, "glIndexd", gl_Indexd, 1);
	rb_define_module_function(module, "glIndexdv", gl_Indexdv, 1);
	rb_define_module_function(module, "glIndexf", gl_Indexf, 1);
	rb_define_module_function(module, "glIndexfv", gl_Indexfv, 1);
	rb_define_module_function(module, "glIndexi", gl_Indexi, 1);
	rb_define_module_function(module, "glIndexiv", gl_Indexiv, 1);
	rb_define_module_function(module, "glIndexs", gl_Indexs, 1);
	rb_define_module_function(module, "glIndexsv", gl_Indexsv, 1);
	rb_define_module_function(module, "glNormal3b", gl_Normal3b, 3);
	rb_define_module_function(module, "glNormal3d", gl_Normal3d, 3);
	rb_define_module_function(module, "glNormal3f", gl_Normal3f, 3);
	rb_define_module_function(module, "glNormal3i", gl_Normal3i, 3);
	rb_define_module_function(module, "glNormal3s", gl_Normal3s, 3);
	rb_define_module_function(module, "glRasterPos2d", gl_RasterPos2d, 2);
	rb_define_module_function(module, "glRasterPos2f", gl_RasterPos2f, 2);
	rb_define_module_function(module, "glRasterPos2i", gl_RasterPos2i, 2);
	rb_define_module_function(module, "glRasterPos2s", gl_RasterPos2s, 2);
	rb_define_module_function(module, "glRasterPos3d", gl_RasterPos3d, 3);
	rb_define_module_function(module, "glRasterPos3f", gl_RasterPos3f, 3);
	rb_define_module_function(module, "glRasterPos3i", gl_RasterPos3i, 3);
	rb_define_module_function(module, "glRasterPos3s", gl_RasterPos3s, 3);
	rb_define_module_function(module, "glRasterPos4d", gl_RasterPos4d, 4);
	rb_define_module_function(module, "glRasterPos4f", gl_RasterPos4f, 4);
	rb_define_module_function(module, "glRasterPos4i", gl_RasterPos4i, 4);
	rb_define_module_function(module, "glRasterPos4s", gl_RasterPos4s, 4);
	rb_define_module_function(module, "glRectd", gl_Rectd, 4);
	rb_define_module_function(module, "glRectf", gl_Rectf, 4);
	rb_define_module_function(module, "glRecti", gl_Recti, 4);
	rb_define_module_function(module, "glRects", gl_Rects, 4);
	rb_define_module_function(module, "glTexCoord1d", gl_TexCoord1d, 1);
	rb_define_module_function(module, "glTexCoord1f", gl_TexCoord1f, 1);
	rb_define_module_function(module, "glTexCoord1i", gl_TexCoord1i, 1);
	rb_define_module_function(module, "glTexCoord1s", gl_TexCoord1s, 1);
	rb_define_module_function(module, "glTexCoord2d", gl_TexCoord2d, 2);
	rb_define_module_function(module, "glTexCoord2f", gl_TexCoord2f, 2);
	rb_define_module_function(module, "glTexCoord2i", gl_TexCoord2i, 2);
	rb_define_module_function(module, "glTexCoord2s", gl_TexCoord2s, 2);
	rb_define_module_function(module, "glTexCoord3d", gl_TexCoord3d, 3);
	rb_define_module_function(module, "glTexCoord3f", gl_TexCoord3f, 3);
	rb_define_module_function(module, "glTexCoord3i", gl_TexCoord3i, 3);
	rb_define_module_function(module, "glTexCoord3s", gl_TexCoord3s, 3);
	rb_define_module_function(module, "glTexCoord4d", gl_TexCoord4d, 4);
	rb_define_module_function(module, "glTexCoord4f", gl_TexCoord4f, 4);
	rb_define_module_function(module, "glTexCoord4i", gl_TexCoord4i, 4);
	rb_define_module_function(module, "glTexCoord4s", gl_TexCoord4s, 4);
	rb_define_module_function(module, "glVertex2d", gl_Vertex2d, 2);
	rb_define_module_function(module, "glVertex2f", gl_Vertex2f, 2);
	rb_define_module_function(module, "glVertex2i", gl_Vertex2i, 2);
	rb_define_module_function(module, "glVertex2s", gl_Vertex2s, 2);
	rb_define_module_function(module, "glVertex3d", gl_Vertex3d, 3);
	rb_define_module_function(module, "glVertex3f", gl_Vertex3f, 3);
	rb_define_module_function(module, "glVertex3i", gl_Vertex3i, 3);
	rb_define_module_function(module, "glVertex3s", gl_Vertex3s, 3);
	rb_define_module_function(module, "glVertex4d", gl_Vertex4d, 4);
	rb_define_module_function(module, "glVertex4f", gl_Vertex4f, 4);
	rb_define_module_function(module, "glVertex4i", gl_Vertex4i, 4);
	rb_define_module_function(module, "glVertex4s", gl_Vertex4s, 4);
	rb_define_module_function(module, "glClipPlane", gl_ClipPlane, 2);
	rb_define_module_function(module, "glColorMaterial", gl_ColorMaterial, 2);
	rb_define_module_function(module, "glCullFace", gl_CullFace, 1);
	rb_define_module_function(module, "glFogf", gl_Fogf, 2);
	rb_define_module_function(module, "glFogfv", gl_Fogfv, 2);
	rb_define_module_function(module, "glFogi", gl_Fogi, 2);
	rb_define_module_function(module, "glFogiv", gl_Fogiv, 2);
	rb_define_module_function(module, "glFrontFace", gl_FrontFace, 1);
	rb_define_module_function(module, "glHint", gl_Hint, 2);
	rb_define_module_function(module, "glLightf", gl_Lightf, 3);
	rb_define_module_function(module, "glLightfv", gl_Lightfv, 3);
	rb_define_module_function(module, "glLighti", gl_Lighti, 3);
	rb_define_module_function(module, "glLightiv", gl_Lightiv, 3);
	rb_define_module_function(module, "glLightModelf", gl_LightModelf, 2);
	rb_define_module_function(module, "glLightModelfv", gl_LightModelfv, 2);
	rb_define_module_function(module, "glLightModeli", gl_LightModeli, 2);
	rb_define_module_function(module, "glLightModeliv", gl_LightModeliv, 2);
	rb_define_module_function(module, "glLineStipple", gl_LineStipple, 2);
	rb_define_module_function(module, "glLineWidth", gl_LineWidth, 1);
	rb_define_module_function(module, "glMaterialf", gl_Materialf, 3);
	rb_define_module_function(module, "glMaterialfv", gl_Materialfv, 3);
	rb_define_module_function(module, "glMateriali", gl_Materiali, 3);
	rb_define_module_function(module, "glMaterialiv", gl_Materialiv, 3);
	rb_define_module_function(module, "glPointSize", gl_PointSize, 1);
	rb_define_module_function(module, "glPolygonMode", gl_PolygonMode, 2);
	rb_define_module_function(module, "glPolygonStipple", gl_PolygonStipple, 1);
	rb_define_module_function(module, "glScissor", gl_Scissor, 4);
	rb_define_module_function(module, "glShadeModel", gl_ShadeModel, 1);
	rb_define_module_function(module, "glTexParameterf", gl_TexParameterf, 3);
	rb_define_module_function(module, "glTexParameterfv", gl_TexParameterfv, 3);
	rb_define_module_function(module, "glTexParameteri", gl_TexParameteri, 3);
	rb_define_module_function(module, "glTexParameteriv", gl_TexParameteriv, 3);
	rb_define_module_function(module, "glTexImage1D", gl_TexImage1D, 8);
	rb_define_module_function(module, "glTexImage2D", gl_TexImage2D, 9);
	rb_define_module_function(module, "glTexEnvf", gl_TexEnvf, 3);
	rb_define_module_function(module, "glTexEnvfv", gl_TexEnvfv, 3);
	rb_define_module_function(module, "glTexEnvi", gl_TexEnvi, 3);
	rb_define_module_function(module, "glTexEnviv", gl_TexEnviv, 3);
	rb_define_module_function(module, "glTexGend", gl_TexGend, 3);
	rb_define_module_function(module, "glTexGendv", gl_TexGendv, 3);
	rb_define_module_function(module, "glTexGenf", gl_TexGenf, 3);
	rb_define_module_function(module, "glTexGenfv", gl_TexGenfv, 3);
	rb_define_module_function(module, "glTexGeni", gl_TexGeni, 3);
	rb_define_module_function(module, "glTexGeniv", gl_TexGeniv, 3);
	rb_define_module_function(module, "glFeedbackBuffer", gl_FeedbackBuffer, 2);
	rb_define_module_function(module, "glSelectBuffer", gl_SelectBuffer, 1);
	rb_define_module_function(module, "glRenderMode", gl_RenderMode, 1);
	rb_define_module_function(module, "glInitNames", gl_InitNames, 0);
	rb_define_module_function(module, "glLoadName", gl_LoadName, 1);
	rb_define_module_function(module, "glPassThrough", gl_PassThrough, 1);
	rb_define_module_function(module, "glPopName", gl_PopName, 0);
	rb_define_module_function(module, "glPushName", gl_PushName, 1);
	rb_define_module_function(module, "glDrawBuffer", gl_DrawBuffer, 1);
	rb_define_module_function(module, "glClear", gl_Clear, 1);
	rb_define_module_function(module, "glClearAccum", gl_ClearAccum, 4);
	rb_define_module_function(module, "glClearIndex", gl_ClearIndex, 1);
	rb_define_module_function(module, "glClearColor", gl_ClearColor, 4);
	rb_define_module_function(module, "glClearStencil", gl_ClearStencil, 1);
	rb_define_module_function(module, "glClearDepth", gl_ClearDepth, 1);
	rb_define_module_function(module, "glStencilMask", gl_StencilMask, 1);
	rb_define_module_function(module, "glColorMask", gl_ColorMask, 4);
	rb_define_module_function(module, "glDepthMask", gl_DepthMask, 1);
	rb_define_module_function(module, "glIndexMask", gl_IndexMask, 1);
	rb_define_module_function(module, "glAccum", gl_Accum, 2);
	rb_define_module_function(module, "glDisable", gl_Disable, 1);
	rb_define_module_function(module, "glEnable", gl_Enable, 1);
	rb_define_module_function(module, "glFinish", gl_Finish, 0);
	rb_define_module_function(module, "glFlush", gl_Flush, 0);
	rb_define_module_function(module, "glPopAttrib", gl_PopAttrib, 0);
	rb_define_module_function(module, "glPushAttrib", gl_PushAttrib, 1);
	rb_define_module_function(module, "glMap1d", gl_Map1d, 6);
	rb_define_module_function(module, "glMap1f", gl_Map1f, 6);
	rb_define_module_function(module, "glMap2d", gl_Map2d, 10);
	rb_define_module_function(module, "glMap2f", gl_Map2f, 10);
	rb_define_module_function(module, "glMapGrid1d", gl_MapGrid1d, 3);
	rb_define_module_function(module, "glMapGrid1f", gl_MapGrid1f, 3);
	rb_define_module_function(module, "glMapGrid2d", gl_MapGrid2d, 6);
	rb_define_module_function(module, "glMapGrid2f", gl_MapGrid2f, 6);
	rb_define_module_function(module, "glEvalCoord1d", gl_EvalCoord1d, 1);
	rb_define_module_function(module, "glEvalCoord1dv", gl_EvalCoord1dv, 1);
	rb_define_module_function(module, "glEvalCoord1f", gl_EvalCoord1f, 1);
	rb_define_module_function(module, "glEvalCoord1fv", gl_EvalCoord1fv, 1);
	rb_define_module_function(module, "glEvalCoord2d", gl_EvalCoord2d, 2);
	rb_define_module_function(module, "glEvalCoord2dv", gl_EvalCoord2dv, 1);
	rb_define_module_function(module, "glEvalCoord2f", gl_EvalCoord2f, 2);
	rb_define_module_function(module, "glEvalCoord2fv", gl_EvalCoord2fv, 1);
	rb_define_module_function(module, "glEvalMesh1", gl_EvalMesh1, 3);
	rb_define_module_function(module, "glEvalPoint1", gl_EvalPoint1, 1);
	rb_define_module_function(module, "glEvalMesh2", gl_EvalMesh2, 5);
	rb_define_module_function(module, "glEvalPoint2", gl_EvalPoint2, 2);
	rb_define_module_function(module, "glAlphaFunc", gl_AlphaFunc, 2);
	rb_define_module_function(module, "glBlendFunc", gl_BlendFunc, 2);
	rb_define_module_function(module, "glLogicOp", gl_LogicOp, 1);
	rb_define_module_function(module, "glStencilFunc", gl_StencilFunc, 3);
	rb_define_module_function(module, "glStencilOp", gl_StencilOp, 3);
	rb_define_module_function(module, "glDepthFunc", gl_DepthFunc, 1);
	rb_define_module_function(module, "glPixelZoom", gl_PixelZoom, 2);
	rb_define_module_function(module, "glPixelTransferf", gl_PixelTransferf, 2);
	rb_define_module_function(module, "glPixelTransferi", gl_PixelTransferi, 2);
	rb_define_module_function(module, "glPixelStoref", gl_PixelStoref, 2);
	rb_define_module_function(module, "glPixelStorei", gl_PixelStorei, 2);
	rb_define_module_function(module, "glPixelMapfv", gl_PixelMapfv, 2);
	rb_define_module_function(module, "glPixelMapuiv", gl_PixelMapuiv, 2);
	rb_define_module_function(module, "glPixelMapusv", gl_PixelMapusv, 2);
	rb_define_module_function(module, "glReadBuffer", gl_ReadBuffer, 1);
	rb_define_module_function(module, "glCopyPixels", gl_CopyPixels, 5);
	rb_define_module_function(module, "glReadPixels", gl_ReadPixels, 6);
	rb_define_module_function(module, "glDrawPixels", gl_DrawPixels, 5);
	rb_define_module_function(module, "glGetBooleanv", gl_GetBooleanv, 1);
	rb_define_module_function(module, "glGetClipPlane", gl_GetClipPlane, 1);
	rb_define_module_function(module, "glGetDoublev", gl_GetDoublev, 1);
	rb_define_module_function(module, "glGetError", gl_GetError, 0);
	rb_define_module_function(module, "glGetFloatv", gl_GetFloatv, 1);
	rb_define_module_function(module, "glGetIntegerv", gl_GetIntegerv, 1);
	rb_define_module_function(module, "glGetLightfv", gl_GetLightfv, 2);
	rb_define_module_function(module, "glGetLightiv", gl_GetLightiv, 2);
	rb_define_module_function(module, "glGetMapdv", gl_GetMapdv, 2);
	rb_define_module_function(module, "glGetMapfv", gl_GetMapfv, 2); 
	rb_define_module_function(module, "glGetMapiv", gl_GetMapiv, 2);
	rb_define_module_function(module, "glGetMaterialfv", gl_GetMaterialfv, 2);
	rb_define_module_function(module, "glGetMaterialiv", gl_GetMaterialiv, 2);
	rb_define_module_function(module, "glGetPixelMapfv", gl_GetPixelMapfv, 1);
	rb_define_module_function(module, "glGetPixelMapuiv", gl_GetPixelMapuiv, 1);
	rb_define_module_function(module, "glGetPixelMapusv", gl_GetPixelMapusv, 1);
	rb_define_module_function(module, "glGetPolygonStipple", gl_GetPolygonStipple, 0);
	rb_define_module_function(module, "glGetString", gl_GetString, 1);
	rb_define_module_function(module, "glGetTexEnvfv", gl_GetTexEnvfv, 2);
	rb_define_module_function(module, "glGetTexEnviv", gl_GetTexEnviv, 2);
	rb_define_module_function(module, "glGetTexGendv", gl_GetTexGendv, 2);
	rb_define_module_function(module, "glGetTexGenfv", gl_GetTexGenfv, 2);
	rb_define_module_function(module, "glGetTexGeniv", gl_GetTexGeniv, 2);
	rb_define_module_function(module, "glGetTexImage", gl_GetTexImage, 4); 
	rb_define_module_function(module, "glGetTexParameterfv", gl_GetTexParameterfv, 2);
	rb_define_module_function(module, "glGetTexParameteriv", gl_GetTexParameteriv, 2);
	rb_define_module_function(module, "glGetTexLevelParameterfv", gl_GetTexLevelParameterfv, 3);
	rb_define_module_function(module, "glGetTexLevelParameteriv", gl_GetTexLevelParameteriv, 3);
	rb_define_module_function(module, "glIsEnabled", gl_IsEnabled, 1);
	rb_define_module_function(module, "glIsList", gl_IsList, 1);
	rb_define_module_function(module, "glDepthRange", gl_DepthRange, 2);
	rb_define_module_function(module, "glFrustum", gl_Frustum, 6);
	rb_define_module_function(module, "glLoadIdentity", gl_LoadIdentity, 0);
	rb_define_module_function(module, "glLoadMatrixf", gl_LoadMatrixf, 1);
	rb_define_module_function(module, "glLoadMatrixd", gl_LoadMatrixd, 1);
	rb_define_module_function(module, "glMatrixMode", gl_MatrixMode, 1);
	rb_define_module_function(module, "glMultMatrixf", gl_MultMatrixf, 1);
	rb_define_module_function(module, "glMultMatrixd", gl_MultMatrixd, 1);
	rb_define_module_function(module, "glOrtho", gl_Ortho, 6);
	rb_define_module_function(module, "glPopMatrix", gl_PopMatrix, 0);
	rb_define_module_function(module, "glPushMatrix", gl_PushMatrix, 0);
	rb_define_module_function(module, "glRotated", gl_Rotated, 4);
	rb_define_module_function(module, "glRotatef", gl_Rotatef, 4);
	rb_define_module_function(module, "glScaled", gl_Scaled, 3);
	rb_define_module_function(module, "glScalef", gl_Scalef, 3);
	rb_define_module_function(module, "glTranslated", gl_Translated, 3);
	rb_define_module_function(module, "glTranslatef", gl_Translatef, 3);
	rb_define_module_function(module, "glViewport", gl_Viewport, 4);

	/* OpenGL 1.1 functions */
	rb_define_module_function(module, "glArrayElement", gl_ArrayElement, 1);
	rb_define_module_function(module, "glColorPointer", gl_ColorPointer, 4);
	rb_define_module_function(module, "glDisableClientState", gl_DisableClientState, 1);
	rb_define_module_function(module, "glDrawArrays", gl_DrawArrays, 3);
	rb_define_module_function(module, "glDrawElements", gl_DrawElements, 4);
	rb_define_module_function(module, "glEdgeFlagPointer", gl_EdgeFlagPointer, 2);
	rb_define_module_function(module, "glEnableClientState", gl_EnableClientState, 1);
	rb_define_module_function(module, "glGetPointerv", gl_GetPointerv, 1);
	rb_define_module_function(module, "glIndexPointer", gl_IndexPointer, 3);
	rb_define_module_function(module, "glInterleavedArrays", gl_InterleavedArrays, 3);
	rb_define_module_function(module, "glNormalPointer", gl_NormalPointer, 3);
	rb_define_module_function(module, "glTexCoordPointer", gl_TexCoordPointer, 4);
	rb_define_module_function(module, "glVertexPointer", gl_VertexPointer, 4);
	rb_define_module_function(module, "glPolygonOffset", gl_PolygonOffset, 2);
	rb_define_module_function(module, "glCopyTexImage1D", gl_CopyTexImage1D, 7);
	rb_define_module_function(module, "glCopyTexImage2D", gl_CopyTexImage2D, 8);
	rb_define_module_function(module, "glCopyTexSubImage1D", gl_CopyTexSubImage1D, 6);
	rb_define_module_function(module, "glCopyTexSubImage2D", gl_CopyTexSubImage2D, 8);
	rb_define_module_function(module, "glTexSubImage1D", gl_TexSubImage1D, 7);
	rb_define_module_function(module, "glTexSubImage2D", gl_TexSubImage2D, 9);
	rb_define_module_function(module, "glAreTexturesResident", gl_AreTexturesResident, 1);
	rb_define_module_function(module, "glBindTexture", gl_BindTexture, 2);
	rb_define_module_function(module, "glDeleteTextures", gl_DeleteTextures, 1);
	rb_define_module_function(module, "glGenTextures", gl_GenTextures, 1);
	rb_define_module_function(module, "glIsTexture", gl_IsTexture, 1);
	rb_define_module_function(module, "glPrioritizeTextures", gl_PrioritizeTextures, 2);
	rb_define_module_function(module, "glIndexub", gl_Indexub, 1);
	rb_define_module_function(module, "glIndexubv", gl_Indexubv, 1);
	rb_define_module_function(module, "glPopClientAttrib", gl_PopClientAttrib, 0);
	rb_define_module_function(module, "glPushClientAttrib", gl_PushClientAttrib, 1);
	
	/* additional functions */
	rb_define_module_function(module, "glColor", gl_Colordv, -1);
	rb_define_module_function(module, "glColor3bv", gl_Colorbv, -1);
	rb_define_module_function(module, "glColor3dv", gl_Colordv, -1);
	rb_define_module_function(module, "glColor3fv", gl_Colorfv, -1);
	rb_define_module_function(module, "glColor3iv", gl_Coloriv, -1);
	rb_define_module_function(module, "glColor3sv", gl_Colorsv, -1);
	rb_define_module_function(module, "glColor3ubv", gl_Colorubv, -1);
	rb_define_module_function(module, "glColor3uiv", gl_Coloruiv, -1);
	rb_define_module_function(module, "glColor3usv", gl_Colorusv, -1);
	rb_define_module_function(module, "glColor4bv", gl_Colorbv, -1);
	rb_define_module_function(module, "glColor4dv", gl_Colordv, -1);
	rb_define_module_function(module, "glColor4fv", gl_Colorfv, -1);
	rb_define_module_function(module, "glColor4iv", gl_Coloriv, -1);
	rb_define_module_function(module, "glColor4sv", gl_Colorsv, -1);
	rb_define_module_function(module, "glColor4ubv", gl_Colorubv, -1);
	rb_define_module_function(module, "glColor4uiv", gl_Coloruiv, -1);
	rb_define_module_function(module, "glColor4usv", gl_Colorusv, -1);

	rb_define_module_function(module, "glNormal", gl_Normaldv, -1);
	rb_define_module_function(module, "glNormal3bv", gl_Normalbv, -1);
	rb_define_module_function(module, "glNormal3dv", gl_Normaldv, -1);
	rb_define_module_function(module, "glNormal3fv", gl_Normalfv, -1);
	rb_define_module_function(module, "glNormal3iv", gl_Normaliv, -1);
	rb_define_module_function(module, "glNormal3sv", gl_Normalsv, -1);

	rb_define_module_function(module, "glRasterPos", gl_RasterPosdv, -1);
	rb_define_module_function(module, "glRasterPos2dv", gl_RasterPosdv, -1);
	rb_define_module_function(module, "glRasterPos2fv", gl_RasterPosfv, -1);
	rb_define_module_function(module, "glRasterPos2iv", gl_RasterPosiv, -1);
	rb_define_module_function(module, "glRasterPos2sv", gl_RasterPossv, -1);
	rb_define_module_function(module, "glRasterPos3dv", gl_RasterPosdv, -1);
	rb_define_module_function(module, "glRasterPos3fv", gl_RasterPosfv, -1);
	rb_define_module_function(module, "glRasterPos3iv", gl_RasterPosiv, -1);
	rb_define_module_function(module, "glRasterPos3sv", gl_RasterPossv, -1);
	rb_define_module_function(module, "glRasterPos4dv", gl_RasterPosdv, -1);
	rb_define_module_function(module, "glRasterPos4fv", gl_RasterPosfv, -1);
	rb_define_module_function(module, "glRasterPos4iv", gl_RasterPosiv, -1);
	rb_define_module_function(module, "glRasterPos4sv", gl_RasterPossv, -1);

	rb_define_module_function(module, "glRect", gl_Rectdv, -1);
	rb_define_module_function(module, "glRectdv", gl_Rectdv, -1);
	rb_define_module_function(module, "glRectfv", gl_Rectfv, -1);
	rb_define_module_function(module, "glRectiv", gl_Rectiv, -1);
	rb_define_module_function(module, "glRectsv", gl_Rectsv, -1);

	rb_define_module_function(module, "glTexCoord", gl_TexCoorddv, -1);
	rb_define_module_function(module, "glTexCoord1dv", gl_TexCoorddv, -1);
	rb_define_module_function(module, "glTexCoord1fv", gl_TexCoordfv, -1);
	rb_define_module_function(module, "glTexCoord1iv", gl_TexCoordiv, -1);
	rb_define_module_function(module, "glTexCoord1sv", gl_TexCoordsv, -1);
	rb_define_module_function(module, "glTexCoord2dv", gl_TexCoorddv, -1);
	rb_define_module_function(module, "glTexCoord2fv", gl_TexCoordfv, -1);
	rb_define_module_function(module, "glTexCoord2iv", gl_TexCoordiv, -1);
	rb_define_module_function(module, "glTexCoord2sv", gl_TexCoordsv, -1);
	rb_define_module_function(module, "glTexCoord3dv", gl_TexCoorddv, -1);
	rb_define_module_function(module, "glTexCoord3fv", gl_TexCoordfv, -1);
	rb_define_module_function(module, "glTexCoord3iv", gl_TexCoordiv, -1);
	rb_define_module_function(module, "glTexCoord3sv", gl_TexCoordsv, -1);
	rb_define_module_function(module, "glTexCoord4dv", gl_TexCoorddv, -1);
	rb_define_module_function(module, "glTexCoord4fv", gl_TexCoordfv, -1);
	rb_define_module_function(module, "glTexCoord4iv", gl_TexCoordiv, -1);
	rb_define_module_function(module, "glTexCoord4sv", gl_TexCoordsv, -1);

	rb_define_module_function(module, "glVertex", gl_Vertexdv, -1);
	rb_define_module_function(module, "glVertex2dv", gl_Vertexdv, -1);
	rb_define_module_function(module, "glVertex2fv", gl_Vertexfv, -1);
	rb_define_module_function(module, "glVertex2iv", gl_Vertexiv, -1);
	rb_define_module_function(module, "glVertex2sv", gl_Vertexsv, -1);
	rb_define_module_function(module, "glVertex3dv", gl_Vertexdv, -1);
	rb_define_module_function(module, "glVertex3fv", gl_Vertexfv, -1);
	rb_define_module_function(module, "glVertex3iv", gl_Vertexiv, -1);
	rb_define_module_function(module, "glVertex3sv", gl_Vertexsv, -1);
	rb_define_module_function(module, "glVertex4dv", gl_Vertexdv, -1);
	rb_define_module_function(module, "glVertex4fv", gl_Vertexfv, -1);
	rb_define_module_function(module, "glVertex4iv", gl_Vertexiv, -1);
	rb_define_module_function(module, "glVertex4sv", gl_Vertexsv, -1);

	/* these simply calls normal or vector (*v) function depending on
	   if array or single value is passed to them */
	rb_define_module_function(module, "glLightModel", gl_LightModel, 2);
	rb_define_module_function(module, "glMaterial", gl_Material, 3);
	rb_define_module_function(module, "glFog", gl_Fog, 2);
	rb_define_module_function(module, "glLight", gl_Light, 3);
	rb_define_module_function(module, "glTexParameter", gl_TexParameter, 3);
	rb_define_module_function(module, "glTexEnv", gl_TexEnv, 3);
	rb_define_module_function(module, "glTexGen", gl_TexGen, 3);

	/* aliases */
	rb_define_module_function(module, "glMultMatrix", gl_MultMatrixd, 1);
	rb_define_module_function(module, "glLoadMatrix", gl_LoadMatrixd, 1);
	rb_define_module_function(module, "glRotate", gl_Rotated, 4);
	rb_define_module_function(module, "glScale", gl_Scaled, 3);
	rb_define_module_function(module, "glTranslate", gl_Translated, 3);

	rb_define_module_function(module, "glPixelStore", gl_PixelStoref, 2);
	rb_define_module_function(module, "glPixelTransfer", gl_PixelTransferf, 2);
	rb_define_module_function(module, "glIndex", gl_Indexi, 1);
	rb_define_module_function(module, "glGetMaterial", gl_GetMaterialfv, 2);
	rb_define_module_function(module, "glGetDouble", gl_GetDoublev, 1);

	rb_global_variable(&g_current_sel_buffer);
	rb_global_variable(&g_current_feed_buffer);
	rb_global_variable(&g_Vertex_ptr);
	rb_global_variable(&g_Normal_ptr);
	rb_global_variable(&g_Color_ptr);
	rb_global_variable(&g_Index_ptr);
	rb_global_variable(&g_TexCoord_ptr);
	rb_global_variable(&g_EdgeFlag_ptr);
	rb_global_variable(&g_FogCoord_ptr);
	rb_global_variable(&g_SecondaryColor_ptr);
}
