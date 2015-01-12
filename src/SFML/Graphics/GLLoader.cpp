#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <SFML/Graphics/GLLoader.hpp>

#if defined(__APPLE__)
#include <dlfcn.h>

static void* AppleGLGetProcAddress (const char *name)
{
    static void* image = NULL;

    if (NULL == image)
        image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);

    return (image ? dlsym(image, name) : NULL);
}
#define IntGetProcAddress(name) AppleGLGetProcAddress(name)
#endif /* __APPLE__ */

#if defined(__sgi) || defined (__sun)
#include <dlfcn.h>
#include <stdio.h>

static void* SunGetProcAddress (const GLubyte* name)
{
    static void* h = NULL;
    static void* gpa;

    if (h == NULL)
    {
        if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
        gpa = dlsym(h, "glXGetProcAddress");
    }

    if (gpa != NULL)
        return ((void*(*)(const GLubyte*))gpa)(name);
    else
        return dlsym(h, (const char*)name);
}
#endif /* __sgi || __sun */

#if defined(_WIN32)

#ifdef _MSC_VER
#pragma warning(disable: 4055)
#pragma warning(disable: 4054)
#pragma warning(disable: 4996)
#endif

static int TestPointer(const PROC pTest)
{
    ptrdiff_t iTest;
    if(!pTest) return 0;
    iTest = (ptrdiff_t)pTest;

    if(iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;

    return 1;
}

static PROC WinGetProcAddress(const char *name)
{
    static HMODULE glMod = NULL;
    PROC pFunc = wglGetProcAddress((LPCSTR)name);

    if (TestPointer(pFunc)) return pFunc;

    if (NULL == glMod)
        glMod = GetModuleHandleA("OpenGL32.dll");

    return (PROC)GetProcAddress(glMod, (LPCSTR)name);
}

#define IntGetProcAddress(name) WinGetProcAddress(name)
#else
    #if defined(__APPLE__)
        #define IntGetProcAddress(name) AppleGLGetProcAddress(name)
    #else
        #if defined(__sgi) || defined(__sun)
            #define IntGetProcAddress(name) SunGetProcAddress(name)
        #else /* GLX */
                #include <GL/glx.h>

            #define IntGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
        #endif
    #endif
#endif

int sfogl_ext_EXT_blend_minmax = sfogl_LOAD_FAILED;
int sfogl_ext_EXT_blend_subtract = sfogl_LOAD_FAILED;
int sfogl_ext_ARB_multitexture = sfogl_LOAD_FAILED;
int sfogl_ext_EXT_blend_func_separate = sfogl_LOAD_FAILED;
int sfogl_ext_ARB_shading_language_100 = sfogl_LOAD_FAILED;
int sfogl_ext_ARB_shader_objects = sfogl_LOAD_FAILED;
int sfogl_ext_ARB_vertex_shader = sfogl_LOAD_FAILED;
int sfogl_ext_ARB_fragment_shader = sfogl_LOAD_FAILED;
int sfogl_ext_ARB_texture_non_power_of_two = sfogl_LOAD_FAILED;
int sfogl_ext_EXT_blend_equation_separate = sfogl_LOAD_FAILED;
int sfogl_ext_EXT_framebuffer_object = sfogl_LOAD_FAILED;

void (CODEGEN_FUNCPTR *sf_ptrc_glBlendEquationEXT)(GLenum) = NULL;

static int Load_EXT_blend_minmax()
{
    int numFailed = 0;
    sf_ptrc_glBlendEquationEXT = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBlendEquationEXT");
    if(!sf_ptrc_glBlendEquationEXT) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glActiveTextureARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClientActiveTextureARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1dARB)(GLenum, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1fARB)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1iARB)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1sARB)(GLenum, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord1svARB)(GLenum, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2dARB)(GLenum, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2fARB)(GLenum, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2iARB)(GLenum, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2sARB)(GLenum, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord2svARB)(GLenum, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3dARB)(GLenum, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3fARB)(GLenum, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3iARB)(GLenum, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3sARB)(GLenum, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord3svARB)(GLenum, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4dARB)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4fARB)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4iARB)(GLenum, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4sARB)(GLenum, GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultiTexCoord4svARB)(GLenum, const GLshort *) = NULL;

static int Load_ARB_multitexture()
{
    int numFailed = 0;
    sf_ptrc_glActiveTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glActiveTextureARB");
    if(!sf_ptrc_glActiveTextureARB) numFailed++;
    sf_ptrc_glClientActiveTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glClientActiveTextureARB");
    if(!sf_ptrc_glClientActiveTextureARB) numFailed++;
    sf_ptrc_glMultiTexCoord1dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble))IntGetProcAddress("glMultiTexCoord1dARB");
    if(!sf_ptrc_glMultiTexCoord1dARB) numFailed++;
    sf_ptrc_glMultiTexCoord1dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord1dvARB");
    if(!sf_ptrc_glMultiTexCoord1dvARB) numFailed++;
    sf_ptrc_glMultiTexCoord1fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glMultiTexCoord1fARB");
    if(!sf_ptrc_glMultiTexCoord1fARB) numFailed++;
    sf_ptrc_glMultiTexCoord1fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord1fvARB");
    if(!sf_ptrc_glMultiTexCoord1fvARB) numFailed++;
    sf_ptrc_glMultiTexCoord1iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glMultiTexCoord1iARB");
    if(!sf_ptrc_glMultiTexCoord1iARB) numFailed++;
    sf_ptrc_glMultiTexCoord1ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord1ivARB");
    if(!sf_ptrc_glMultiTexCoord1ivARB) numFailed++;
    sf_ptrc_glMultiTexCoord1sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort))IntGetProcAddress("glMultiTexCoord1sARB");
    if(!sf_ptrc_glMultiTexCoord1sARB) numFailed++;
    sf_ptrc_glMultiTexCoord1svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord1svARB");
    if(!sf_ptrc_glMultiTexCoord1svARB) numFailed++;
    sf_ptrc_glMultiTexCoord2dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble))IntGetProcAddress("glMultiTexCoord2dARB");
    if(!sf_ptrc_glMultiTexCoord2dARB) numFailed++;
    sf_ptrc_glMultiTexCoord2dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord2dvARB");
    if(!sf_ptrc_glMultiTexCoord2dvARB) numFailed++;
    sf_ptrc_glMultiTexCoord2fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat))IntGetProcAddress("glMultiTexCoord2fARB");
    if(!sf_ptrc_glMultiTexCoord2fARB) numFailed++;
    sf_ptrc_glMultiTexCoord2fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord2fvARB");
    if(!sf_ptrc_glMultiTexCoord2fvARB) numFailed++;
    sf_ptrc_glMultiTexCoord2iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint))IntGetProcAddress("glMultiTexCoord2iARB");
    if(!sf_ptrc_glMultiTexCoord2iARB) numFailed++;
    sf_ptrc_glMultiTexCoord2ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord2ivARB");
    if(!sf_ptrc_glMultiTexCoord2ivARB) numFailed++;
    sf_ptrc_glMultiTexCoord2sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort, GLshort))IntGetProcAddress("glMultiTexCoord2sARB");
    if(!sf_ptrc_glMultiTexCoord2sARB) numFailed++;
    sf_ptrc_glMultiTexCoord2svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord2svARB");
    if(!sf_ptrc_glMultiTexCoord2svARB) numFailed++;
    sf_ptrc_glMultiTexCoord3dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glMultiTexCoord3dARB");
    if(!sf_ptrc_glMultiTexCoord3dARB) numFailed++;
    sf_ptrc_glMultiTexCoord3dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord3dvARB");
    if(!sf_ptrc_glMultiTexCoord3dvARB) numFailed++;
    sf_ptrc_glMultiTexCoord3fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glMultiTexCoord3fARB");
    if(!sf_ptrc_glMultiTexCoord3fARB) numFailed++;
    sf_ptrc_glMultiTexCoord3fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord3fvARB");
    if(!sf_ptrc_glMultiTexCoord3fvARB) numFailed++;
    sf_ptrc_glMultiTexCoord3iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint))IntGetProcAddress("glMultiTexCoord3iARB");
    if(!sf_ptrc_glMultiTexCoord3iARB) numFailed++;
    sf_ptrc_glMultiTexCoord3ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord3ivARB");
    if(!sf_ptrc_glMultiTexCoord3ivARB) numFailed++;
    sf_ptrc_glMultiTexCoord3sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort, GLshort, GLshort))IntGetProcAddress("glMultiTexCoord3sARB");
    if(!sf_ptrc_glMultiTexCoord3sARB) numFailed++;
    sf_ptrc_glMultiTexCoord3svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord3svARB");
    if(!sf_ptrc_glMultiTexCoord3svARB) numFailed++;
    sf_ptrc_glMultiTexCoord4dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glMultiTexCoord4dARB");
    if(!sf_ptrc_glMultiTexCoord4dARB) numFailed++;
    sf_ptrc_glMultiTexCoord4dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord4dvARB");
    if(!sf_ptrc_glMultiTexCoord4dvARB) numFailed++;
    sf_ptrc_glMultiTexCoord4fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glMultiTexCoord4fARB");
    if(!sf_ptrc_glMultiTexCoord4fARB) numFailed++;
    sf_ptrc_glMultiTexCoord4fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord4fvARB");
    if(!sf_ptrc_glMultiTexCoord4fvARB) numFailed++;
    sf_ptrc_glMultiTexCoord4iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint))IntGetProcAddress("glMultiTexCoord4iARB");
    if(!sf_ptrc_glMultiTexCoord4iARB) numFailed++;
    sf_ptrc_glMultiTexCoord4ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord4ivARB");
    if(!sf_ptrc_glMultiTexCoord4ivARB) numFailed++;
    sf_ptrc_glMultiTexCoord4sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glMultiTexCoord4sARB");
    if(!sf_ptrc_glMultiTexCoord4sARB) numFailed++;
    sf_ptrc_glMultiTexCoord4svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord4svARB");
    if(!sf_ptrc_glMultiTexCoord4svARB) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glBlendFuncSeparateEXT)(GLenum, GLenum, GLenum, GLenum) = NULL;

static int Load_EXT_blend_func_separate()
{
    int numFailed = 0;
    sf_ptrc_glBlendFuncSeparateEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLenum))IntGetProcAddress("glBlendFuncSeparateEXT");
    if(!sf_ptrc_glBlendFuncSeparateEXT) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glAttachObjectARB)(GLhandleARB, GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCompileShaderARB)(GLhandleARB) = NULL;
