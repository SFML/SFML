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
#include <SFML/Window/Unix/GlxExtensions.hpp>
#include <SFML/Window/Context.hpp>
#include <cstdlib>
#include <cstring>
#include <cstddef>

static sf::GlFunctionPointer IntGetProcAddress(const char* name)
{
    return sf::Context::getFunction(name);
}

int sfglx_ext_EXT_swap_control = sfglx_LOAD_FAILED;
int sfglx_ext_MESA_swap_control = sfglx_LOAD_FAILED;
int sfglx_ext_SGI_swap_control = sfglx_LOAD_FAILED;
int sfglx_ext_ARB_multisample = sfglx_LOAD_FAILED;
int sfglx_ext_ARB_create_context = sfglx_LOAD_FAILED;
int sfglx_ext_ARB_create_context_profile = sfglx_LOAD_FAILED;

void (CODEGEN_FUNCPTR *sf_ptrc_glXSwapIntervalEXT)(Display *, GLXDrawable, int) = NULL;

static int Load_EXT_swap_control(void)
{
    int numFailed = 0;
    sf_ptrc_glXSwapIntervalEXT = (void (CODEGEN_FUNCPTR *)(Display *, GLXDrawable, int))IntGetProcAddress("glXSwapIntervalEXT");
    if(!sf_ptrc_glXSwapIntervalEXT) numFailed++;
    return numFailed;
}

int (CODEGEN_FUNCPTR *sf_ptrc_glXSwapIntervalMESA)(int) = NULL;

static int Load_MESA_swap_control(void)
{
    int numFailed = 0;
    sf_ptrc_glXSwapIntervalMESA = (int (CODEGEN_FUNCPTR *)(int))IntGetProcAddress("glXSwapIntervalMESA");
    if(!sf_ptrc_glXSwapIntervalMESA) numFailed++;
    return numFailed;
}

int (CODEGEN_FUNCPTR *sf_ptrc_glXSwapIntervalSGI)(int) = NULL;

static int Load_SGI_swap_control(void)
{
    int numFailed = 0;
    sf_ptrc_glXSwapIntervalSGI = (int (CODEGEN_FUNCPTR *)(int))IntGetProcAddress("glXSwapIntervalSGI");
    if(!sf_ptrc_glXSwapIntervalSGI) numFailed++;
    return numFailed;
}

GLXContext (CODEGEN_FUNCPTR *sf_ptrc_glXCreateContextAttribsARB)(Display *, GLXFBConfig, GLXContext, Bool, const int *) = NULL;

static int Load_ARB_create_context(void)
{
    int numFailed = 0;
    sf_ptrc_glXCreateContextAttribsARB = (GLXContext (CODEGEN_FUNCPTR *)(Display *, GLXFBConfig, GLXContext, Bool, const int *))IntGetProcAddress("glXCreateContextAttribsARB");
    if(!sf_ptrc_glXCreateContextAttribsARB) numFailed++;
    return numFailed;
}

typedef int (*PFN_LOADFUNCPOINTERS)(void);
typedef struct sfglx_StrToExtMap_s
{
    const char *extensionName;
    int *extensionVariable;
    PFN_LOADFUNCPOINTERS LoadExtension;
} sfglx_StrToExtMap;

static sfglx_StrToExtMap ExtensionMap[6] = {
    {"GLX_EXT_swap_control", &sfglx_ext_EXT_swap_control, Load_EXT_swap_control},
    {"GLX_MESA_swap_control", &sfglx_ext_MESA_swap_control, Load_MESA_swap_control},
    {"GLX_SGI_swap_control", &sfglx_ext_SGI_swap_control, Load_SGI_swap_control},
    {"GLX_ARB_multisample", &sfglx_ext_ARB_multisample, NULL},
    {"GLX_ARB_create_context", &sfglx_ext_ARB_create_context, Load_ARB_create_context},
    {"GLX_ARB_create_context_profile", &sfglx_ext_ARB_create_context_profile, NULL},
};

static int g_extensionMapSize = 6;

static sfglx_StrToExtMap *FindExtEntry(const char *extensionName)
{
    int loop;
    sfglx_StrToExtMap *currLoc = ExtensionMap;
    for(loop = 0; loop < g_extensionMapSize; ++loop, ++currLoc)
    {
        if(strcmp(extensionName, currLoc->extensionName) == 0)
            return currLoc;
    }

    return NULL;
}

static void ClearExtensionVars(void)
{
    sfglx_ext_EXT_swap_control = sfglx_LOAD_FAILED;
    sfglx_ext_MESA_swap_control = sfglx_LOAD_FAILED;
    sfglx_ext_SGI_swap_control = sfglx_LOAD_FAILED;
    sfglx_ext_ARB_multisample = sfglx_LOAD_FAILED;
    sfglx_ext_ARB_create_context = sfglx_LOAD_FAILED;
    sfglx_ext_ARB_create_context_profile = sfglx_LOAD_FAILED;
}


static void LoadExtByName(const char *extensionName)
{
    sfglx_StrToExtMap *entry = NULL;
    entry = FindExtEntry(extensionName);
    if(entry)
    {
        if(entry->LoadExtension)
        {
            int numFailed = entry->LoadExtension();
            if(numFailed == 0)
            {
                *(entry->extensionVariable) = sfglx_LOAD_SUCCEEDED;
            }
            else
            {
                *(entry->extensionVariable) = sfglx_LOAD_SUCCEEDED + numFailed;
            }
        }
        else
        {
            *(entry->extensionVariable) = sfglx_LOAD_SUCCEEDED;
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

int sfglx_LoadFunctions(Display *display, int screen)
{
    ClearExtensionVars();


    ProcExtsFromExtString((const char *)glXQueryExtensionsString(display, screen));
    return sfglx_LOAD_SUCCEEDED;
}

