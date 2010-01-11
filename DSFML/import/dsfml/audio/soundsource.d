/**
 *	
 */
module dsfml.audio.soundsource;

import dsfml.system.vector3;
import dsfml.system.common;

enum SoundStatus
{
	Stopped, /// Sound is not playing
	Paused,  /// Sound is paused
	Playing  /// Sound is playing
}

/// base class
package class SoundSource(alias symbol) : DSFMLObject
{
private:
protected :

	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// This constructor is meant ot be called by derived classes only.
	///
	////////////////////////////////////////////////////////////
	this()
	{
		super(sfSoundSource_Create());
	}
	
	this(void* ptr)
	{
		super(ptr, true);
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the current status of the sound (stopped, paused, playing)
	///
	/// \return Current status of the sound
	///
	////////////////////////////////////////////////////////////
	SoundStatus getStatus()
	{
		return sfSoundSource_GetStatus(m_ptr);
	}
	
public:
	////////////////////////////////////////////////////////////
	/// \brief Set the pitch of the sound
	///
	/// The pitch represents the perceived fundamental frequency
	/// of a sound; thus you can make a sound more acute or grave
	/// by changing its pitch. A side effect of changing the pitch
	/// is to modify the playing speed of the sound as well.
	/// The default value for the pitch is 1.
	///
	/// \param pitch New pitch to apply to the sound
	///
	/// \see GetPitch
	///
	////////////////////////////////////////////////////////////
	void setPitch(float pitch)
	{
		sfSoundSource_SetPitch(m_ptr, pitch);
	}

	////////////////////////////////////////////////////////////
	/// \brief Set the volume of the sound
	///
	/// The volume is a value between 0 (mute) and 100 (full volume).
	/// The default value for the volume is 100.
	///
	/// \param volume Volume of the sound
	///
	/// \see GetVolume
	///
	////////////////////////////////////////////////////////////
	void setVolume(float volume)
	in
	{
		assert(volume >= 0 && volume <= 100);
	}
	body
	{
		sfSoundSource_SetVolume(m_ptr, volume);
	}

	////////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the sound in the audio scene
	///
	/// Only sounds with one channel (mono sounds) can be
	/// spatialized.
	/// The default position of a sound is (0, 0, 0).
	///
	/// \param x X coordinate of the position of the sound in the scene
	/// \param y Y coordinate of the position of the sound in the scene
	/// \param z Z coordinate of the position of the sound in the scene
	///
	/// \see GetPosition
	///
	////////////////////////////////////////////////////////////
	void setPosition(float x, float y, float z)
	{
		sfSoundSource_SetPosition(m_ptr, x, y, z);
	}

	////////////////////////////////////////////////////////////
	/// \brief Set the 3D position of the sound in the audio scene
	///
	/// Only sounds with one channel (mono sounds) can be
	/// spatialized.
	/// The default position of a sound is (0, 0, 0).
	///
	/// \param position Position of the sound in the scene
	///
	/// \see GetPosition
	///
	////////////////////////////////////////////////////////////
	void setPosition(Vector3f position)
	{
		sfSoundSource_SetPosition(m_ptr, position.x, position.y, position.z);
	}

	////////////////////////////////////////////////////////////
	/// \brief Make the sound's position relative to the listener or absolute
	///
	/// Making a sound relative to the listener will ensure that it will always
	/// be played the same way regardless the position of the listener.
	/// This can be useful for non-spatialized sounds, sounds that are
	/// produced by the listener, or sounds attached to it.
	/// The default value is false (position is absolute).
	///
	/// \param relative True to set the position relative, false to set it absolute
	///
	/// \see IsRelativeToListener
	///
	////////////////////////////////////////////////////////////
	void setRelativeToListener(bool relative)
	{
		sfSoundSource_SetRelativeToListener(m_ptr, relative);
	}

	////////////////////////////////////////////////////////////
	/// \brief Set the minimum distance of the sound
	///
	/// The "minimum distance" of a sound is the maximum
	/// distance at which it is heard at its maximum volume. Further
	/// than the minimum distance, it will start to fade out according
	/// to its attenuation factor. A value of 0 ("inside the head
	/// of the listener") is an invalid value and is forbidden.
	/// The default value of the minimum distance is 1.
	///
	/// \param distance New minimum distance of the sound
	///
	/// \see GetMinDistance, SetAttenuation
	///
	////////////////////////////////////////////////////////////
	void setMinDistance(float distance)
	{
		sfSoundSource_SetMinDistance(m_ptr, distance);
	}

	////////////////////////////////////////////////////////////
	/// \brief Set the attenuation factor of the sound
	///
	/// The attenuation is a multiplicative factor which makes
	/// the sound more or less loud according to its distance
	/// from the listener. An attenuation of 0 will produce a
	/// non-attenuated sound, i.e. its volume will always be the same
	/// whether it is heard from near or from far. On the other hand,
	/// an attenuation value such as 100 will make the sound fade out
	/// very quickly as it gets further from the listener.
	/// The default value of the attenuation is 1.
	///
	/// \param attenuation New attenuation factor of the sound
	///
	/// \see GetAttenuation, SetMinDistance
	///
	////////////////////////////////////////////////////////////
	void setAttenuation(float attenuation)
	{
		sfSoundSource_SetAttenuation(m_ptr, attenuation);
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the pitch of the sound
	///
	/// \return Pitch of the sound
	///
	/// \see SetPitch
	///
	////////////////////////////////////////////////////////////
	float GetPitch()
	{
		return sfSoundSource_GetPitch(m_ptr);
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the volume of the sound
	///
	/// \return Volume of the sound, in the range [0, 100]
	///
	/// \see SetVolume
	///
	////////////////////////////////////////////////////////////
	float GetVolume()
	{
		return sfSoundSource_GetVolume(m_ptr);
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the 3D position of the sound in the audio scene
	///
	/// \return Position of the sound
	///
	/// \see SetPosition
	///
	////////////////////////////////////////////////////////////
	Vector3f getPosition()
	{
		Vector3f ret;
		sfSoundSource_GetPosition(m_ptr, &ret.x, &ret.y, &ret.z);
		return ret;
	}

	////////////////////////////////////////////////////////////
	/// \brief Tell whether the sound's position is relative to the
	///		listener or is absolute
	///
	/// \return True if the position is relative, false if it's absolute
	///
	/// \see SetRelativeToListener
	///
	////////////////////////////////////////////////////////////
	bool isRelativeToListener()
	{
		return sfSoundSource_IsRelativeToListener(m_ptr);
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the minimum distance of the sound
	///
	/// \return Minimum distance of the sound
	///
	/// \see SetMinDistance, GetAttenuation
	///
	////////////////////////////////////////////////////////////
	float getMinDistance()
	{
		return sfSoundSource_GetMinDistance(m_ptr);
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the attenuation factor of the sound
	///
	/// \return Attenuation factor of the sound
	///
	/// \see SetAttenuation, GetMinDistance
	///
	////////////////////////////////////////////////////////////
	float getAttenuation()
	{
		return sfSoundSource_GetAttenuation(m_ptr);
	}

	override void dispose()
	{
		sfSoundSource_Destroy(m_ptr);
	}

private:

	static extern(C)
	{
		void*		function()								sfSoundSource_Create;
		void		function(void*)							sfSoundSource_Destroy;

		SoundStatus	function(void*)							sfSoundSource_GetStatus;
		void		function(void*, float)					sfSoundSource_SetPitch;
		void		function(void*, float)					sfSoundSource_SetVolume;
		void		function(void*, float, float, float)	sfSoundSource_SetPosition;
		float		function(void*)							sfSoundSource_GetPitch;
		float		function(void*)							sfSoundSource_GetVolume;
		void		function(void*, float*, float*, float*)	sfSoundSource_GetPosition;
		float		function(void*)							sfSoundSource_GetMinDistance;
		float		function(void*)							sfSoundSource_GetAttenuation;
		void		function(void*, float)					sfSoundSource_SetMinDistance;
		void		function(void*, float)					sfSoundSource_SetAttenuation;
		
		void		function(void*, bool)					sfSoundSource_SetRelativeToListener;
		bool		function(void*)							sfSoundSource_IsRelativeToListener;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-audio-d");
	else
		DllLoader dll = DllLoader.load("csfml-audio");
		
		mixin(loadDerivedFromSharedLib("sfSoundSource", "Create", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "Destroy", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "GetStatus", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "GetPitch", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "SetPitch", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "GetVolume", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "SetVolume", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "GetPosition", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "SetPosition", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "GetMinDistance", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "SetMinDistance", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "GetAttenuation", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "SetAttenuation", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "SetRelativeToListener", symbol));
		mixin(loadDerivedFromSharedLib("sfSoundSource", "IsRelativeToListener", symbol));
	}
}