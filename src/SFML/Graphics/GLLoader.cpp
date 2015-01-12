
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/GLLoader.hpp>
#include <SFML/Window/Context.hpp>
#include <cstdlib>
#include <cstring>
#include <cstddef>

static void* IntGetProcAddress(const char* name)
{
    return sf::Context::getFunction(name);
}

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

static int Load_ARB_multitexture()
{
    int numFailed = 0;
    sf_ptrc_glActiveTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glActiveTextureARB");
    if(!sf_ptrc_glActiveTextureARB) numFailed++;
    sf_ptrc_glClientActiveTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glClientActiveTextureARB");
    if(!sf_ptrc_glClientActiveTextureARB) numFailed++;
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

void (CODEGEN_FUNCPTR *sf_ptrc_glBlendFunc)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClear)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearDepth)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClearStencil)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glClipPlane)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glColorMask)(GLboolean, GLboolean, GLboolean, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCopyPixels)(GLint, GLint, GLsizei, GLsizei, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glCullFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDepthFunc)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDepthMask)(GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDepthRange)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDisable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glDrawBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glEnable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFinish)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFlush)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFrontFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glFrustum)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetBooleanv)(GLenum, GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetClipPlane)(GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetDoublev)(GLenum, GLdouble *) = NULL;
GLenum (CODEGEN_FUNCPTR *sf_ptrc_glGetError)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetFloatv)(GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetIntegerv)(GLenum, GLint *) = NULL;
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
GLboolean (CODEGEN_FUNCPTR *sf_ptrc_glIsEnabled)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLineWidth)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadIdentity)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadMatrixd)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glLoadMatrixf)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMatrixMode)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultMatrixd)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glMultMatrixf)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glOrtho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPointSize)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPopAttrib)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPopMatrix)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPushAttrib)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glPushMatrix)() = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glReadBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glScissor)(GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glSelectBuffer)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glShadeModel)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glStencilFunc)(GLenum, GLint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glStencilMask)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glStencilOp)(GLenum, GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexImage1D)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameterf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameterfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameteri)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTexParameteriv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTranslated)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glTranslatef)(GLfloat, GLfloat, GLfloat) = NULL;
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
void (CODEGEN_FUNCPTR *sf_ptrc_glEnableClientState)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGenTextures)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sf_ptrc_glGetPointerv)(GLenum, GLvoid **) = NULL;
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
    sf_ptrc_glBlendFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendFunc");
    if(!sf_ptrc_glBlendFunc) numFailed++;
    sf_ptrc_glClear = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glClear");
    if(!sf_ptrc_glClear) numFailed++;
    sf_ptrc_glClearColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glClearColor");
    if(!sf_ptrc_glClearColor) numFailed++;
    sf_ptrc_glClearDepth = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glClearDepth");
    if(!sf_ptrc_glClearDepth) numFailed++;
    sf_ptrc_glClearStencil = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glClearStencil");
    if(!sf_ptrc_glClearStencil) numFailed++;
    sf_ptrc_glClipPlane = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glClipPlane");
    if(!sf_ptrc_glClipPlane) numFailed++;
    sf_ptrc_glColorMask = (void (CODEGEN_FUNCPTR *)(GLboolean, GLboolean, GLboolean, GLboolean))IntGetProcAddress("glColorMask");
    if(!sf_ptrc_glColorMask) numFailed++;
    sf_ptrc_glCopyPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum))IntGetProcAddress("glCopyPixels");
    if(!sf_ptrc_glCopyPixels) numFailed++;
    sf_ptrc_glCullFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCullFace");
    if(!sf_ptrc_glCullFace) numFailed++;
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
    sf_ptrc_glEnable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnable");
    if(!sf_ptrc_glEnable) numFailed++;
    sf_ptrc_glFinish = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glFinish");
    if(!sf_ptrc_glFinish) numFailed++;
    sf_ptrc_glFlush = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glFlush");
    if(!sf_ptrc_glFlush) numFailed++;
    sf_ptrc_glFrontFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glFrontFace");
    if(!sf_ptrc_glFrontFace) numFailed++;
    sf_ptrc_glFrustum = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glFrustum");
    if(!sf_ptrc_glFrustum) numFailed++;
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
    sf_ptrc_glIsEnabled = (GLboolean (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glIsEnabled");
    if(!sf_ptrc_glIsEnabled) numFailed++;
    sf_ptrc_glLineWidth = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glLineWidth");
    if(!sf_ptrc_glLineWidth) numFailed++;
    sf_ptrc_glLoadIdentity = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glLoadIdentity");
    if(!sf_ptrc_glLoadIdentity) numFailed++;
    sf_ptrc_glLoadMatrixd = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glLoadMatrixd");
    if(!sf_ptrc_glLoadMatrixd) numFailed++;
    sf_ptrc_glLoadMatrixf = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glLoadMatrixf");
    if(!sf_ptrc_glLoadMatrixf) numFailed++;
    sf_ptrc_glMatrixMode = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glMatrixMode");
    if(!sf_ptrc_glMatrixMode) numFailed++;
    sf_ptrc_glMultMatrixd = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glMultMatrixd");
    if(!sf_ptrc_glMultMatrixd) numFailed++;
    sf_ptrc_glMultMatrixf = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glMultMatrixf");
    if(!sf_ptrc_glMultMatrixf) numFailed++;
    sf_ptrc_glOrtho = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glOrtho");
    if(!sf_ptrc_glOrtho) numFailed++;
    sf_ptrc_glPointSize = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glPointSize");
    if(!sf_ptrc_glPointSize) numFailed++;
    sf_ptrc_glPopAttrib = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPopAttrib");
    if(!sf_ptrc_glPopAttrib) numFailed++;
    sf_ptrc_glPopMatrix = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPopMatrix");
    if(!sf_ptrc_glPopMatrix) numFailed++;
    sf_ptrc_glPushAttrib = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glPushAttrib");
    if(!sf_ptrc_glPushAttrib) numFailed++;
    sf_ptrc_glPushMatrix = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glPushMatrix");
    if(!sf_ptrc_glPushMatrix) numFailed++;
    sf_ptrc_glReadBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glReadBuffer");
    if(!sf_ptrc_glReadBuffer) numFailed++;
    sf_ptrc_glReadPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *))IntGetProcAddress("glReadPixels");
    if(!sf_ptrc_glReadPixels) numFailed++;
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
    sf_ptrc_glEnableClientState = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnableClientState");
    if(!sf_ptrc_glEnableClientState) numFailed++;
    sf_ptrc_glGenTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenTextures");
    if(!sf_ptrc_glGenTextures) numFailed++;
    sf_ptrc_glGetPointerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLvoid **))IntGetProcAddress("glGetPointerv");
    if(!sf_ptrc_glGetPointerv) numFailed++;
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

