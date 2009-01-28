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

static void (APIENTRY * fptr_glUniformMatrix2x3fv)(GLint,GLsizei,GLboolean,GLfloat *);
static VALUE
gl_UniformMatrix2x3fv(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat *value;	
	LOAD_GL_FUNC(glUniformMatrix2x3fv)
	location = (GLint)NUM2INT(arg1);
	count = (GLint)NUM2INT(arg2);
	transpose = (GLboolean)NUM2INT(arg3);
	value = ALLOC_N(GLfloat, 2*3*count);
	ary2cflt(arg4,value,2*3*count);
	fptr_glUniformMatrix2x3fv(location,count,transpose,value);
	xfree(value);
	return Qnil;
}

static void (APIENTRY * fptr_glUniformMatrix3x2fv)(GLint,GLsizei,GLboolean,GLfloat *);
static VALUE
gl_UniformMatrix3x2fv(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat *value;	
	LOAD_GL_FUNC(glUniformMatrix3x2fv)
	location = (GLint)NUM2INT(arg1);
	count = (GLint)NUM2INT(arg2);
	transpose = (GLboolean)NUM2INT(arg3);
	value = ALLOC_N(GLfloat, 3*2*count);
	ary2cflt(arg4,value,3*2*count);
	fptr_glUniformMatrix3x2fv(location,count,transpose,value);
	xfree(value);
	return Qnil;
}

static void (APIENTRY * fptr_glUniformMatrix2x4fv)(GLint,GLsizei,GLboolean,GLfloat *);
static VALUE
gl_UniformMatrix2x4fv(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat *value;	
	LOAD_GL_FUNC(glUniformMatrix2x4fv)
	location = (GLint)NUM2INT(arg1);
	count = (GLint)NUM2INT(arg2);
	transpose = (GLboolean)NUM2INT(arg3);
	value = ALLOC_N(GLfloat, 2*4*count);
	ary2cflt(arg4,value,2*4*count);
	fptr_glUniformMatrix2x4fv(location,count,transpose,value);
	xfree(value);
	return Qnil;
}

static void (APIENTRY * fptr_glUniformMatrix4x2fv)(GLint,GLsizei,GLboolean,GLfloat *);
static VALUE
gl_UniformMatrix4x2fv(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat *value;	
	LOAD_GL_FUNC(glUniformMatrix4x2fv)
	location = (GLint)NUM2INT(arg1);
	count = (GLint)NUM2INT(arg2);
	transpose = (GLboolean)NUM2INT(arg3);
	value = ALLOC_N(GLfloat, 4*2*count);
	ary2cflt(arg4,value,4*2*count);
	fptr_glUniformMatrix4x2fv(location,count,transpose,value);
	xfree(value);
	return Qnil;
}

static void (APIENTRY * fptr_glUniformMatrix3x4fv)(GLint,GLsizei,GLboolean,GLfloat *);
static VALUE
gl_UniformMatrix3x4fv(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat *value;	
	LOAD_GL_FUNC(glUniformMatrix3x4fv)
	location = (GLint)NUM2INT(arg1);
	count = (GLint)NUM2INT(arg2);
	transpose = (GLboolean)NUM2INT(arg3);
	value = ALLOC_N(GLfloat, 3*4*count);
	ary2cflt(arg4,value,3*4*count);
	fptr_glUniformMatrix3x4fv(location,count,transpose,value);
	xfree(value);
	return Qnil;
}

static void (APIENTRY * fptr_glUniformMatrix4x3fv)(GLint,GLsizei,GLboolean,GLfloat *);
static VALUE
gl_UniformMatrix4x3fv(obj,arg1,arg2,arg3,arg4)
VALUE obj,arg1,arg2,arg3,arg4;
{
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat *value;	
	LOAD_GL_FUNC(glUniformMatrix4x3fv)
	location = (GLint)NUM2INT(arg1);
	count = (GLint)NUM2INT(arg2);
	transpose = (GLboolean)NUM2INT(arg3);
	value = ALLOC_N(GLfloat, 4*3*count);
	ary2cflt(arg4,value,4*3*count);
	fptr_glUniformMatrix4x3fv(location,count,transpose,value);
	xfree(value);
	return Qnil;
}

void gl_init_functions_2_1(VALUE module)
{
	rb_define_module_function(module, "glUniformMatrix2x3fv", gl_UniformMatrix2x3fv, 4);
	rb_define_module_function(module, "glUniformMatrix3x2fv", gl_UniformMatrix3x2fv, 4);
	rb_define_module_function(module, "glUniformMatrix2x4fv", gl_UniformMatrix2x4fv, 4);
	rb_define_module_function(module, "glUniformMatrix4x2fv", gl_UniformMatrix4x2fv, 4);
	rb_define_module_function(module, "glUniformMatrix3x4fv", gl_UniformMatrix3x4fv, 4);
	rb_define_module_function(module, "glUniformMatrix4x3fv", gl_UniformMatrix4x3fv, 4);
}
