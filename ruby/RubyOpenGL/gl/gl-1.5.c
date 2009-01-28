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

/* OpenGL 1.5 functions */

static void (APIENTRY * fptr_glGenQueries)(GLsizei,GLuint *);
static VALUE
gl_GenQueries(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLuint *queries;
	RArray *ret;
	int i;
	LOAD_GL_FUNC(glGenQueries)
	n = (GLsizei)NUM2UINT(arg1);
	queries = ALLOC_N(GLuint, n);
	fptr_glGenQueries(n, queries);
	ret = RARRAY(rb_ary_new2(n));
	for (i = 0; i < n; i++)
		rb_ary_push((VALUE)ret, INT2NUM(queries[i]));
	xfree(queries);
	return (VALUE)ret;
}

static void (APIENTRY * fptr_glDeleteQueries)(GLsizei,GLuint *);
static VALUE
gl_DeleteQueries(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLuint *queries;
	LOAD_GL_FUNC(glDeleteQueries)
	Check_Type(arg1,T_ARRAY);
	n = RARRAY(arg1)->len;
	queries = ALLOC_N(GLuint,n);
	ary2cuint(arg1,queries,n); 
	fptr_glDeleteQueries( n, queries);
	xfree(queries);
	return Qnil;
}

static GLboolean (APIENTRY * fptr_glIsQuery)(GLuint);
static VALUE
gl_IsQuery(obj,arg1)
VALUE obj,arg1;
{
	GLuint query;
	GLboolean ret;
	LOAD_GL_FUNC(glIsQuery)
	query = (GLuint)NUM2UINT(arg1);
	ret = fptr_glIsQuery(query);
	return INT2NUM(ret);
}

static void (APIENTRY * fptr_glBeginQuery)(GLenum,GLuint);
static VALUE
gl_BeginQuery(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLuint id;
	LOAD_GL_FUNC(glBeginQuery)
	target = (GLenum)NUM2INT(arg1);
	id = (GLuint)NUM2UINT(arg2);
	fptr_glBeginQuery(target,id);
	return Qnil;
}

static void (APIENTRY * fptr_glEndQuery)(GLenum);
static VALUE
gl_EndQuery(obj,arg1)
VALUE obj,arg1;
{
	GLenum target;
	LOAD_GL_FUNC(glEndQuery)
	target = (GLenum)NUM2INT(arg1);
	fptr_glEndQuery(target);
	return Qnil;
}

static void (APIENTRY * fptr_glGetQueryiv)(GLenum,GLenum,GLint *);
static VALUE
gl_GetQueryiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum pname;
	GLint params = 0;
	VALUE retary;
	LOAD_GL_FUNC(glGetQueryiv)
	target = (GLenum)NUM2INT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetQueryiv(target,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary, INT2NUM(params));
	return retary;
}

static void (APIENTRY * fptr_glGetQueryObjectiv)(GLuint,GLenum,GLint *);
static VALUE
gl_GetQueryObjectiv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLuint id;
	GLenum pname;
	GLint params = 0;
	VALUE retary;
	LOAD_GL_FUNC(glGetQueryObjectiv)
	id = (GLuint)NUM2UINT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetQueryObjectiv(id,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary,INT2NUM(params));
	return retary;
}

static void (APIENTRY * fptr_glGetQueryObjectuiv)(GLuint,GLenum,GLuint *);
static VALUE
gl_GetQueryObjectuiv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLuint id;
	GLenum pname;
	GLuint params = 0;
	VALUE retary;
	LOAD_GL_FUNC(glGetQueryObjectuiv)
	id = (GLuint)NUM2UINT(arg1);
	pname = (GLenum)NUM2INT(arg2);
	fptr_glGetQueryObjectuiv(id,pname,&params);
	retary = rb_ary_new2(1);
	rb_ary_push(retary,INT2NUM(params));
	return retary;
}

