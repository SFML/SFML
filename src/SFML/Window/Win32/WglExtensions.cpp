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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Win32/WglExtensions.hpp>
#include <SFML/Window/Context.hpp>
#include <cstdlib>
#include <cstring>
#include <cstddef>

static sf::GlFunctionPointer IntGetProcAddress(const char* name)
{
    return sf::Context::getFunction(name);
}

int sfwgl_ext_EXT_swap_control = sfwgl_LOAD_FAILED;
int sfwgl_ext_ARB_multisample = sfwgl_LOAD_FAILED;
int sfwgl_ext_ARB_pixel_format = sfwgl_LOAD_FAILED;
int sfwgl_ext_ARB_create_context = sfwgl_LOAD_FAILED;
int sfwgl_ext_ARB_create_context_profile = sfwgl_LOAD_FAILED;

int (CODEGEN_FUNCPTR *sf_ptrc_wglGetSwapIntervalEXT)(void) = NULL;
BOOL (CODEGEN_FUNCPTR *sf_ptrc_wglSwapIntervalEXT)(int) = NULL;

static int Load_EXT_swap_control(void)
{
    int numFailed = 0;
    sf_ptrc_wglGetSwapIntervalEXT = (int (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("wglGetSwapIntervalEXT");
    if(!sf_ptrc_wglGetSwapIntervalEXT) numFailed++;
    sf_ptrc_wglSwapIntervalEXT = (BOOL (CODEGEN_FUNCPTR *)(int))IntGetProcAddress("wglSwapIntervalEXT");
    if(!sf_ptrc_wglSwapIntervalEXT) numFailed++;
    return numFailed;
}

BOOL (CODEGEN_FUNCPTR *sf_ptrc_wglChoosePixelFormatARB)(HDC, const int *, const FLOAT *, UINT, int *, UINT *) = NULL;
BOOL (CODEGEN_FUNCPTR *sf_ptrc_wglGetPixelFormatAttribfvARB)(HDC, int, int, UINT, const int *, FLOAT *) = NULL;
BOOL (CODEGEN_FUNCPTR *sf_ptrc_wglGetPixelFormatAttribivARB)(HDC, int, int, UINT, const int *, int *) = NULL;

static int Load_ARB_pixel_format(void)
{
    int numFailed = 0;
    sf_ptrc_wglChoosePixelFormatARB = (BOOL (CODEGEN_FUNCPTR *)(HDC, const int *, const FLOAT *, UINT, int *, UINT *))IntGetProcAddress("wglChoosePixelFormatARB");
    if(!sf_ptrc_wglChoosePixelFormatARB) numFailed++;
    sf_ptrc_wglGetPixelFormatAttribfvARB = (BOOL (CODEGEN_FUNCPTR *)(HDC, int, int, UINT, const int *, FLOAT *))IntGetProcAddress("wglGetPixelFormatAttribfvARB");
    if(!sf_ptrc_wglGetPixelFormatAttribfvARB) numFailed++;
    sf_ptrc_wglGetPixelFormatAttribivARB = (BOOL (CODEGEN_FUNCPTR *)(HDC, int, int, UINT, const int *, int *))IntGetProcAddress("wglGetPixelFormatAttribivARB");
    if(!sf_ptrc_wglGetPixelFormatAttribivARB) numFailed++;
    return numFailed;
}

HGLRC (CODEGEN_FUNCPTR *sf_ptrc_wglCreateContextAttribsARB)(HDC, HGLRC, const int *) = NULL;

static int Load_ARB_create_context(void)
{
    int numFailed = 0;
    sf_ptrc_wglCreateContextAttribsARB = (HGLRC (CODEGEN_FUNCPTR *)(HDC, HGLRC, const int *))IntGetProcAddress("wglCreateContextAttribsARB");
    if(!sf_ptrc_wglCreateContextAttribsARB) numFailed++;
    return numFailed;
}


static const char * (CODEGEN_FUNCPTR *sf_ptrc_wglGetExtensionsStringARB)(HDC) = NULL;

typedef int (*PFN_LOADFUNCPOINTERS)(void);
typedef struct sfwgl_StrToExtMap_s
{
    const char *extensionName;
    int *extensionVariable;
    PFN_LOADFUNCPOINTERS LoadExtension;
} sfwgl_StrToExtMap;

static sfwgl_StrToExtMap ExtensionMap[5] = {
    {"WGL_EXT_swap_control", &sfwgl_ext_EXT_swap_control, Load_EXT_swap_control},
    {"WGL_ARB_multisample", &sfwgl_ext_ARB_multisample, NULL},
    {"WGL_ARB_pixel_format", &sfwgl_ext_ARB_pixel_format, Load_ARB_pixel_format},
    {"WGL_ARB_create_context", &sfwgl_ext_ARB_create_context, Load_ARB_create_context},
    {"WGL_ARB_create_context_profile", &sfwgl_ext_ARB_create_context_profile, NULL},
};

static int g_extensionMapSize = 5;

static sfwgl_StrToExtMap *FindExtEntry(const char *extensionName)
{
    int loop;
    sfwgl_StrToExtMap *currLoc = ExtensionMap;
    for(loop = 0; loop < g_extensionMapSize; ++loop, ++currLoc)
    {
        if(strcmp(extensionName, currLoc->extensionName) == 0)
            return currLoc;
    }

    return NULL;
}

static void ClearExtensionVars(void)
{
    sfwgl_ext_EXT_swap_control = sfwgl_LOAD_FAILED;
    sfwgl_ext_ARB_multisample = sfwgl_LOAD_FAILED;
    sfwgl_ext_ARB_pixel_format = sfwgl_LOAD_FAILED;
    sfwgl_ext_ARB_create_context = sfwgl_LOAD_FAILED;
    sfwgl_ext_ARB_create_context_profile = sfwgl_LOAD_FAILED;
}


static void LoadExtByName(const char *extensionName)
{
    sfwgl_StrToExtMap *entry = NULL;
    entry = FindExtEntry(extensionName);
    if(entry)
    {
        if(entry->LoadExtension)
        {
            int numFailed = entry->LoadExtension();
            if(numFailed == 0)
            {
                *(entry->extensionVariable) = sfwgl_LOAD_SUCCEEDED;
            }
            else
            {
                *(entry->extensionVariable) = sfwgl_LOAD_SUCCEEDED + numFailed;
            }
        }
        else
        {
            *(entry->extensionVariable) = sfwgl_LOAD_SUCCEEDED;
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

int sfwgl_LoadFunctions(HDC hdc)
{
    ClearExtensionVars();

    sf_ptrc_wglGetExtensionsStringARB = (const char * (CODEGEN_FUNCPTR *)(HDC))IntGetProcAddress("wglGetExtensionsStringARB");
    if(!sf_ptrc_wglGetExtensionsStringARB) return sfwgl_LOAD_FAILED;

    ProcExtsFromExtString((const char *)sf_ptrc_wglGetExtensionsStringARB(hdc));
    return sfwgl_LOAD_SUCCEEDED;
}

