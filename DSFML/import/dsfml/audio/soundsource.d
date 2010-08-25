/*
 *	DSFML - SFML Library wrapper for the D programming language.
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

module dsfml.audio.soundsource;

import dsfml.system.vector;
import dsfml.system.common;


/// the sound's current status
enum SoundStatus
{
	Stopped, /// Sound is not playing
	Paused,  /// Sound is paused
	Playing  /// Sound is playing
}

/// base class
package class SoundSource(alias derivedClassName) : DSFMLObject
{
protected:

	/**
	 *	Default constructor
	 *
	 *	This constructor is meant ot be called by derived classes only.
	 *
	 */
	this()
	{
		super(sfSoundSource_Create());
	}
	
	this(SFMLClass ptr)
	{
		super(ptr);
	}

	override void dispose()
	{
		sfSoundSource_Destroy(m_ptr);
	}

public:

	/**
	 *	Set the 3D position of the sound in the audio scene
	 *
	 *	Only sounds with one channel (mono sounds) can be spatialized.
	 *	The default position of a sound is (0, 0, 0).
	 *
	 *	Params:
	 *		x = X coordinate of the position of the sound in the scene
	 *		y = Y coordinate of the position of the sound in the scene
	 *		z = Z coordinate of the position of the sound in the scene
	 */
	void setPosition(float x, float y, float z)
	{
		sfSoundSource_SetPosition(m_ptr, x, y, z);
	}
	
@property
{
	/**
	 *	Get the current status of the sound (stopped, paused, playing)
	 *
	 *	Returns:
	 *		current status of the sound
	 */
	SoundStatus status()
	{
		return sfSoundSource_GetStatus(m_ptr);
	}
	
	/**
	 *	Set the pitch of the sound
	 *
	 *	The pitch represents the perceived fundamental frequency
	 *	of a sound; thus you can make a sound more acute or grave
	 *	by changing its pitch. A side effect of changing the pitch
	 *	is to modify the playing speed of the sound as well.
	 *	The default value for the pitch is 1.
	 *
	 *	Params:
	 *		pitch = New pitch to apply to the sound
	 */
	void pitch(float pitch)
	{
		sfSoundSource_SetPitch(m_ptr, pitch);
	}

	/**
	 *	Get the pitch of the sound
	 *
	 *	Returns:
	 *		pitch of the sound
	 */
	float pitch()
	{
		return sfSoundSource_GetPitch(m_ptr);
	}
	
	/**
	 *	Set the volume of the sound
	 *
	 *	The volume is a value between 0 (mute) and 100 (full volume).
	 *	The default value for the volume is 100.
	 *
	 *	Params:
	 *		volume = volume of the sound
	 */
	void volume(float volume)
	in
	{
		assert(volume >= 0 && volume <= 100);
	}
	body
	{
		sfSoundSource_SetVolume(m_ptr, volume);
	}

	/**
	 *	Get the volume of the sound
	 *
	 *	Returns:
	 *		Volume of the sound, in the range [0, 100]
	 */
	float volume()
	{
		return sfSoundSource_GetVolume(m_ptr);
	}
	
	/**
	 *	Set the 3D position of the sound in the audio scene
	 *
	 *	Only sounds with one channel (mono sounds) can be
	 *	spatialized.
	 *	The default position of a sound is (0, 0, 0).
	 *
	 *	Params:
	 *		position = Position of the sound in the scene
	 */
	void position(Vector3f position)
	{
		sfSoundSource_SetPosition(m_ptr, position.x, position.y, position.z);
	}

	/**
	 *	Get the 3D position of the sound in the audio scene
	 *
	 *	Returns:
	 *	Position of the sound
	 */
	Vector3f position()
	{
		Vector3f ret;
		sfSoundSource_GetPosition(m_ptr, &ret.x, &ret.y, &ret.z);
		return ret;
	}
	
	/**
	 *	Make the sound's position relative to the listener or absolute
	 *
	 *	Making a sound relative to the listener will ensure that it will always
	 *	be played the same way regardless the position of the listener.
	 *	This can be useful for non-spatialized sounds, sounds that are
	 *	produced by the listener, or sounds attached to it.
	 *	The default value is false (position is absolute).
	 *
	 *	Params:
	 *		relative = True to set the position relative, false to set it absolute
	 */
	void relativeToListener(bool relative)
	{
		sfSoundSource_SetRelativeToListener(m_ptr, relative);
	}

	/**
	 *	Tell whether the sound's position is relative to the listener or is absolute
	 *
	 *	Returns:
	 *		True if the position is relative, false if it's absolute
	 */
	bool relativeToListener()
	{
		return sfSoundSource_IsRelativeToListener(m_ptr);
	}
	
	/**
	 *	Set the minimum distance of the sound
	 *
	 *	The "minimum distance" of a sound is the maximum
	 *	distance at which it is heard at its maximum volume. Further
	 *	than the minimum distance, it will start to fade out according
	 *	to its attenuation factor. A value of 0 ("inside the head
	 *	of the listener") is an invalid value and is forbidden.
	 *	The default value of the minimum distance is 1.
	 *
	 *	Params:
	 *		distance = New minimum distance of the sound
	 *
	 *	\see GetMinDistance, SetAttenuation
	 *
	 */
	void minDistance(float distance)
	{
		sfSoundSource_SetMinDistance(m_ptr, distance);
	}

	/**
	 *	Get the minimum distance of the sound
	 *
	 *	Returns:
	 *		Minimum distance of the sound
	 *
	 *	\see SetMinDistance, GetAttenuation
	 *
	 */
	float minDistance()
	{
		return sfSoundSource_GetMinDistance(m_ptr);
	}
	
	/**
	 *	Set the attenuation factor of the sound
	 *
	 *	The attenuation is a multiplicative factor which makes
	 *	the sound more or less loud according to its distance
	 *	from the listener. An attenuation of 0 will produce a
	 *	non-attenuated sound, i.e. its volume will always be the same
	 *	whether it is heard from near or from far. On the other hand,
	 *	an attenuation value such as 100 will make the sound fade out
	 *	very quickly as it gets further from the listener.
	 *	The default value of the attenuation is 1.
	 *
	 *	Params:
	 *		attenuation = New attenuation factor of the sound
	 *
	 *	\see GetAttenuation, SetMinDistance
	 *
	 */
	void attenuation(float attenuation)
	{
		sfSoundSource_SetAttenuation(m_ptr, attenuation);
	}

	/**
	 *	Get the attenuation factor of the sound
	 *
	 *	Returns:
	 *		Attenuation factor of the sound
	 *
	 *	\see SetAttenuation, GetMinDistance
	 *
	 */
	float attenuation()
	{
		return sfSoundSource_GetAttenuation(m_ptr);
	}
} // of @property