static void (APIENTRY * fptr_glBindBuffer)(GLenum,GLuint);
static VALUE
gl_BindBuffer(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLuint buffer;
	LOAD_GL_FUNC(glBindBuffer)
	target = (GLenum)NUM2INT(arg1);
	buffer = (GLenum)NUM2INT(arg2);
	fptr_glBindBuffer(target,buffer);
	return Qnil;
}

static void (APIENTRY * fptr_glDeleteBuffers)(GLsizei,GLuint *);
static VALUE
gl_DeleteBuffers(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLuint *buffers;
	LOAD_GL_FUNC(glDeleteBuffers)
	Check_Type(arg1,T_ARRAY);
	n = RARRAY(arg1)->len;
	buffers = ALLOC_N(GLuint,n);
	ary2cuint(arg1,buffers,n); 
	fptr_glDeleteBuffers(n, buffers);
	xfree(buffers);
	return Qnil;
}

static void (APIENTRY * fptr_glGenBuffers)(GLsizei,GLuint *);
static VALUE
gl_GenBuffers(obj,arg1)
VALUE obj,arg1;
{
	GLsizei n;
	GLuint *buffers;
	RArray *ret;
	int i;
	LOAD_GL_FUNC(glGenBuffers)
	n = (GLsizei)NUM2UINT(arg1);
	buffers = ALLOC_N(GLuint, n);
	fptr_glGenBuffers(n, buffers);
	ret = RARRAY(rb_ary_new2(n));
	for (i = 0; i < n; i++)
		rb_ary_push((VALUE)ret, INT2NUM(buffers[i]));
	xfree(buffers);
	return (VALUE)ret;
}

static GLboolean (APIENTRY * fptr_glIsBuffer)(GLuint);
static VALUE
gl_IsBuffer(obj,arg1)
VALUE obj,arg1;
{
	GLuint buffer;
	GLboolean ret;
	LOAD_GL_FUNC(glIsBuffer)
	buffer = (GLuint)NUM2UINT(arg1);
	ret = fptr_glIsBuffer(buffer);
	return INT2NUM(ret);
}

static void (APIENTRY * fptr_glBufferData)(GLenum,GLsizeiptr,GLvoid *,GLenum);
static VALUE
gl_BufferData(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLsizeiptr size;
	GLenum usage;
	LOAD_GL_FUNC(glBufferData)
	target = (GLenum)NUM2INT(arg1);
	size = (GLsizeiptr)NUM2INT(arg2);
	usage = (GLenum)NUM2INT(arg4);
	if (TYPE(arg3) == T_STRING) {
		fptr_glBufferData(target,size,(GLvoid *)RSTRING(arg3)->ptr,usage);
	} else if (NIL_P(arg3)) {
		fptr_glBufferData(target,size,NULL,usage);
	} else {
		Check_Type(arg3,T_STRING); /* force exception */
	}
	return Qnil;
}

static void (APIENTRY * fptr_glBufferSubData)(GLenum,GLintptr,GLsizeiptr,GLvoid *);
static VALUE
gl_BufferSubData(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
	LOAD_GL_FUNC(glBufferSubData)
	target = (GLenum)NUM2INT(arg1);
	offset = (GLintptr)NUM2INT(arg2);
	size = (GLsizeiptr)NUM2INT(arg3);
	Check_Type(arg4,T_STRING);
	fptr_glBufferSubData(target,offset,size,(GLvoid *)RSTRING(arg4)->ptr);
	return Qnil;
}

static void (APIENTRY * fptr_glGetBufferSubData)(GLenum,GLintptr,GLsizeiptr,GLvoid *);
static VALUE
gl_GetBufferSubData(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
	VALUE data;
	LOAD_GL_FUNC(glGetBufferSubData)
	target = (GLenum)NUM2INT(arg1);
	offset = (GLintptr)NUM2INT(arg2);
	size = (GLsizeiptr)NUM2INT(arg3);
	data = allocate_buffer_with_string(size);
	fptr_glGetBufferSubData(target,offset,size,(GLvoid *)RSTRING(data)->ptr);
	return data;
}