GLhandleARB (CODEGEN_FUNCPTR *sf_ptrc_glCreateProgramObjectARB)() = NULL;
GLhandleARB (CODEGEN_FUNCPTR *sf_ptrc_glCreateShaderObjectARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteObjectARB)(GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDetachObjectARB)(GLhandleARB, GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetActiveUniformARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetAttachedObjectsARB)(GLhandleARB, GLsizei, GLsizei *, GLhandleARB *) = NULL;
GLhandleARB (CODEGEN_FUNCPTR *sf_ptrc_glGetHandleARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetInfoLogARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetObjectParameterfvARB)(GLhandleARB, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetObjectParameterivARB)(GLhandleARB, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetShaderSourceARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *) = NULL;
GLint (CODEGEN_FUNCPTR *sf_ptrc_glGetUniformLocationARB)(GLhandleARB, const GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetUniformfvARB)(GLhandleARB, GLint, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetUniformivARB)(GLhandleARB, GLint, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLinkProgramARB)(GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glShaderSourceARB)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1fARB)(GLint, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1iARB)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform1ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2fARB)(GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2iARB)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform2ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3fARB)(GLint, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3iARB)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform3ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4fARB)(GLint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4iARB)(GLint, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniform4ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniformMatrix2fvARB)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniformMatrix3fvARB)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUniformMatrix4fvARB)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glUseProgramObjectARB)(GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glValidateProgramARB)(GLhandleARB) = NULL;

static int Load_ARB_shader_objects()
{
    int numFailed = 0;
    sf_ptrc_glAttachObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLhandleARB))IntGetProcAddress("glAttachObjectARB");
    if(!sf_ptrc_glAttachObjectARB) numFailed++;
    sf_ptrc_glCompileShaderARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glCompileShaderARB");
    if(!sf_ptrc_glCompileShaderARB) numFailed++;
    sf_ptrc_glCreateProgramObjectARB = (GLhandleARB (CODEGEN_FUNCPTR *)())IntGetProcAddress("glCreateProgramObjectARB");
    if(!sf_ptrc_glCreateProgramObjectARB) numFailed++;
    sf_ptrc_glCreateShaderObjectARB = (GLhandleARB (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCreateShaderObjectARB");
    if(!sf_ptrc_glCreateShaderObjectARB) numFailed++;
    sf_ptrc_glDeleteObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glDeleteObjectARB");
    if(!sf_ptrc_glDeleteObjectARB) numFailed++;
    sf_ptrc_glDetachObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLhandleARB))IntGetProcAddress("glDetachObjectARB");
    if(!sf_ptrc_glDetachObjectARB) numFailed++;
    sf_ptrc_glGetActiveUniformARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *))IntGetProcAddress("glGetActiveUniformARB");
    if(!sf_ptrc_glGetActiveUniformARB) numFailed++;
    sf_ptrc_glGetAttachedObjectsARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, GLsizei *, GLhandleARB *))IntGetProcAddress("glGetAttachedObjectsARB");
    if(!sf_ptrc_glGetAttachedObjectsARB) numFailed++;
    sf_ptrc_glGetHandleARB = (GLhandleARB (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetHandleARB");
    if(!sf_ptrc_glGetHandleARB) numFailed++;
    sf_ptrc_glGetInfoLogARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *))IntGetProcAddress("glGetInfoLogARB");
    if(!sf_ptrc_glGetInfoLogARB) numFailed++;
    sf_ptrc_glGetObjectParameterfvARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLenum, GLfloat *))IntGetProcAddress("glGetObjectParameterfvARB");
    if(!sf_ptrc_glGetObjectParameterfvARB) numFailed++;
    sf_ptrc_glGetObjectParameterivARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLenum, GLint *))IntGetProcAddress("glGetObjectParameterivARB");
    if(!sf_ptrc_glGetObjectParameterivARB) numFailed++;
    sf_ptrc_glGetShaderSourceARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *))IntGetProcAddress("glGetShaderSourceARB");
    if(!sf_ptrc_glGetShaderSourceARB) numFailed++;
    sf_ptrc_glGetUniformLocationARB = (GLint (CODEGEN_FUNCPTR *)(GLhandleARB, const GLcharARB *))IntGetProcAddress("glGetUniformLocationARB");
    if(!sf_ptrc_glGetUniformLocationARB) numFailed++;
    sf_ptrc_glGetUniformfvARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLint, GLfloat *))IntGetProcAddress("glGetUniformfvARB");
    if(!sf_ptrc_glGetUniformfvARB) numFailed++;
    sf_ptrc_glGetUniformivARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLint, GLint *))IntGetProcAddress("glGetUniformivARB");
    if(!sf_ptrc_glGetUniformivARB) numFailed++;
    sf_ptrc_glLinkProgramARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glLinkProgramARB");
    if(!sf_ptrc_glLinkProgramARB) numFailed++;
    sf_ptrc_glShaderSourceARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *))IntGetProcAddress("glShaderSourceARB");
    if(!sf_ptrc_glShaderSourceARB) numFailed++;
    sf_ptrc_glUniform1fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat))IntGetProcAddress("glUniform1fARB");
    if(!sf_ptrc_glUniform1fARB) numFailed++;
    sf_ptrc_glUniform1fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform1fvARB");
    if(!sf_ptrc_glUniform1fvARB) numFailed++;
    sf_ptrc_glUniform1iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glUniform1iARB");
    if(!sf_ptrc_glUniform1iARB) numFailed++;
    sf_ptrc_glUniform1ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform1ivARB");
    if(!sf_ptrc_glUniform1ivARB) numFailed++;
    sf_ptrc_glUniform2fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat))IntGetProcAddress("glUniform2fARB");
    if(!sf_ptrc_glUniform2fARB) numFailed++;
    sf_ptrc_glUniform2fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform2fvARB");
    if(!sf_ptrc_glUniform2fvARB) numFailed++;
    sf_ptrc_glUniform2iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glUniform2iARB");
    if(!sf_ptrc_glUniform2iARB) numFailed++;
    sf_ptrc_glUniform2ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform2ivARB");
    if(!sf_ptrc_glUniform2ivARB) numFailed++;
    sf_ptrc_glUniform3fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glUniform3fARB");
    if(!sf_ptrc_glUniform3fARB) numFailed++;
    sf_ptrc_glUniform3fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform3fvARB");
    if(!sf_ptrc_glUniform3fvARB) numFailed++;
    sf_ptrc_glUniform3iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glUniform3iARB");
    if(!sf_ptrc_glUniform3iARB) numFailed++;
    sf_ptrc_glUniform3ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform3ivARB");
    if(!sf_ptrc_glUniform3ivARB) numFailed++;
    sf_ptrc_glUniform4fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glUniform4fARB");
    if(!sf_ptrc_glUniform4fARB) numFailed++;
    sf_ptrc_glUniform4fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform4fvARB");
    if(!sf_ptrc_glUniform4fvARB) numFailed++;
    sf_ptrc_glUniform4iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint, GLint))IntGetProcAddress("glUniform4iARB");
    if(!sf_ptrc_glUniform4iARB) numFailed++;
    sf_ptrc_glUniform4ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform4ivARB");
    if(!sf_ptrc_glUniform4ivARB) numFailed++;
    sf_ptrc_glUniformMatrix2fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix2fvARB");
    if(!sf_ptrc_glUniformMatrix2fvARB) numFailed++;
    sf_ptrc_glUniformMatrix3fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix3fvARB");
    if(!sf_ptrc_glUniformMatrix3fvARB) numFailed++;
    sf_ptrc_glUniformMatrix4fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix4fvARB");
    if(!sf_ptrc_glUniformMatrix4fvARB) numFailed++;
    sf_ptrc_glUseProgramObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glUseProgramObjectARB");
    if(!sf_ptrc_glUseProgramObjectARB) numFailed++;
    sf_ptrc_glValidateProgramARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glValidateProgramARB");
    if(!sf_ptrc_glValidateProgramARB) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glBindAttribLocationARB)(GLhandleARB, GLuint, const GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetActiveAttribARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *) = NULL;
GLint (CODEGEN_FUNCPTR *sf_ptrc_glGetAttribLocationARB)(GLhandleARB, const GLcharARB *) = NULL;

static int Load_ARB_vertex_shader()
{
    int numFailed = 0;
    sf_ptrc_glBindAttribLocationARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLuint, const GLcharARB *))IntGetProcAddress("glBindAttribLocationARB");
    if(!sf_ptrc_glBindAttribLocationARB) numFailed++;
    sf_ptrc_glGetActiveAttribARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *))IntGetProcAddress("glGetActiveAttribARB");
    if(!sf_ptrc_glGetActiveAttribARB) numFailed++;
    sf_ptrc_glGetAttribLocationARB = (GLint (CODEGEN_FUNCPTR *)(GLhandleARB, const GLcharARB *))IntGetProcAddress("glGetAttribLocationARB");
    if(!sf_ptrc_glGetAttribLocationARB) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glBlendEquationSeparateEXT)(GLenum, GLenum) = NULL;

static int Load_EXT_blend_equation_separate()
{
    int numFailed = 0;
    sf_ptrc_glBlendEquationSeparateEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendEquationSeparateEXT");
    if(!sf_ptrc_glBlendEquationSeparateEXT) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glBindFramebufferEXT)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glBindRenderbufferEXT)(GLenum, GLuint) = NULL;
GLenum (CODEGEN_FUNCPTR *sf_ptrc_glCheckFramebufferStatusEXT)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteFramebuffersEXT)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteRenderbuffersEXT)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferRenderbufferEXT)(GLenum, GLenum, GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferTexture1DEXT)(GLenum, GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferTexture2DEXT)(GLenum, GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFramebufferTexture3DEXT)(GLenum, GLenum, GLenum, GLuint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGenFramebuffersEXT)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGenRenderbuffersEXT)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGenerateMipmapEXT)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetFramebufferAttachmentParameterivEXT)(GLenum, GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetRenderbufferParameterivEXT)(GLenum, GLenum, GLint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsFramebufferEXT)(GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsRenderbufferEXT)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRenderbufferStorageEXT)(GLenum, GLenum, GLsizei, GLsizei) = NULL;

