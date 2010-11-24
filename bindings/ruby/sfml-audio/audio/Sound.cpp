/* rbSFML - Copyright (c) 2010 Henrik Valter Vogelius Hansson - groogy@groogy.se
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    If you use this software in a product, an acknowledgment
 *    in the product documentation would be appreciated but
 *    is not required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any
 *    source distribution.
 */
 
#include "Sound.hpp"
#include "Vector3.hpp"
#include "main.hpp"
#include <SFML/Audio/Sound.hpp>

VALUE globalSoundClass;

/* External classes */
extern VALUE globalVector3Class;
extern VALUE globalSoundSourceClass;

static VALUE Sound_New( int argc, VALUE *args, VALUE self )
{
	sf::Sound *object = new sf::Sound();
}

void Init_Sound( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Base class defining a sound's properties.
 *
 * SFML::SoundSource is not meant to be used directly, it only serves as a common base for all audio objects that can 
 * live in the audio environment.
 *
 * It defines several properties for the sound: pitch, volume, position, attenuation, etc. All of them can be changed 
 * at any time with no impact on performances.
 */
	globalSoundClass = rb_define_class_under( sfml, "Sound", globalSoundSourceClass );
	
	// Instance methods
	
	// Aliases
}
