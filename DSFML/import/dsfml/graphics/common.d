/*
*	DSFML - SFML Library wrapper for the D programming language.
*	Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*	Copyright (C) 2010 Andreas Hollandt
*
*	This software is provided 'as-is', without any express or
*	implied warranty. In no event will the authors be held
*	liable for any damages arising from the use of this software.
*
*	Permission is granted to anyone to use this software for any purpose,
*	including commercial applications, and to alter it and redistribute
*	it freely, subject to the following restrictions:
*
*	1.  The origin of this software must not be misrepresented;
*		you must not claim that you wrote the original software.
*		If you use this software in a product, an acknowledgment
*		in the product documentation would be appreciated but
*		is not required.
*
*	2.  Altered source versions must be plainly marked as such,
*		and must not be misrepresented as being the original software.
*
*	3.  This notice may not be removed or altered from any
*		source distribution.
*/

module dsfml.graphics.common;

private import	dsfml.system.common,
				dsfml.window.window,
				dsfml.window.windowhandle,
				dsfml.graphics.color,
				dsfml.graphics.rect,
				dsfml.graphics.font,
				dsfml.window.videomode;

package extern (C)
{
	// sfFont
	void*	function()					sfFont_Create;
	void*	function(cchar*)			sfFont_CreateFromFile;
	void*	function(ubyte*, size_t)	sfFont_CreateFromMemory;
	void	function(void*)				sfFont_Destroy;
	void*	function()					sfFont_GetDefaultFont;

	Glyph	function(void*, uint, uint, bool)	sfFont_GetGlyph;

	// sfRenderWindow
	void*	function(VideoMode, cchar*, uint, ContextSettings*)	sfRenderWindow_Create;
	void*	function(WindowHandle, ContextSettings*)			sfRenderWindow_CreateFromHandle;
	void	function(void*)										sfRenderWindow_Destroy;
	void*	function(void*)										sfRenderWindow_GetInput;
//	bool	function(void*)										sfRenderWindow_IsOpened;

/*
	void	function(void*, void*)								sfRenderWindow_DrawSprite;
	void	function(void*, void*)								sfRenderWindow_DrawShape;
	void	function(void*, void*)								sfRenderWindow_DrawText;

	void	function(void*, void*, void*)						sfRenderWindow_DrawSpriteWithShader;
	void	function(void*, void*, void*)						sfRenderWindow_DrawShapeWithShader;
	void	function(void*, void*, void*)						sfRenderWindow_DrawTextWithShader;
*/
	
	void*	function(void*)										sfRenderWindow_Capture;
	void	function(void*, Color)								sfRenderWindow_Clear;
	void	function(void*, void*)								sfRenderWindow_SetView;
	void*	function(void*)										sfRenderWindow_GetView;
	void*	function (void*)									sfRenderWindow_GetDefaultView;
	void	function(void*, uint, uint, float*, float*, void*)	sfRenderWindow_ConvertCoords;
	
	// DSFML2
	
	void	function(void*)										sfRenderWindow_Flush;
 
	// sfShader
	void*	function(cchar*)									sfShader_CreateFromFile;
	void*	function(cchar*)									sfShader_CreateFromMemory;
	void	function(void*)										sfShader_Destroy;
	void	function(void*, cchar*, float)						sfShader_SetParameter1;
	void	function(void*, cchar*, float, float)				sfShader_SetParameter2;
	void	function(void*, cchar*, float, float, float)		sfShader_SetParameter3;
	void	function(void*, cchar*, float, float, float, float)	sfShader_SetParameter4;
	void	function(void*, cchar*, void*)						sfShader_SetTexture;
	int		function()											sfShader_IsAvailable;
	void	function(void*)										sfShader_Bind;
	void	function(void*)										sfShader_Unbind;

	// sfView
	void*		function()						sfView_Create;
	void*		function(FloatRect)				sfView_CreateFromRect;
	void		function(void*)					sfView_Destroy;
	void		function(void*, float, float)	sfView_SetCenter;
	void		function(void*, float, float)	sfView_SetSize;
	void		function(void*, FloatRect)		sfView_SetViewport;
	float		function(void*)					sfView_GetCenterX;
	float		function(void*)					sfView_GetCenterY;
	float		function(void*)					sfView_GetWidth;
	float		function(void*)					sfView_GetHeight;
	FloatRect	function(void*)					sfView_GetViewport;
	void		function(void*, float, float)	sfView_Move;
	void		function(void*, float)			sfView_Zoom;
	
	// DSFML2
	void		function(void*, float)			sfView_SetRotation;
	float		function(void*)					sfView_GetRotation;
	void		function(void*, float)			sfView_Rotate;
	void		function(void*, FloatRect)		sfView_Reset;
}