static int Load_EXT_framebuffer_object()
{
    int numFailed = 0;
    sf_ptrc_glBindFramebufferEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindFramebufferEXT");
    if(!sf_ptrc_glBindFramebufferEXT) numFailed++;
    sf_ptrc_glBindRenderbufferEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindRenderbufferEXT");
    if(!sf_ptrc_glBindRenderbufferEXT) numFailed++;
    sf_ptrc_glCheckFramebufferStatusEXT = (GLenum (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCheckFramebufferStatusEXT");
    if(!sf_ptrc_glCheckFramebufferStatusEXT) numFailed++;
    sf_ptrc_glDeleteFramebuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteFramebuffersEXT");
    if(!sf_ptrc_glDeleteFramebuffersEXT) numFailed++;
    sf_ptrc_glDeleteRenderbuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteRenderbuffersEXT");
    if(!sf_ptrc_glDeleteRenderbuffersEXT) numFailed++;
    sf_ptrc_glFramebufferRenderbufferEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint))IntGetProcAddress("glFramebufferRenderbufferEXT");
    if(!sf_ptrc_glFramebufferRenderbufferEXT) numFailed++;
    sf_ptrc_glFramebufferTexture1DEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture1DEXT");
    if(!sf_ptrc_glFramebufferTexture1DEXT) numFailed++;
    sf_ptrc_glFramebufferTexture2DEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture2DEXT");
    if(!sf_ptrc_glFramebufferTexture2DEXT) numFailed++;
    sf_ptrc_glFramebufferTexture3DEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint, GLint))IntGetProcAddress("glFramebufferTexture3DEXT");
    if(!sf_ptrc_glFramebufferTexture3DEXT) numFailed++;
    sf_ptrc_glGenFramebuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenFramebuffersEXT");
    if(!sf_ptrc_glGenFramebuffersEXT) numFailed++;
    sf_ptrc_glGenRenderbuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenRenderbuffersEXT");
    if(!sf_ptrc_glGenRenderbuffersEXT) numFailed++;
    sf_ptrc_glGenerateMipmapEXT = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGenerateMipmapEXT");
    if(!sf_ptrc_glGenerateMipmapEXT) numFailed++;
    sf_ptrc_glGetFramebufferAttachmentParameterivEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLint *))IntGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
    if(!sf_ptrc_glGetFramebufferAttachmentParameterivEXT) numFailed++;
    sf_ptrc_glGetRenderbufferParameterivEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetRenderbufferParameterivEXT");
    if(!sf_ptrc_glGetRenderbufferParameterivEXT) numFailed++;
    sf_ptrc_glIsFramebufferEXT = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsFramebufferEXT");
    if(!sf_ptrc_glIsFramebufferEXT) numFailed++;
    sf_ptrc_glIsRenderbufferEXT = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsRenderbufferEXT");
    if(!sf_ptrc_glIsRenderbufferEXT) numFailed++;
    sf_ptrc_glRenderbufferStorageEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLsizei, GLsizei))IntGetProcAddress("glRenderbufferStorageEXT");
    if(!sf_ptrc_glRenderbufferStorageEXT) numFailed++;
    return numFailed;
}

void (CODEGEN_FUNCPTR *sf_ptrc_glAccum)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glAlphaFunc)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glBegin)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glBitmap)(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glBlendFunc)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCallList)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCallLists)(GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClear)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearAccum)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearDepth)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearIndex)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearStencil)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClipPlane)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3b)(GLbyte, GLbyte, GLbyte) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3bv)(const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3ub)(GLubyte, GLubyte, GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3ubv)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3ui)(GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3uiv)(const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3us)(GLushort, GLushort, GLushort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor3usv)(const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4b)(GLbyte, GLbyte, GLbyte, GLbyte) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4bv)(const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4ub)(GLubyte, GLubyte, GLubyte, GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4ubv)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4ui)(GLuint, GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4uiv)(const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4us)(GLushort, GLushort, GLushort, GLushort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColor4usv)(const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColorMask)(GLboolean, GLboolean, GLboolean, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColorMaterial)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyPixels)(GLint, GLint, GLsizei, GLsizei, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCullFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteLists)(GLuint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDepthFunc)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDepthMask)(GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDepthRange)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDisable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDrawBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDrawPixels)(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEdgeFlag)(GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEdgeFlagv)(const GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEnable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEnd)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEndList)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord1d)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord1dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord1f)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord1fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalCoord2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalMesh1)(GLenum, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalMesh2)(GLenum, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalPoint1)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEvalPoint2)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFeedbackBuffer)(GLsizei, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFinish)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFlush)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFogf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFogfv)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFogi)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFogiv)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFrontFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFrustum)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
GLuint (CODEGEN_FUNCPTR *sf_ptrc_glGenLists)(GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetBooleanv)(GLenum, GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetClipPlane)(GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetDoublev)(GLenum, GLdouble *) = NULL;
GLenum (CODEGEN_FUNCPTR *sf_ptrc_glGetError)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetFloatv)(GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetIntegerv)(GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetLightfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetLightiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetMapdv)(GLenum, GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetMapfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetMapiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetMaterialfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetMaterialiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetPixelMapfv)(GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetPixelMapuiv)(GLenum, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetPixelMapusv)(GLenum, GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetPolygonStipple)(GLubyte *) = NULL;
const GLubyte * (CODEGEN_FUNCPTR *sf_ptrc_glGetString)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexEnvfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexEnviv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexGendv)(GLenum, GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexGenfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexGeniv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexImage)(GLenum, GLint, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexLevelParameterfv)(GLenum, GLint, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexLevelParameteriv)(GLenum, GLint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexParameterfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetTexParameteriv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glHint)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexMask)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexd)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexdv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexf)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexfv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexi)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexiv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexs)(GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexsv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glInitNames)() = NULL;
GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsEnabled)(GLenum) = NULL;
GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsList)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightModelf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightModelfv)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightModeli)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightModeliv)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLighti)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLightiv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLineStipple)(GLint, GLushort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLineWidth)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glListBase)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadIdentity)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadMatrixd)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadMatrixf)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadName)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLogicOp)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMap1d)(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMap1f)(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMap2d)(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMap2f)(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMapGrid1d)(GLint, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMapGrid1f)(GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMapGrid2d)(GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMapGrid2f)(GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMaterialf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMaterialfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMateriali)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMaterialiv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMatrixMode)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultMatrixd)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultMatrixf)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNewList)(GLuint, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3b)(GLbyte, GLbyte, GLbyte) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3bv)(const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormal3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glOrtho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPassThrough)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelMapfv)(GLenum, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelMapuiv)(GLenum, GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelMapusv)(GLenum, GLsizei, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelStoref)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelStorei)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelTransferf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelTransferi)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPixelZoom)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPointSize)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPolygonMode)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPolygonStipple)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPopAttrib)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPopMatrix)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPopName)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPushAttrib)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPushMatrix)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPushName)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2s)(GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos2sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRasterPos4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glReadBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRectd)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRectdv)(const GLdouble *, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRectf)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRectfv)(const GLfloat *, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRecti)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRectiv)(const GLint *, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRects)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRectsv)(const GLshort *, const GLshort *) = NULL;
GLint (CODEGEN_FUNCPTR *sf_ptrc_glRenderMode)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRotated)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glRotatef)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glScaled)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glScalef)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glScissor)(GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glSelectBuffer)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glShadeModel)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glStencilFunc)(GLenum, GLint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glStencilMask)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glStencilOp)(GLenum, GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1d)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1f)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1i)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1s)(GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord1sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2s)(GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord2sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoord4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexEnvf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexEnvfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexEnvi)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexEnviv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexGend)(GLenum, GLenum, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexGendv)(GLenum, GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexGenf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexGenfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexGeni)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexGeniv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexImage1D)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameterf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameterfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameteri)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameteriv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTranslated)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTranslatef)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2s)(GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex2sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertex4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glViewport)(GLint, GLint, GLsizei, GLsizei) = NULL;

GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glAreTexturesResident)(GLsizei, const GLuint *, GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glArrayElement)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glBindTexture)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColorPointer)(GLint, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyTexImage1D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyTexImage2D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyTexSubImage1D)(GLenum, GLint, GLint, GLint, GLint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyTexSubImage2D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDeleteTextures)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDisableClientState)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDrawArrays)(GLenum, GLint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDrawElements)(GLenum, GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEdgeFlagPointer)(GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEnableClientState)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGenTextures)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetPointerv)(GLenum, GLvoid **) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexPointer)(GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexub)(GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glIndexubv)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glInterleavedArrays)(GLenum, GLsizei, const GLvoid *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsTexture)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glNormalPointer)(GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPolygonOffset)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPopClientAttrib)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPrioritizeTextures)(GLsizei, const GLuint *, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPushClientAttrib)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexCoordPointer)(GLint, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexSubImage1D)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glVertexPointer)(GLint, GLenum, GLsizei, const GLvoid *) = NULL;

void (CODEGEN_FUNCPTR *sf_ptrc_glBlendColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glBlendEquation)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDrawRangeElements)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;