static void (APIENTRY * fptr_glGetBufferParameteriv)(GLenum,GLenum,GLint *);
static VALUE
gl_GetBufferParameteriv(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum value;
	GLint data = 0;
	VALUE retary;
	LOAD_GL_FUNC(glGetBufferParameteriv)
	target = (GLenum)NUM2INT(arg1);
	value = (GLenum)NUM2INT(arg2);
	fptr_glGetBufferParameteriv(target,value,&data);
	retary = rb_ary_new2(1);
	rb_ary_push(retary,INT2NUM(data));
	return retary;
}

static GLvoid * (APIENTRY * fptr_glMapBuffer)(GLenum,GLenum);
static VALUE
gl_MapBuffer(obj,arg1,arg2)
VALUE obj,arg1,arg2;
{
	GLenum target;
	GLenum access;
	GLint size = 0;
	VALUE data;
	GLvoid *buffer_ptr;
	LOAD_GL_FUNC(glMapBuffer)
	LOAD_GL_FUNC(glGetBufferParameteriv)
	target = (GLenum)NUM2INT(arg1);
	access = (GLenum)NUM2INT(arg2);
	fptr_glGetBufferParameteriv(target,GL_BUFFER_SIZE,&size);
	buffer_ptr = fptr_glMapBuffer(target,access);
	data = allocate_buffer_with_string(size);
	memcpy(RSTRING(data)->ptr, buffer_ptr, size);
	return data;
}

static GLboolean (APIENTRY * fptr_glUnmapBuffer)(GLenum);
static VALUE
gl_UnmapBuffer(obj,arg1)
VALUE obj,arg1;
{
	GLenum target;
	GLboolean ret;
	LOAD_GL_FUNC(glUnmapBuffer)
	target = (GLenum)NUM2INT(arg1);
	ret = fptr_glUnmapBuffer(target);
	return INT2NUM(ret);
}

static void (APIENTRY * fptr_glGetBufferPointerv)(GLenum,GLenum,GLvoid **);
static VALUE
gl_GetBufferPointerv(obj,arg1,arg2,arg3)
VALUE obj,arg1,arg2,arg3;
{
	LOAD_GL_FUNC(glGetBufferPointerv)
	/* not implemented */
	return Qnil;
}

void gl_init_functions_1_5(VALUE module)
{
	rb_define_module_function(module, "glGenQueries", gl_GenQueries, 1);
	rb_define_module_function(module, "glDeleteQueries", gl_DeleteQueries, 1);
	rb_define_module_function(module, "glIsQuery", gl_IsQuery, 1);
	rb_define_module_function(module, "glBeginQuery", gl_BeginQuery, 2);
	rb_define_module_function(module, "glEndQuery", gl_EndQuery, 1);
	rb_define_module_function(module, "glGetQueryiv", gl_GetQueryiv, 2);
	rb_define_module_function(module, "glGetQueryObjectiv", gl_GetQueryObjectiv, 2);
	rb_define_module_function(module, "glGetQueryObjectuiv", gl_GetQueryObjectuiv, 2);
	rb_define_module_function(module, "glBindBuffer", gl_BindBuffer, 2);
	rb_define_module_function(module, "glDeleteBuffers", gl_DeleteBuffers, 1);
	rb_define_module_function(module, "glGenBuffers", gl_GenBuffers, 1);
	rb_define_module_function(module, "glIsBuffer", gl_IsBuffer, 1);
	rb_define_module_function(module, "glBufferData", gl_BufferData, 4);
	rb_define_module_function(module, "glBufferSubData", gl_BufferSubData, 4);
	rb_define_module_function(module, "glGetBufferSubData", gl_GetBufferSubData, 3);
	rb_define_module_function(module, "glMapBuffer", gl_MapBuffer, 2);
	rb_define_module_function(module, "glUnmapBuffer", gl_UnmapBuffer, 1);
	rb_define_module_function(module, "glGetBufferParameteriv", gl_GetBufferParameteriv, 2);
	rb_define_module_function(module, "glGetBufferPointerv", gl_GetBufferPointerv, 3);
}