static this()
{
debug
	DllLoader dll = DllLoader.load("csfml-graphics-d");
else
	DllLoader dll = DllLoader.load("csfml-graphics");

	// sfFont
	mixin(loadFromSharedLib("sfFont_CreateFromFile"));
	mixin(loadFromSharedLib("sfFont_CreateFromMemory"));
	mixin(loadFromSharedLib("sfFont_Destroy"));
	mixin(loadFromSharedLib("sfFont_GetDefaultFont"));
	mixin(loadFromSharedLib("sfFont_GetGlyph"));

	// sfRenderWindow
	mixin(loadFromSharedLib("sfRenderWindow_Create"));
	mixin(loadFromSharedLib("sfRenderWindow_CreateFromHandle"));
	mixin(loadFromSharedLib("sfRenderWindow_Destroy"));
	mixin(loadFromSharedLib("sfRenderWindow_GetInput"));

/*
	mixin(loadFromSharedLib("sfRenderWindow_DrawSprite"));
	mixin(loadFromSharedLib("sfRenderWindow_DrawShape"));
	mixin(loadFromSharedLib("sfRenderWindow_DrawText"));

	mixin(loadFromSharedLib("sfRenderWindow_DrawSpriteWithShader"));
	mixin(loadFromSharedLib("sfRenderWindow_DrawShapeWithShader"));
	mixin(loadFromSharedLib("sfRenderWindow_DrawTextWithShader"));
*/
	
	mixin(loadFromSharedLib("sfRenderWindow_Clear"));
	mixin(loadFromSharedLib("sfRenderWindow_SetView"));
	mixin(loadFromSharedLib("sfRenderWindow_GetView"));
	mixin(loadFromSharedLib("sfRenderWindow_GetDefaultView"));
	mixin(loadFromSharedLib("sfRenderWindow_ConvertCoords"));
	
	// DSFML2
	mixin(loadFromSharedLib("sfRenderWindow_Flush"));

	// sfShader
	mixin(loadFromSharedLib("sfShader_CreateFromFile"));
	mixin(loadFromSharedLib("sfShader_CreateFromMemory"));
	mixin(loadFromSharedLib("sfShader_Destroy"));
	mixin(loadFromSharedLib("sfShader_SetParameter1"));
	mixin(loadFromSharedLib("sfShader_SetParameter2"));
	mixin(loadFromSharedLib("sfShader_SetParameter3"));
	mixin(loadFromSharedLib("sfShader_SetParameter4"));
	mixin(loadFromSharedLib("sfShader_SetTexture"));
	mixin(loadFromSharedLib("sfShader_IsAvailable"));
	mixin(loadFromSharedLib("sfShader_Bind"));
	mixin(loadFromSharedLib("sfShader_Unbind"));
	
	// sfView
	mixin(loadFromSharedLib("sfView_Create"));
	mixin(loadFromSharedLib("sfView_CreateFromRect"));
	mixin(loadFromSharedLib("sfView_Destroy"));
	mixin(loadFromSharedLib("sfView_SetCenter"));
	mixin(loadFromSharedLib("sfView_SetSize"));
	mixin(loadFromSharedLib("sfView_SetViewport"));
	mixin(loadFromSharedLib("sfView_GetCenterX"));
	mixin(loadFromSharedLib("sfView_GetCenterY"));
	mixin(loadFromSharedLib("sfView_GetWidth"));
	mixin(loadFromSharedLib("sfView_GetHeight"));
	mixin(loadFromSharedLib("sfView_GetViewport"));
	mixin(loadFromSharedLib("sfView_Move"));
	mixin(loadFromSharedLib("sfView_Zoom"));
	
	// DSFML2
	mixin(loadFromSharedLib("sfView_SetRotation"));
	mixin(loadFromSharedLib("sfView_GetRotation"));
	mixin(loadFromSharedLib("sfView_Rotate"));
	mixin(loadFromSharedLib("sfView_Reset"));
}