private:

	static extern(C)
	{
		SFMLClass		function()								sfSoundSource_Create;
		void		function(SFMLClass)							sfSoundSource_Destroy;

		SoundStatus	function(SFMLClass)							sfSoundSource_GetStatus;
		void		function(SFMLClass, float)					sfSoundSource_SetPitch;
		void		function(SFMLClass, float)					sfSoundSource_SetVolume;
		void		function(SFMLClass, float, float, float)	sfSoundSource_SetPosition;
		float		function(SFMLClass)							sfSoundSource_GetPitch;
		float		function(SFMLClass)							sfSoundSource_GetVolume;
		void		function(SFMLClass, float*, float*, float*)	sfSoundSource_GetPosition;
		float		function(SFMLClass)							sfSoundSource_GetMinDistance;
		float		function(SFMLClass)							sfSoundSource_GetAttenuation;
		void		function(SFMLClass, float)					sfSoundSource_SetMinDistance;
		void		function(SFMLClass, float)					sfSoundSource_SetAttenuation;
		
		void		function(SFMLClass, bool)					sfSoundSource_SetRelativeToListener;
		bool		function(SFMLClass)							sfSoundSource_IsRelativeToListener;
	}

	mixin(loadDerivedFromSharedLib("csfml-audio", "sfSoundSource", derivedClassName,
		"Create", "Destroy", "GetStatus", "GetPitch", "SetPitch", "GetVolume", "SetVolume", "GetPosition", "SetPosition",
		"GetMinDistance", "SetMinDistance", "GetAttenuation", "SetAttenuation", "SetRelativeToListener", "IsRelativeToListener"));
}