/*  

SDL_rotozoom.c: rotozoomer, zoomer and shrinker for 32bit or 8bit surfaces

Copyright (C) 2001-2012  Andreas Schiffler

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net

*/

#ifndef _SDL_srotozoom_h
#define _SDL_srotozoom_h

#include <math.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI	3.141592654
#endif

#include "SDL.h"

	/* ---- Defines */

	/*!
	\brief Disable anti-aliasing (no smoothing).
	*/
#define SMOOTHING_OFF		0

	/*!
	\brief Enable anti-aliasing (smoothing).
	*/
#define SMOOTHING_ON		1

	/* ---- Function Prototypes */

#ifdef _MSC_VER
#  if defined(DLL_EXPORT) && !defined(LIBSDL_SGFX_DLL_IMPORT)
#    define SDL_SROTOZOOM_SCOPE __declspec(dllexport)
#  else
#    ifdef LIBSDL_SGFX_DLL_IMPORT
#      define SDL_SROTOZOOM_SCOPE __declspec(dllimport)
#    endif
#  endif
#endif
#ifndef SDL_SROTOZOOM_SCOPE
#  define SDL_SROTOZOOM_SCOPE extern
#endif

	/* 

	Rotozoom functions

	*/

	SDL_SROTOZOOM_SCOPE SDL_Surface *srotozoomSurface(SDL_Surface * src, double angle, double zoom, int smooth);

	SDL_SROTOZOOM_SCOPE SDL_Surface *srotozoomSurfaceXY
		(SDL_Surface * src, double angle, double zoomx, double zoomy, int smooth);


	SDL_SROTOZOOM_SCOPE void srotozoomSurfaceSize(int width, int height, double angle, double zoom, int *dstwidth,
		int *dstheight);

	SDL_SROTOZOOM_SCOPE void srotozoomSurfaceSizeXY
		(int width, int height, double angle, double zoomx, double zoomy, 
		int *dstwidth, int *dstheight);

	/* 

	Zooming functions

	*/

	SDL_SROTOZOOM_SCOPE SDL_Surface *szoomSurface(SDL_Surface * src, double zoomx, double zoomy, int smooth);

	SDL_SROTOZOOM_SCOPE void szoomSurfaceSize(int width, int height, double zoomx, double zoomy, int *dstwidth, int *dstheight);

	/* 

	Shrinking functions

	*/     

	SDL_SROTOZOOM_SCOPE SDL_Surface *sshrinkSurface(SDL_Surface * src, int factorx, int factory);

	/* 

	Specialized rotation functions

	*/

	SDL_SROTOZOOM_SCOPE SDL_Surface* srotateSurface90Degrees(SDL_Surface* src, int numClockwiseTurns);

	/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif				/* _SDL_rotozoom_h */
