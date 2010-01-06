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

module dsfml.system.randomizer;

import dsfml.system.common;

/**
*	Randomizer is an utility class for generating pseudo-random
*	numbers
*	
*	Examples:
*	-----------------------------------------------------------
*	int randI = Randomizer.Random(1, 100);
*	float randF = Randomizer.Random(1.0, 10.0)
*	-----------------------------------------------------------
*/
class Randomizer
{
	/**
	*	Set the seed for the generator. Using a known seed
	*	allows you to reproduce the same sequence of random number
	*
	*	Params:
	*		seed = Number to use as the seed
	*
	*/
	static void setSeed(uint seed)
	{
		sfRandom_SetSeed(seed);
	}

	/**
	*	Get the seed used to generate random numbers the generator.
	*
	*	Returns:
	*		Current seed
	*/
	static uint getSeed()
	{
		return sfRandom_GetSeed();
	}
	
	/**
	*	Get a random float number in a given range
	*
	*	Params:
	*		begin = Start of the range
	*		end = End of the range
	*	Returns:
	*		Random number in [Begin, End]								
	*/
	static float random(float begin, float end)
	{
		return sfRandom_Float(begin, end);
	}
	
	/**
	*	Get a random integral number in a given range
	*
	*	Params:
	*		begin = Start of the range
	*		end = End of the range
	*	Returns:
	*		Random number in [Begin, End]								
	*/
	static int random(int begin, int end)
	{
		return sfRandom_Int(begin, end);
	}


private:

	/*
	*	Prevent instanciation
	*/
	this()
	{
		
	}

// External ====================================================================

	extern (C)
	{
		typedef void function(uint) pf_sfRandom_SetSeed;
		typedef uint function() pf_sfRandom_GetSeed;
		typedef float function(float, float) pf_sfRandom_Float;
		typedef int function(int, int) pf_sfRandom_Int;
		
		static pf_sfRandom_SetSeed sfRandom_SetSeed;
		static pf_sfRandom_GetSeed sfRandom_GetSeed;
		static pf_sfRandom_Float sfRandom_Float;
		static pf_sfRandom_Int sfRandom_Int;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-system-d");
	else
		DllLoader dll = DllLoader.load("csfml-system");
		
		sfRandom_SetSeed = cast(pf_sfRandom_SetSeed)dll.getSymbol("sfRandom_SetSeed");
		sfRandom_GetSeed = cast(pf_sfRandom_GetSeed)dll.getSymbol("sfRandom_GetSeed");
		sfRandom_Float = cast(pf_sfRandom_Float)dll.getSymbol("sfRandom_Float");
		sfRandom_Int = cast(pf_sfRandom_Int)dll.getSymbol("sfRandom_Int");
	}
}