static int Load_Version_1_2()
{
    int numFailed = 0;
    sf_ptrc_glAccum = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glAccum");
    if(!sf_ptrc_glAccum) numFailed++;
    sf_ptrc_glAlphaFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glAlphaFunc");
    if(!sf_ptrc_glAlphaFunc) numFailed++;
    sf_ptrc_glBegin = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBegin");
    if(!sf_ptrc_glBegin) numFailed++;
    sf_ptrc_glBitmap = (void (CODEGEN_FUNCPTR *)(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *))IntGetProcAddress("glBitmap");
    if(!sf_ptrc_glBitmap) numFailed++;
    sf_ptrc_glBlendFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendFunc");
    if(!sf_ptrc_glBlendFunc) numFailed++;
    sf_ptrc_glCallList = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glCallList");
    if(!sf_ptrc_glCallList) numFailed++;
    sf_ptrc_glCallLists = (void (CODEGEN_FUNCPTR *)(GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glCallLists");
    if(!sf_ptrc_glCallLists) numFailed++;
    sf_ptrc_glClear = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glClear");
    if(!sf_ptrc_glClear) numFailed++;
    sf_ptrc_glClearAccum = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glClearAccum");
    if(!sf_ptrc_glClearAccum) numFailed++;
    sf_ptrc_glClearColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glClearColor");
    if(!sf_ptrc_glClearColor) numFailed++;
    sf_ptrc_glClearDepth = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glClearDepth");
    if(!sf_ptrc_glClearDepth) numFailed++;
    sf_ptrc_glClearIndex = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glClearIndex");
    if(!sf_ptrc_glClearIndex) numFailed++;
    sf_ptrc_glClearStencil = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glClearStencil");
    if(!sf_ptrc_glClearStencil) numFailed++;
    sf_ptrc_glClipPlane = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glClipPlane");
    if(!sf_ptrc_glClipPlane) numFailed++;
    sf_ptrc_glColor3b = (void (CODEGEN_FUNCPTR *)(GLbyte, GLbyte, GLbyte))IntGetProcAddress("glColor3b");
    if(!sf_ptrc_glColor3b) numFailed++;
    sf_ptrc_glColor3bv = (void (CODEGEN_FUNCPTR *)(const GLbyte *))IntGetProcAddress("glColor3bv");
    if(!sf_ptrc_glColor3bv) numFailed++;
    sf_ptrc_glColor3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glColor3d");
    if(!sf_ptrc_glColor3d) numFailed++;
    sf_ptrc_glColor3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glColor3dv");
    if(!sf_ptrc_glColor3dv) numFailed++;
    sf_ptrc_glColor3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glColor3f");
    if(!sf_ptrc_glColor3f) numFailed++;
    sf_ptrc_glColor3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glColor3fv");
    if(!sf_ptrc_glColor3fv) numFailed++;
    sf_ptrc_glColor3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glColor3i");
    if(!sf_ptrc_glColor3i) numFailed++;
    sf_ptrc_glColor3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glColor3iv");
    if(!sf_ptrc_glColor3iv) numFailed++;
    sf_ptrc_glColor3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glColor3s");
    if(!sf_ptrc_glColor3s) numFailed++;
    sf_ptrc_glColor3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glColor3sv");
    if(!sf_ptrc_glColor3sv) numFailed++;
    sf_ptrc_glColor3ub = (void (CODEGEN_FUNCPTR *)(GLubyte, GLubyte, GLubyte))IntGetProcAddress("glColor3ub");
    if(!sf_ptrc_glColor3ub) numFailed++;
    sf_ptrc_glColor3ubv = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glColor3ubv");
    if(!sf_ptrc_glColor3ubv) numFailed++;
    sf_ptrc_glColor3ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint))IntGetProcAddress("glColor3ui");
    if(!sf_ptrc_glColor3ui) numFailed++;
    sf_ptrc_glColor3uiv = (void (CODEGEN_FUNCPTR *)(const GLuint *))IntGetProcAddress("glColor3uiv");
    if(!sf_ptrc_glColor3uiv) numFailed++;
    sf_ptrc_glColor3us = (void (CODEGEN_FUNCPTR *)(GLushort, GLushort, GLushort))IntGetProcAddress("glColor3us");
    if(!sf_ptrc_glColor3us) numFailed++;
    sf_ptrc_glColor3usv = (void (CODEGEN_FUNCPTR *)(const GLushort *))IntGetProcAddress("glColor3usv");
    if(!sf_ptrc_glColor3usv) numFailed++;
    sf_ptrc_glColor4b = (void (CODEGEN_FUNCPTR *)(GLbyte, GLbyte, GLbyte, GLbyte))IntGetProcAddress("glColor4b");
    if(!sf_ptrc_glColor4b) numFailed++;
    sf_ptrc_glColor4bv = (void (CODEGEN_FUNCPTR *)(const GLbyte *))IntGetProcAddress("glColor4bv");
    if(!sf_ptrc_glColor4bv) numFailed++;
    sf_ptrc_glColor4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glColor4d");
    if(!sf_ptrc_glColor4d) numFailed++;
    sf_ptrc_glColor4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glColor4dv");
    if(!sf_ptrc_glColor4dv) numFailed++;
    sf_ptrc_glColor4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glColor4f");
    if(!sf_ptrc_glColor4f) numFailed++;
    sf_ptrc_glColor4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glColor4fv");
    if(!sf_ptrc_glColor4fv) numFailed++;
    sf_ptrc_glColor4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glColor4i");
    if(!sf_ptrc_glColor4i) numFailed++;
    sf_ptrc_glColor4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glColor4iv");
    if(!sf_ptrc_glColor4iv) numFailed++;
    sf_ptrc_glColor4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glColor4s");
    if(!sf_ptrc_glColor4s) numFailed++;
    sf_ptrc_glColor4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glColor4sv");
    if(!sf_ptrc_glColor4sv) numFailed++;
    sf_ptrc_glColor4ub = (void (CODEGEN_FUNCPTR *)(GLubyte, GLubyte, GLubyte, GLubyte))IntGetProcAddress("glColor4ub");
    if(!sf_ptrc_glColor4ub) numFailed++;
    sf_ptrc_glColor4ubv = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glColor4ubv");
    if(!sf_ptrc_glColor4ubv) numFailed++;
    sf_ptrc_glColor4ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint, GLuint))IntGetProcAddress("glColor4ui");
    if(!sf_ptrc_glColor4ui) numFailed++;
    sf_ptrc_glColor4uiv = (void (CODEGEN_FUNCPTR *)(const GLuint *))IntGetProcAddress("glColor4uiv");
    if(!sf_ptrc_glColor4uiv) numFailed++;
    sf_ptrc_glColor4us = (void (CODEGEN_FUNCPTR *)(GLushort, GLushort, GLushort, GLushort))IntGetProcAddress("glColor4us");
    if(!sf_ptrc_glColor4us) numFailed++;
    sf_ptrc_glColor4usv = (void (CODEGEN_FUNCPTR *)(const GLushort *))IntGetProcAddress("glColor4usv");
    if(!sf_ptrc_glColor4usv) numFailed++;
    sf_ptrc_glColorMask = (void (CODEGEN_FUNCPTR *)(GLboolean, GLboolean, GLboolean, GLboolean))IntGetProcAddress("glColorMask");
    if(!sf_ptrc_glColorMask) numFailed++;
    sf_ptrc_glColorMaterial = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glColorMaterial");
    if(!sf_ptrc_glColorMaterial) numFailed++;
    sf_ptrc_glCopyPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum))IntGetProcAddress("glCopyPixels");
    if(!sf_ptrc_glCopyPixels) numFailed++;
    sf_ptrc_glCullFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCullFace");
    if(!sf_ptrc_glCullFace) numFailed++;
    sf_ptrc_glDeleteLists = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei))IntGetProcAddress("glDeleteLists");
    if(!sf_ptrc_glDeleteLists) numFailed++;
    sf_ptrc_glDepthFunc = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDepthFunc");
    if(!sf_ptrc_glDepthFunc) numFailed++;
    sf_ptrc_glDepthMask = (void (CODEGEN_FUNCPTR *)(GLboolean))IntGetProcAddress("glDepthMask");
    if(!sf_ptrc_glDepthMask) numFailed++;
    sf_ptrc_glDepthRange = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glDepthRange");
    if(!sf_ptrc_glDepthRange) numFailed++;
    sf_ptrc_glDisable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDisable");
    if(!sf_ptrc_glDisable) numFailed++;
    sf_ptrc_glDrawBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDrawBuffer");
    if(!sf_ptrc_glDrawBuffer) numFailed++;
    sf_ptrc_glDrawPixels = (void (CODEGEN_FUNCPTR *)(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glDrawPixels");
    if(!sf_ptrc_glDrawPixels) numFailed++;
    sf_ptrc_glEdgeFlag = (void (CODEGEN_FUNCPTR *)(GLboolean))IntGetProcAddress("glEdgeFlag");
    if(!sf_ptrc_glEdgeFlag) numFailed++;
    sf_ptrc_glEdgeFlagv = (void (CODEGEN_FUNCPTR *)(const GLboolean *))IntGetProcAddress("glEdgeFlagv");
    if(!sf_ptrc_glEdgeFlagv) numFailed++;
    sf_ptrc_glEnable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnable");
    if(!sf_ptrc_glEnable) numFailed++;
    sf_ptrc_glEnd = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glEnd");
    if(!sf_ptrc_glEnd) numFailed++;
    sf_ptrc_glEndList = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glEndList");
    if(!sf_ptrc_glEndList) numFailed++;
    sf_ptrc_glEvalCoord1d = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glEvalCoord1d");
    if(!sf_ptrc_glEvalCoord1d) numFailed++;
    sf_ptrc_glEvalCoord1dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glEvalCoord1dv");
    if(!sf_ptrc_glEvalCoord1dv) numFailed++;
    sf_ptrc_glEvalCoord1f = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glEvalCoord1f");
    if(!sf_ptrc_glEvalCoord1f) numFailed++;
    sf_ptrc_glEvalCoord1fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glEvalCoord1fv");
    if(!sf_ptrc_glEvalCoord1fv) numFailed++;
    sf_ptrc_glEvalCoord2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glEvalCoord2d");
    if(!sf_ptrc_glEvalCoord2d) numFailed++;
    sf_ptrc_glEvalCoord2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glEvalCoord2dv");
    if(!sf_ptrc_glEvalCoord2dv) numFailed++;
    sf_ptrc_glEvalCoord2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glEvalCoord2f");
    if(!sf_ptrc_glEvalCoord2f) numFailed++;
    sf_ptrc_glEvalCoord2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glEvalCoord2fv");
    if(!sf_ptrc_glEvalCoord2fv) numFailed++;
    sf_ptrc_glEvalMesh1 = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint))IntGetProcAddress("glEvalMesh1");
    if(!sf_ptrc_glEvalMesh1) numFailed++;
    sf_ptrc_glEvalMesh2 = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint))IntGetProcAddress("glEvalMesh2");
    if(!sf_ptrc_glEvalMesh2) numFailed++;
    sf_ptrc_glEvalPoint1 = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glEvalPoint1");
    if(!sf_ptrc_glEvalPoint1) numFailed++;
    sf_ptrc_glEvalPoint2 = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glEvalPoint2");
    if(!sf_ptrc_glEvalPoint2) numFailed++;
    sf_ptrc_glFeedbackBuffer = (void (CODEGEN_FUNCPTR *)(GLsizei, GLenum, GLfloat *))IntGetProcAddress("glFeedbackBuffer");
    if(!sf_ptrc_glFeedbackBuffer) numFailed++;
    sf_ptrc_glFinish = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glFinish");
    if(!sf_ptrc_glFinish) numFailed++;
    sf_ptrc_glFlush = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glFlush");
    if(!sf_ptrc_glFlush) numFailed++;
    sf_ptrc_glFogf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glFogf");
    if(!sf_ptrc_glFogf) numFailed++;
    sf_ptrc_glFogfv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glFogfv");
    if(!sf_ptrc_glFogfv) numFailed++;
    sf_ptrc_glFogi = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glFogi");
    if(!sf_ptrc_glFogi) numFailed++;
    sf_ptrc_glFogiv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glFogiv");
    if(!sf_ptrc_glFogiv) numFailed++;
    sf_ptrc_glFrontFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glFrontFace");
    if(!sf_ptrc_glFrontFace) numFailed++;
    sf_ptrc_glFrustum = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glFrustum");
    if(!sf_ptrc_glFrustum) numFailed++;
    sf_ptrc_glGenLists = (GLuint (CODEGEN_FUNCPTR *)(GLsizei))IntGetProcAddress("glGenLists");
    if(!sf_ptrc_glGenLists) numFailed++;
    sf_ptrc_glGetBooleanv = (void (CODEGEN_FUNCPTR *)(GLenum, GLboolean *))IntGetProcAddress("glGetBooleanv");
    if(!sf_ptrc_glGetBooleanv) numFailed++;
    sf_ptrc_glGetClipPlane = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble *))IntGetProcAddress("glGetClipPlane");
    if(!sf_ptrc_glGetClipPlane) numFailed++;
    sf_ptrc_glGetDoublev = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble *))IntGetProcAddress("glGetDoublev");
    if(!sf_ptrc_glGetDoublev) numFailed++;
    sf_ptrc_glGetError = (GLenum (CODEGEN_FUNCPTR *)())IntGetProcAddress("glGetError");
    if(!sf_ptrc_glGetError) numFailed++;
    sf_ptrc_glGetFloatv = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat *))IntGetProcAddress("glGetFloatv");
    if(!sf_ptrc_glGetFloatv) numFailed++;
    sf_ptrc_glGetIntegerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint *))IntGetProcAddress("glGetIntegerv");
    if(!sf_ptrc_glGetIntegerv) numFailed++;
    sf_ptrc_glGetLightfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetLightfv");
    if(!sf_ptrc_glGetLightfv) numFailed++;
    sf_ptrc_glGetLightiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetLightiv");
    if(!sf_ptrc_glGetLightiv) numFailed++;
    sf_ptrc_glGetMapdv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLdouble *))IntGetProcAddress("glGetMapdv");
    if(!sf_ptrc_glGetMapdv) numFailed++;
    sf_ptrc_glGetMapfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetMapfv");
    if(!sf_ptrc_glGetMapfv) numFailed++;
    sf_ptrc_glGetMapiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetMapiv");
    if(!sf_ptrc_glGetMapiv) numFailed++;
    sf_ptrc_glGetMaterialfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetMaterialfv");
    if(!sf_ptrc_glGetMaterialfv) numFailed++;
    sf_ptrc_glGetMaterialiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetMaterialiv");
    if(!sf_ptrc_glGetMaterialiv) numFailed++;
    sf_ptrc_glGetPixelMapfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat *))IntGetProcAddress("glGetPixelMapfv");
    if(!sf_ptrc_glGetPixelMapfv) numFailed++;
    sf_ptrc_glGetPixelMapuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint *))IntGetProcAddress("glGetPixelMapuiv");
    if(!sf_ptrc_glGetPixelMapuiv) numFailed++;
    sf_ptrc_glGetPixelMapusv = (void (CODEGEN_FUNCPTR *)(GLenum, GLushort *))IntGetProcAddress("glGetPixelMapusv");
    if(!sf_ptrc_glGetPixelMapusv) numFailed++;
    sf_ptrc_glGetPolygonStipple = (void (CODEGEN_FUNCPTR *)(GLubyte *))IntGetProcAddress("glGetPolygonStipple");
    if(!sf_ptrc_glGetPolygonStipple) numFailed++;
    sf_ptrc_glGetString = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetString");
    if(!sf_ptrc_glGetString) numFailed++;
    sf_ptrc_glGetTexEnvfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexEnvfv");
    if(!sf_ptrc_glGetTexEnvfv) numFailed++;
    sf_ptrc_glGetTexEnviv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexEnviv");
    if(!sf_ptrc_glGetTexEnviv) numFailed++;
    sf_ptrc_glGetTexGendv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLdouble *))IntGetProcAddress("glGetTexGendv");
    if(!sf_ptrc_glGetTexGendv) numFailed++;
    sf_ptrc_glGetTexGenfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexGenfv");
    if(!sf_ptrc_glGetTexGenfv) numFailed++;
    sf_ptrc_glGetTexGeniv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexGeniv");
    if(!sf_ptrc_glGetTexGeniv) numFailed++;
    sf_ptrc_glGetTexImage = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLenum, GLvoid *))IntGetProcAddress("glGetTexImage");
    if(!sf_ptrc_glGetTexImage) numFailed++;
    sf_ptrc_glGetTexLevelParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLfloat *))IntGetProcAddress("glGetTexLevelParameterfv");
    if(!sf_ptrc_glGetTexLevelParameterfv) numFailed++;
    sf_ptrc_glGetTexLevelParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint *))IntGetProcAddress("glGetTexLevelParameteriv");
    if(!sf_ptrc_glGetTexLevelParameteriv) numFailed++;
    sf_ptrc_glGetTexParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexParameterfv");
    if(!sf_ptrc_glGetTexParameterfv) numFailed++;
    sf_ptrc_glGetTexParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexParameteriv");
    if(!sf_ptrc_glGetTexParameteriv) numFailed++;
    sf_ptrc_glHint = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glHint");
    if(!sf_ptrc_glHint) numFailed++;
    sf_ptrc_glIndexMask = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIndexMask");
    if(!sf_ptrc_glIndexMask) numFailed++;
    sf_ptrc_glIndexd = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glIndexd");
    if(!sf_ptrc_glIndexd) numFailed++;
    sf_ptrc_glIndexdv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glIndexdv");
    if(!sf_ptrc_glIndexdv) numFailed++;
    sf_ptrc_glIndexf = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glIndexf");
    if(!sf_ptrc_glIndexf) numFailed++;
    sf_ptrc_glIndexfv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glIndexfv");
    if(!sf_ptrc_glIndexfv) numFailed++;
    sf_ptrc_glIndexi = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glIndexi");
    if(!sf_ptrc_glIndexi) numFailed++;
    sf_ptrc_glIndexiv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glIndexiv");
    if(!sf_ptrc_glIndexiv) numFailed++;
    sf_ptrc_glIndexs = (void (CODEGEN_FUNCPTR *)(GLshort))IntGetProcAddress("glIndexs");
    if(!sf_ptrc_glIndexs) numFailed++;
    sf_ptrc_glIndexsv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glIndexsv");
    if(!sf_ptrc_glIndexsv) numFailed++;
    sf_ptrc_glInitNames = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glInitNames");
    if(!sf_ptrc_glInitNames) numFailed++;
    sf_ptrc_glIsEnabled = (GLboolean (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glIsEnabled");
    if(!sf_ptrc_glIsEnabled) numFailed++;
    sf_ptrc_glIsList = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsList");
    if(!sf_ptrc_glIsList) numFailed++;
    sf_ptrc_glLightModelf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glLightModelf");
    if(!sf_ptrc_glLightModelf) numFailed++;
    sf_ptrc_glLightModelfv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glLightModelfv");
    if(!sf_ptrc_glLightModelfv) numFailed++;
    sf_ptrc_glLightModeli = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glLightModeli");
    if(!sf_ptrc_glLightModeli) numFailed++;
    sf_ptrc_glLightModeliv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glLightModeliv");
    if(!sf_ptrc_glLightModeliv) numFailed++;
    sf_ptrc_glLightf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glLightf");
    if(!sf_ptrc_glLightf) numFailed++;
    sf_ptrc_glLightfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glLightfv");
    if(!sf_ptrc_glLightfv) numFailed++;
    sf_ptrc_glLighti = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glLighti");
    if(!sf_ptrc_glLighti) numFailed++;
    sf_ptrc_glLightiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glLightiv");
    if(!sf_ptrc_glLightiv) numFailed++;
    sf_ptrc_glLineStipple = (void (CODEGEN_FUNCPTR *)(GLint, GLushort))IntGetProcAddress("glLineStipple");
    if(!sf_ptrc_glLineStipple) numFailed++;
    sf_ptrc_glLineWidth = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glLineWidth");
    if(!sf_ptrc_glLineWidth) numFailed++;
    sf_ptrc_glListBase = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glListBase");
    if(!sf_ptrc_glListBase) numFailed++;
    sf_ptrc_glLoadIdentity = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glLoadIdentity");
    if(!sf_ptrc_glLoadIdentity) numFailed++;
    sf_ptrc_glLoadMatrixd = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glLoadMatrixd");
    if(!sf_ptrc_glLoadMatrixd) numFailed++;
    sf_ptrc_glLoadMatrixf = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glLoadMatrixf");
    if(!sf_ptrc_glLoadMatrixf) numFailed++;
    sf_ptrc_glLoadName = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glLoadName");
    if(!sf_ptrc_glLoadName) numFailed++;
    sf_ptrc_glLogicOp = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glLogicOp");
    if(!sf_ptrc_glLogicOp) numFailed++;
    sf_ptrc_glMap1d = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *))IntGetProcAddress("glMap1d");
    if(!sf_ptrc_glMap1d) numFailed++;
    sf_ptrc_glMap1f = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *))IntGetProcAddress("glMap1f");
    if(!sf_ptrc_glMap1f) numFailed++;
    sf_ptrc_glMap2d = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *))IntGetProcAddress("glMap2d");
    if(!sf_ptrc_glMap2d) numFailed++;
    sf_ptrc_glMap2f = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *))IntGetProcAddress("glMap2f");
    if(!sf_ptrc_glMap2f) numFailed++;
    sf_ptrc_glMapGrid1d = (void (CODEGEN_FUNCPTR *)(GLint, GLdouble, GLdouble))IntGetProcAddress("glMapGrid1d");
    if(!sf_ptrc_glMapGrid1d) numFailed++;
    sf_ptrc_glMapGrid1f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat))IntGetProcAddress("glMapGrid1f");
    if(!sf_ptrc_glMapGrid1f) numFailed++;
    sf_ptrc_glMapGrid2d = (void (CODEGEN_FUNCPTR *)(GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble))IntGetProcAddress("glMapGrid2d");
    if(!sf_ptrc_glMapGrid2d) numFailed++;
    sf_ptrc_glMapGrid2f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat))IntGetProcAddress("glMapGrid2f");
    if(!sf_ptrc_glMapGrid2f) numFailed++;
    sf_ptrc_glMaterialf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glMaterialf");
    if(!sf_ptrc_glMaterialf) numFailed++;
    sf_ptrc_glMaterialfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glMaterialfv");
    if(!sf_ptrc_glMaterialfv) numFailed++;
    sf_ptrc_glMateriali = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glMateriali");
    if(!sf_ptrc_glMateriali) numFailed++;
    sf_ptrc_glMaterialiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glMaterialiv");
    if(!sf_ptrc_glMaterialiv) numFailed++;
    sf_ptrc_glMatrixMode = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glMatrixMode");
    if(!sf_ptrc_glMatrixMode) numFailed++;
    sf_ptrc_glMultMatrixd = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glMultMatrixd");
    if(!sf_ptrc_glMultMatrixd) numFailed++;
    sf_ptrc_glMultMatrixf = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glMultMatrixf");
    if(!sf_ptrc_glMultMatrixf) numFailed++;
    sf_ptrc_glNewList = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum))IntGetProcAddress("glNewList");
    if(!sf_ptrc_glNewList) numFailed++;
    sf_ptrc_glNormal3b = (void (CODEGEN_FUNCPTR *)(GLbyte, GLbyte, GLbyte))IntGetProcAddress("glNormal3b");
    if(!sf_ptrc_glNormal3b) numFailed++;
    sf_ptrc_glNormal3bv = (void (CODEGEN_FUNCPTR *)(const GLbyte *))IntGetProcAddress("glNormal3bv");
    if(!sf_ptrc_glNormal3bv) numFailed++;
    sf_ptrc_glNormal3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glNormal3d");
    if(!sf_ptrc_glNormal3d) numFailed++;
    sf_ptrc_glNormal3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glNormal3dv");
    if(!sf_ptrc_glNormal3dv) numFailed++;
    sf_ptrc_glNormal3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glNormal3f");
    if(!sf_ptrc_glNormal3f) numFailed++;
    sf_ptrc_glNormal3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glNormal3fv");
    if(!sf_ptrc_glNormal3fv) numFailed++;
    sf_ptrc_glNormal3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glNormal3i");
    if(!sf_ptrc_glNormal3i) numFailed++;
    sf_ptrc_glNormal3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glNormal3iv");
    if(!sf_ptrc_glNormal3iv) numFailed++;
    sf_ptrc_glNormal3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glNormal3s");
    if(!sf_ptrc_glNormal3s) numFailed++;
    sf_ptrc_glNormal3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glNormal3sv");
    if(!sf_ptrc_glNormal3sv) numFailed++;
    sf_ptrc_glOrtho = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glOrtho");
    if(!sf_ptrc_glOrtho) numFailed++;
    sf_ptrc_glPassThrough = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glPassThrough");
    if(!sf_ptrc_glPassThrough) numFailed++;
    sf_ptrc_glPixelMapfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLfloat *))IntGetProcAddress("glPixelMapfv");
    if(!sf_ptrc_glPixelMapfv) numFailed++;
    sf_ptrc_glPixelMapuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLuint *))IntGetProcAddress("glPixelMapuiv");
    if(!sf_ptrc_glPixelMapuiv) numFailed++;
    sf_ptrc_glPixelMapusv = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLushort *))IntGetProcAddress("glPixelMapusv");
    if(!sf_ptrc_glPixelMapusv) numFailed++;
    sf_ptrc_glPixelStoref = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glPixelStoref");
    if(!sf_ptrc_glPixelStoref) numFailed++;
    sf_ptrc_glPixelStorei = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glPixelStorei");
    if(!sf_ptrc_glPixelStorei) numFailed++;
    sf_ptrc_glPixelTransferf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glPixelTransferf");
    if(!sf_ptrc_glPixelTransferf) numFailed++;
    sf_ptrc_glPixelTransferi = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glPixelTransferi");
    if(!sf_ptrc_glPixelTransferi) numFailed++;
    sf_ptrc_glPixelZoom = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glPixelZoom");
    if(!sf_ptrc_glPixelZoom) numFailed++;
    sf_ptrc_glPointSize = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glPointSize");
    if(!sf_ptrc_glPointSize) numFailed++;
    sf_ptrc_glPolygonMode = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glPolygonMode");
    if(!sf_ptrc_glPolygonMode) numFailed++;
    sf_ptrc_glPolygonStipple = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glPolygonStipple");
    if(!sf_ptrc_glPolygonStipple) numFailed++;
    sf_ptrc_glPopAttrib = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPopAttrib");
    if(!sf_ptrc_glPopAttrib) numFailed++;
    sf_ptrc_glPopMatrix = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPopMatrix");
    if(!sf_ptrc_glPopMatrix) numFailed++;
    sf_ptrc_glPopName = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPopName");
    if(!sf_ptrc_glPopName) numFailed++;
    sf_ptrc_glPushAttrib = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glPushAttrib");
    if(!sf_ptrc_glPushAttrib) numFailed++;
    sf_ptrc_glPushMatrix = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPushMatrix");
    if(!sf_ptrc_glPushMatrix) numFailed++;
    sf_ptrc_glPushName = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glPushName");
    if(!sf_ptrc_glPushName) numFailed++;
    sf_ptrc_glRasterPos2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glRasterPos2d");
    if(!sf_ptrc_glRasterPos2d) numFailed++;
    sf_ptrc_glRasterPos2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glRasterPos2dv");
    if(!sf_ptrc_glRasterPos2dv) numFailed++;
    sf_ptrc_glRasterPos2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glRasterPos2f");
    if(!sf_ptrc_glRasterPos2f) numFailed++;
    sf_ptrc_glRasterPos2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glRasterPos2fv");
    if(!sf_ptrc_glRasterPos2fv) numFailed++;
    sf_ptrc_glRasterPos2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glRasterPos2i");
    if(!sf_ptrc_glRasterPos2i) numFailed++;
    sf_ptrc_glRasterPos2iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glRasterPos2iv");
    if(!sf_ptrc_glRasterPos2iv) numFailed++;
    sf_ptrc_glRasterPos2s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort))IntGetProcAddress("glRasterPos2s");
    if(!sf_ptrc_glRasterPos2s) numFailed++;
    sf_ptrc_glRasterPos2sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glRasterPos2sv");
    if(!sf_ptrc_glRasterPos2sv) numFailed++;
    sf_ptrc_glRasterPos3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRasterPos3d");
    if(!sf_ptrc_glRasterPos3d) numFailed++;
    sf_ptrc_glRasterPos3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glRasterPos3dv");
    if(!sf_ptrc_glRasterPos3dv) numFailed++;
    sf_ptrc_glRasterPos3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRasterPos3f");
    if(!sf_ptrc_glRasterPos3f) numFailed++;
    sf_ptrc_glRasterPos3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glRasterPos3fv");
    if(!sf_ptrc_glRasterPos3fv) numFailed++;
    sf_ptrc_glRasterPos3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glRasterPos3i");
    if(!sf_ptrc_glRasterPos3i) numFailed++;
    sf_ptrc_glRasterPos3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glRasterPos3iv");
    if(!sf_ptrc_glRasterPos3iv) numFailed++;
    sf_ptrc_glRasterPos3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glRasterPos3s");
    if(!sf_ptrc_glRasterPos3s) numFailed++;
    sf_ptrc_glRasterPos3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glRasterPos3sv");
    if(!sf_ptrc_glRasterPos3sv) numFailed++;
    sf_ptrc_glRasterPos4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRasterPos4d");
    if(!sf_ptrc_glRasterPos4d) numFailed++;
    sf_ptrc_glRasterPos4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glRasterPos4dv");
    if(!sf_ptrc_glRasterPos4dv) numFailed++;
    sf_ptrc_glRasterPos4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRasterPos4f");
    if(!sf_ptrc_glRasterPos4f) numFailed++;
    sf_ptrc_glRasterPos4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glRasterPos4fv");
    if(!sf_ptrc_glRasterPos4fv) numFailed++;
    sf_ptrc_glRasterPos4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glRasterPos4i");
    if(!sf_ptrc_glRasterPos4i) numFailed++;
    sf_ptrc_glRasterPos4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glRasterPos4iv");
    if(!sf_ptrc_glRasterPos4iv) numFailed++;
    sf_ptrc_glRasterPos4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glRasterPos4s");
    if(!sf_ptrc_glRasterPos4s) numFailed++;
    sf_ptrc_glRasterPos4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glRasterPos4sv");
    if(!sf_ptrc_glRasterPos4sv) numFailed++;
    sf_ptrc_glReadBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glReadBuffer");
    if(!sf_ptrc_glReadBuffer) numFailed++;
    sf_ptrc_glReadPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *))IntGetProcAddress("glReadPixels");
    if(!sf_ptrc_glReadPixels) numFailed++;
    sf_ptrc_glRectd = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRectd");
    if(!sf_ptrc_glRectd) numFailed++;
    sf_ptrc_glRectdv = (void (CODEGEN_FUNCPTR *)(const GLdouble *, const GLdouble *))IntGetProcAddress("glRectdv");
    if(!sf_ptrc_glRectdv) numFailed++;
    sf_ptrc_glRectf = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRectf");
    if(!sf_ptrc_glRectf) numFailed++;
    sf_ptrc_glRectfv = (void (CODEGEN_FUNCPTR *)(const GLfloat *, const GLfloat *))IntGetProcAddress("glRectfv");
    if(!sf_ptrc_glRectfv) numFailed++;
    sf_ptrc_glRecti = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glRecti");
    if(!sf_ptrc_glRecti) numFailed++;
    sf_ptrc_glRectiv = (void (CODEGEN_FUNCPTR *)(const GLint *, const GLint *))IntGetProcAddress("glRectiv");
    if(!sf_ptrc_glRectiv) numFailed++;
    sf_ptrc_glRects = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glRects");
    if(!sf_ptrc_glRects) numFailed++;
    sf_ptrc_glRectsv = (void (CODEGEN_FUNCPTR *)(const GLshort *, const GLshort *))IntGetProcAddress("glRectsv");
    if(!sf_ptrc_glRectsv) numFailed++;
    sf_ptrc_glRenderMode = (GLint (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glRenderMode");
    if(!sf_ptrc_glRenderMode) numFailed++;
    sf_ptrc_glRotated = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRotated");
    if(!sf_ptrc_glRotated) numFailed++;
    sf_ptrc_glRotatef = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRotatef");
    if(!sf_ptrc_glRotatef) numFailed++;
    sf_ptrc_glScaled = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glScaled");
    if(!sf_ptrc_glScaled) numFailed++;
    sf_ptrc_glScalef = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glScalef");
    if(!sf_ptrc_glScalef) numFailed++;
    sf_ptrc_glScissor = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glScissor");
    if(!sf_ptrc_glScissor) numFailed++;
    sf_ptrc_glSelectBuffer = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glSelectBuffer");
    if(!sf_ptrc_glSelectBuffer) numFailed++;
    sf_ptrc_glShadeModel = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glShadeModel");
    if(!sf_ptrc_glShadeModel) numFailed++;
    sf_ptrc_glStencilFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLuint))IntGetProcAddress("glStencilFunc");
    if(!sf_ptrc_glStencilFunc) numFailed++;
    sf_ptrc_glStencilMask = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glStencilMask");
    if(!sf_ptrc_glStencilMask) numFailed++;
    sf_ptrc_glStencilOp = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum))IntGetProcAddress("glStencilOp");
    if(!sf_ptrc_glStencilOp) numFailed++;
    sf_ptrc_glTexCoord1d = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glTexCoord1d");
    if(!sf_ptrc_glTexCoord1d) numFailed++;
    sf_ptrc_glTexCoord1dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord1dv");
    if(!sf_ptrc_glTexCoord1dv) numFailed++;
    sf_ptrc_glTexCoord1f = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glTexCoord1f");
    if(!sf_ptrc_glTexCoord1f) numFailed++;
    sf_ptrc_glTexCoord1fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord1fv");
    if(!sf_ptrc_glTexCoord1fv) numFailed++;
    sf_ptrc_glTexCoord1i = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glTexCoord1i");
    if(!sf_ptrc_glTexCoord1i) numFailed++;
    sf_ptrc_glTexCoord1iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord1iv");
    if(!sf_ptrc_glTexCoord1iv) numFailed++;
    sf_ptrc_glTexCoord1s = (void (CODEGEN_FUNCPTR *)(GLshort))IntGetProcAddress("glTexCoord1s");
    if(!sf_ptrc_glTexCoord1s) numFailed++;
    sf_ptrc_glTexCoord1sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord1sv");
    if(!sf_ptrc_glTexCoord1sv) numFailed++;
    sf_ptrc_glTexCoord2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glTexCoord2d");
    if(!sf_ptrc_glTexCoord2d) numFailed++;
    sf_ptrc_glTexCoord2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord2dv");
    if(!sf_ptrc_glTexCoord2dv) numFailed++;
    sf_ptrc_glTexCoord2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glTexCoord2f");
    if(!sf_ptrc_glTexCoord2f) numFailed++;
    sf_ptrc_glTexCoord2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord2fv");
    if(!sf_ptrc_glTexCoord2fv) numFailed++;
    sf_ptrc_glTexCoord2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glTexCoord2i");
    if(!sf_ptrc_glTexCoord2i) numFailed++;
    sf_ptrc_glTexCoord2iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord2iv");
    if(!sf_ptrc_glTexCoord2iv) numFailed++;
    sf_ptrc_glTexCoord2s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort))IntGetProcAddress("glTexCoord2s");
    if(!sf_ptrc_glTexCoord2s) numFailed++;
    sf_ptrc_glTexCoord2sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord2sv");
    if(!sf_ptrc_glTexCoord2sv) numFailed++;
    sf_ptrc_glTexCoord3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glTexCoord3d");
    if(!sf_ptrc_glTexCoord3d) numFailed++;
    sf_ptrc_glTexCoord3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord3dv");
    if(!sf_ptrc_glTexCoord3dv) numFailed++;
    sf_ptrc_glTexCoord3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glTexCoord3f");
    if(!sf_ptrc_glTexCoord3f) numFailed++;
    sf_ptrc_glTexCoord3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord3fv");
    if(!sf_ptrc_glTexCoord3fv) numFailed++;
    sf_ptrc_glTexCoord3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glTexCoord3i");
    if(!sf_ptrc_glTexCoord3i) numFailed++;
    sf_ptrc_glTexCoord3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord3iv");
    if(!sf_ptrc_glTexCoord3iv) numFailed++;
    sf_ptrc_glTexCoord3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glTexCoord3s");
    if(!sf_ptrc_glTexCoord3s) numFailed++;
    sf_ptrc_glTexCoord3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord3sv");
    if(!sf_ptrc_glTexCoord3sv) numFailed++;
    sf_ptrc_glTexCoord4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glTexCoord4d");
    if(!sf_ptrc_glTexCoord4d) numFailed++;
    sf_ptrc_glTexCoord4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord4dv");
    if(!sf_ptrc_glTexCoord4dv) numFailed++;
    sf_ptrc_glTexCoord4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glTexCoord4f");
    if(!sf_ptrc_glTexCoord4f) numFailed++;
    sf_ptrc_glTexCoord4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord4fv");
    if(!sf_ptrc_glTexCoord4fv) numFailed++;
    sf_ptrc_glTexCoord4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glTexCoord4i");
    if(!sf_ptrc_glTexCoord4i) numFailed++;
    sf_ptrc_glTexCoord4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord4iv");
    if(!sf_ptrc_glTexCoord4iv) numFailed++;
    sf_ptrc_glTexCoord4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glTexCoord4s");
    if(!sf_ptrc_glTexCoord4s) numFailed++;
    sf_ptrc_glTexCoord4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord4sv");
    if(!sf_ptrc_glTexCoord4sv) numFailed++;
    sf_ptrc_glTexEnvf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexEnvf");
    if(!sf_ptrc_glTexEnvf) numFailed++;
    sf_ptrc_glTexEnvfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexEnvfv");
    if(!sf_ptrc_glTexEnvfv) numFailed++;
    sf_ptrc_glTexEnvi = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexEnvi");
    if(!sf_ptrc_glTexEnvi) numFailed++;
    sf_ptrc_glTexEnviv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexEnviv");
    if(!sf_ptrc_glTexEnviv) numFailed++;
    sf_ptrc_glTexGend = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLdouble))IntGetProcAddress("glTexGend");
    if(!sf_ptrc_glTexGend) numFailed++;
    sf_ptrc_glTexGendv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLdouble *))IntGetProcAddress("glTexGendv");
    if(!sf_ptrc_glTexGendv) numFailed++;
    sf_ptrc_glTexGenf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexGenf");
    if(!sf_ptrc_glTexGenf) numFailed++;
    sf_ptrc_glTexGenfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexGenfv");
    if(!sf_ptrc_glTexGenfv) numFailed++;
    sf_ptrc_glTexGeni = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexGeni");
    if(!sf_ptrc_glTexGeni) numFailed++;
    sf_ptrc_glTexGeniv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexGeniv");
    if(!sf_ptrc_glTexGeniv) numFailed++;
    sf_ptrc_glTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage1D");
    if(!sf_ptrc_glTexImage1D) numFailed++;
    sf_ptrc_glTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage2D");
    if(!sf_ptrc_glTexImage2D) numFailed++;
    sf_ptrc_glTexParameterf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexParameterf");
    if(!sf_ptrc_glTexParameterf) numFailed++;
    sf_ptrc_glTexParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexParameterfv");
    if(!sf_ptrc_glTexParameterfv) numFailed++;
    sf_ptrc_glTexParameteri = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexParameteri");
    if(!sf_ptrc_glTexParameteri) numFailed++;
    sf_ptrc_glTexParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexParameteriv");
    if(!sf_ptrc_glTexParameteriv) numFailed++;
    sf_ptrc_glTranslated = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glTranslated");
    if(!sf_ptrc_glTranslated) numFailed++;
    sf_ptrc_glTranslatef = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glTranslatef");
    if(!sf_ptrc_glTranslatef) numFailed++;
    sf_ptrc_glVertex2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glVertex2d");
    if(!sf_ptrc_glVertex2d) numFailed++;
    sf_ptrc_glVertex2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glVertex2dv");
    if(!sf_ptrc_glVertex2dv) numFailed++;
    sf_ptrc_glVertex2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glVertex2f");
    if(!sf_ptrc_glVertex2f) numFailed++;
    sf_ptrc_glVertex2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glVertex2fv");
    if(!sf_ptrc_glVertex2fv) numFailed++;
    sf_ptrc_glVertex2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glVertex2i");
    if(!sf_ptrc_glVertex2i) numFailed++;
    sf_ptrc_glVertex2iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glVertex2iv");
    if(!sf_ptrc_glVertex2iv) numFailed++;
    sf_ptrc_glVertex2s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort))IntGetProcAddress("glVertex2s");
    if(!sf_ptrc_glVertex2s) numFailed++;
    sf_ptrc_glVertex2sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glVertex2sv");
    if(!sf_ptrc_glVertex2sv) numFailed++;
    sf_ptrc_glVertex3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertex3d");
    if(!sf_ptrc_glVertex3d) numFailed++;
    sf_ptrc_glVertex3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glVertex3dv");
    if(!sf_ptrc_glVertex3dv) numFailed++;
    sf_ptrc_glVertex3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertex3f");
    if(!sf_ptrc_glVertex3f) numFailed++;
    sf_ptrc_glVertex3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glVertex3fv");
    if(!sf_ptrc_glVertex3fv) numFailed++;
    sf_ptrc_glVertex3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glVertex3i");
    if(!sf_ptrc_glVertex3i) numFailed++;
    sf_ptrc_glVertex3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glVertex3iv");
    if(!sf_ptrc_glVertex3iv) numFailed++;
    sf_ptrc_glVertex3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glVertex3s");
    if(!sf_ptrc_glVertex3s) numFailed++;
    sf_ptrc_glVertex3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glVertex3sv");
    if(!sf_ptrc_glVertex3sv) numFailed++;
    sf_ptrc_glVertex4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertex4d");
    if(!sf_ptrc_glVertex4d) numFailed++;
    sf_ptrc_glVertex4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glVertex4dv");
    if(!sf_ptrc_glVertex4dv) numFailed++;
    sf_ptrc_glVertex4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertex4f");
    if(!sf_ptrc_glVertex4f) numFailed++;
    sf_ptrc_glVertex4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glVertex4fv");
    if(!sf_ptrc_glVertex4fv) numFailed++;
    sf_ptrc_glVertex4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glVertex4i");
    if(!sf_ptrc_glVertex4i) numFailed++;
    sf_ptrc_glVertex4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glVertex4iv");
    if(!sf_ptrc_glVertex4iv) numFailed++;
    sf_ptrc_glVertex4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glVertex4s");
    if(!sf_ptrc_glVertex4s) numFailed++;
    sf_ptrc_glVertex4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glVertex4sv");
    if(!sf_ptrc_glVertex4sv) numFailed++;
    sf_ptrc_glViewport = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glViewport");
    if(!sf_ptrc_glViewport) numFailed++;
    sf_ptrc_glAreTexturesResident = (GLboolean (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *, GLboolean *))IntGetProcAddress("glAreTexturesResident");
    if(!sf_ptrc_glAreTexturesResident) numFailed++;
    sf_ptrc_glArrayElement = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glArrayElement");
    if(!sf_ptrc_glArrayElement) numFailed++;
    sf_ptrc_glBindTexture = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindTexture");
    if(!sf_ptrc_glBindTexture) numFailed++;
    sf_ptrc_glColorPointer = (void (CODEGEN_FUNCPTR *)(GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glColorPointer");
    if(!sf_ptrc_glColorPointer) numFailed++;
    sf_ptrc_glCopyTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint))IntGetProcAddress("glCopyTexImage1D");
    if(!sf_ptrc_glCopyTexImage1D) numFailed++;
    sf_ptrc_glCopyTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint))IntGetProcAddress("glCopyTexImage2D");
    if(!sf_ptrc_glCopyTexImage2D) numFailed++;
    sf_ptrc_glCopyTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei))IntGetProcAddress("glCopyTexSubImage1D");
    if(!sf_ptrc_glCopyTexSubImage1D) numFailed++;
    sf_ptrc_glCopyTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glCopyTexSubImage2D");
    if(!sf_ptrc_glCopyTexSubImage2D) numFailed++;
    sf_ptrc_glDeleteTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteTextures");
    if(!sf_ptrc_glDeleteTextures) numFailed++;
    sf_ptrc_glDisableClientState = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDisableClientState");
    if(!sf_ptrc_glDisableClientState) numFailed++;
    sf_ptrc_glDrawArrays = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLsizei))IntGetProcAddress("glDrawArrays");
    if(!sf_ptrc_glDrawArrays) numFailed++;
    sf_ptrc_glDrawElements = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glDrawElements");
    if(!sf_ptrc_glDrawElements) numFailed++;
    sf_ptrc_glEdgeFlagPointer = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLvoid *))IntGetProcAddress("glEdgeFlagPointer");
    if(!sf_ptrc_glEdgeFlagPointer) numFailed++;
    sf_ptrc_glEnableClientState = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnableClientState");
    if(!sf_ptrc_glEnableClientState) numFailed++;
    sf_ptrc_glGenTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenTextures");
    if(!sf_ptrc_glGenTextures) numFailed++;
    sf_ptrc_glGetPointerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLvoid **))IntGetProcAddress("glGetPointerv");
    if(!sf_ptrc_glGetPointerv) numFailed++;
    sf_ptrc_glIndexPointer = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glIndexPointer");
    if(!sf_ptrc_glIndexPointer) numFailed++;
    sf_ptrc_glIndexub = (void (CODEGEN_FUNCPTR *)(GLubyte))IntGetProcAddress("glIndexub");
    if(!sf_ptrc_glIndexub) numFailed++;
    sf_ptrc_glIndexubv = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glIndexubv");
    if(!sf_ptrc_glIndexubv) numFailed++;
    sf_ptrc_glInterleavedArrays = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glInterleavedArrays");
    if(!sf_ptrc_glInterleavedArrays) numFailed++;
    sf_ptrc_glIsTexture = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsTexture");
    if(!sf_ptrc_glIsTexture) numFailed++;
    sf_ptrc_glNormalPointer = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glNormalPointer");
    if(!sf_ptrc_glNormalPointer) numFailed++;
    sf_ptrc_glPolygonOffset = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glPolygonOffset");
    if(!sf_ptrc_glPolygonOffset) numFailed++;
    sf_ptrc_glPopClientAttrib = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPopClientAttrib");
    if(!sf_ptrc_glPopClientAttrib) numFailed++;
    sf_ptrc_glPrioritizeTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *, const GLfloat *))IntGetProcAddress("glPrioritizeTextures");
    if(!sf_ptrc_glPrioritizeTextures) numFailed++;
    sf_ptrc_glPushClientAttrib = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glPushClientAttrib");
    if(!sf_ptrc_glPushClientAttrib) numFailed++;
    sf_ptrc_glTexCoordPointer = (void (CODEGEN_FUNCPTR *)(GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glTexCoordPointer");
    if(!sf_ptrc_glTexCoordPointer) numFailed++;
    sf_ptrc_glTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage1D");
    if(!sf_ptrc_glTexSubImage1D) numFailed++;
    sf_ptrc_glTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage2D");
    if(!sf_ptrc_glTexSubImage2D) numFailed++;
    sf_ptrc_glVertexPointer = (void (CODEGEN_FUNCPTR *)(GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glVertexPointer");
    if(!sf_ptrc_glVertexPointer) numFailed++;
    sf_ptrc_glBlendColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glBlendColor");
    if(!sf_ptrc_glBlendColor) numFailed++;
    sf_ptrc_glBlendEquation = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBlendEquation");
    if(!sf_ptrc_glBlendEquation) numFailed++;
    sf_ptrc_glCopyTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glCopyTexSubImage3D");
    if(!sf_ptrc_glCopyTexSubImage3D) numFailed++;
    sf_ptrc_glDrawRangeElements = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glDrawRangeElements");
    if(!sf_ptrc_glDrawRangeElements) numFailed++;
    sf_ptrc_glTexImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage3D");
    if(!sf_ptrc_glTexImage3D) numFailed++;
    sf_ptrc_glTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage3D");
    if(!sf_ptrc_glTexSubImage3D) numFailed++;
    return numFailed;
}

typedef int (*PFN_LOADFUNCPOINTERS)();
typedef struct sfogl_StrToExtMap_s
{
    const char *extensionName;
    int *extensionVariable;
    PFN_LOADFUNCPOINTERS LoadExtension;
} sfogl_StrToExtMap;

static sfogl_StrToExtMap ExtensionMap[11] = {
    {"GL_EXT_blend_minmax", &sfogl_ext_EXT_blend_minmax, Load_EXT_blend_minmax},
    {"GL_EXT_blend_subtract", &sfogl_ext_EXT_blend_subtract, NULL},
    {"GL_ARB_multitexture", &sfogl_ext_ARB_multitexture, Load_ARB_multitexture},
    {"GL_EXT_blend_func_separate", &sfogl_ext_EXT_blend_func_separate, Load_EXT_blend_func_separate},
    {"GL_ARB_shading_language_100", &sfogl_ext_ARB_shading_language_100, NULL},
    {"GL_ARB_shader_objects", &sfogl_ext_ARB_shader_objects, Load_ARB_shader_objects},
    {"GL_ARB_vertex_shader", &sfogl_ext_ARB_vertex_shader, Load_ARB_vertex_shader},
    {"GL_ARB_fragment_shader", &sfogl_ext_ARB_fragment_shader, NULL},
    {"GL_ARB_texture_non_power_of_two", &sfogl_ext_ARB_texture_non_power_of_two, NULL},
    {"GL_EXT_blend_equation_separate", &sfogl_ext_EXT_blend_equation_separate, Load_EXT_blend_equation_separate},
    {"GL_EXT_framebuffer_object", &sfogl_ext_EXT_framebuffer_object, Load_EXT_framebuffer_object}
};

static int g_extensionMapSize = 11;

static sfogl_StrToExtMap *FindExtEntry(const char *extensionName)
{
    int loop;
    sfogl_StrToExtMap *currLoc = ExtensionMap;
    for(loop = 0; loop < g_extensionMapSize; ++loop, ++currLoc)
    {
        if(strcmp(extensionName, currLoc->extensionName) == 0)
            return currLoc;
    }

    return NULL;
}

static void ClearExtensionVars()
{
    sfogl_ext_EXT_blend_minmax = sfogl_LOAD_FAILED;
    sfogl_ext_EXT_blend_subtract = sfogl_LOAD_FAILED;
    sfogl_ext_ARB_multitexture = sfogl_LOAD_FAILED;
    sfogl_ext_EXT_blend_func_separate = sfogl_LOAD_FAILED;
    sfogl_ext_ARB_shading_language_100 = sfogl_LOAD_FAILED;
    sfogl_ext_ARB_shader_objects = sfogl_LOAD_FAILED;
    sfogl_ext_ARB_vertex_shader = sfogl_LOAD_FAILED;
    sfogl_ext_ARB_fragment_shader = sfogl_LOAD_FAILED;
    sfogl_ext_ARB_texture_non_power_of_two = sfogl_LOAD_FAILED;
    sfogl_ext_EXT_blend_equation_separate = sfogl_LOAD_FAILED;
    sfogl_ext_EXT_framebuffer_object = sfogl_LOAD_FAILED;
}


static void LoadExtByName(const char *extensionName)
{
    sfogl_StrToExtMap *entry = NULL;
    entry = FindExtEntry(extensionName);
    if(entry)
    {
        if(entry->LoadExtension)
        {
            int numFailed = entry->LoadExtension();
            if(numFailed == 0)
            {
                *(entry->extensionVariable) = sfogl_LOAD_SUCCEEDED;
            }
            else
            {
                *(entry->extensionVariable) = sfogl_LOAD_SUCCEEDED + numFailed;
            }
        }
        else
        {
            *(entry->extensionVariable) = sfogl_LOAD_SUCCEEDED;
        }
    }
}


static void ProcExtsFromExtString(const char *strExtList)
{
    size_t iExtListLen = strlen(strExtList);
    const char *strExtListEnd = strExtList + iExtListLen;
    const char *strCurrPos = strExtList;
    char strWorkBuff[256];

    while(*strCurrPos)
    {
        /*Get the extension at our position.*/
        int iStrLen = 0;
        const char *strEndStr = strchr(strCurrPos, ' ');
        int iStop = 0;
        if(strEndStr == NULL)
        {
            strEndStr = strExtListEnd;
            iStop = 1;
        }

        iStrLen = (int)((ptrdiff_t)strEndStr - (ptrdiff_t)strCurrPos);

        if(iStrLen > 255)
            return;

        strncpy(strWorkBuff, strCurrPos, iStrLen);
        strWorkBuff[iStrLen] = '\0';

        LoadExtByName(strWorkBuff);

        strCurrPos = strEndStr + 1;
        if(iStop) break;
    }
}

int sfogl_LoadFunctions()
{
    int numFailed = 0;
    ClearExtensionVars();

    sf_ptrc_glGetString = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetString");
    if(!sf_ptrc_glGetString) return sfogl_LOAD_FAILED;

    ProcExtsFromExtString((const char *)sf_ptrc_glGetString(GL_EXTENSIONS));
    numFailed = Load_Version_1_2();

    if(numFailed == 0)
        return sfogl_LOAD_SUCCEEDED;
    else
        return sfogl_LOAD_SUCCEEDED + numFailed;
}

static int g_major_version = 0;
static int g_minor_version = 0;

static void ParseVersionFromString(int *pOutMajor, int *pOutMinor, const char *strVersion)
{
    const char *strDotPos = NULL;
    int iLength = 0;
    char strWorkBuff[10];
    *pOutMinor = 0;
    *pOutMajor = 0;

    strDotPos = strchr(strVersion, '.');
    if(!strDotPos)
        return;

    iLength = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
    strncpy(strWorkBuff, strVersion, iLength);
    strWorkBuff[iLength] = '\0';

    *pOutMajor = atoi(strWorkBuff);
    strDotPos = strchr(strVersion + iLength + 1, ' ');
    if(!strDotPos)
    {
        /*No extra data. Take the whole rest of the string.*/
        strcpy(strWorkBuff, strVersion + iLength + 1);
    }
    else
    {
        /*Copy only up until the space.*/
        int iLengthMinor = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
        iLengthMinor = iLengthMinor - (iLength + 1);
        strncpy(strWorkBuff, strVersion + iLength + 1, iLengthMinor);
        strWorkBuff[iLengthMinor] = '\0';
    }

    *pOutMinor = atoi(strWorkBuff);
}

static void GetGLVersion()
{
    ParseVersionFromString(&g_major_version, &g_minor_version, (const char*)glGetString(GL_VERSION));
}

int sfogl_GetMajorVersion()
{
    if(g_major_version == 0)
        GetGLVersion();
    return g_major_version;
}

int sfogl_GetMinorVersion()
{
    if(g_major_version == 0) //Yes, check the major version to get the minor one.
        GetGLVersion();
    return g_minor_version;
}

int sfogl_IsVersionGEQ(int majorVersion, int minorVersion)
{
    if(g_major_version == 0)
        GetGLVersion();

    if(majorVersion > g_major_version) return 0;
    if(majorVersion < g_major_version) return 1;
    if(g_minor_version >= minorVersion) return 1;
    return 0;
}

