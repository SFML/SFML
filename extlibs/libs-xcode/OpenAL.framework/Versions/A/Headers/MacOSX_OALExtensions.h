/**********************************************************************************************************************************
*
*   OpenAL cross platform audio library
*   Copyright © 2006, Apple Computer, Inc. All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without modification, are permitted provided 
*   that the following conditions are met:
*
*   1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
*   2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
*       disclaimer in the documentation and/or other materials provided with the distribution. 
*   3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of its contributors may be used to endorse or promote 
*       products derived from this software without specific prior written permission. 
*
*   THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
*   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS 
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
*   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
*   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
*   USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
**********************************************************************************************************************************/

#ifndef __MAC_OSX_OAL_EXTENSIONS_H__
#define __MAC_OSX_OAL_EXTENSIONS_H__

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*	
	Convert Data When Loading.  
	Default false, currently applies only to monophonic sounds. Use with alEnable()/alDisable()
*/
	#define ALC_MAC_OSX_CONVERT_DATA_UPON_LOADING         	0xF001

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ALC_EXT_MAC_OSX
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef ALvoid (*alcMacOSXRenderingQualityProcPtr) (const ALint value);
typedef ALvoid (*alMacOSXRenderChannelCountProcPtr) (const ALint value);
typedef ALvoid (*alcMacOSXMixerMaxiumumBussesProcPtr) (const ALint value);
typedef ALvoid (*alcMacOSXMixerOutputRateProcPtr) (const ALdouble value);

typedef ALint (*alcMacOSXGetRenderingQualityProcPtr) ();
typedef ALint (*alMacOSXGetRenderChannelCountProcPtr) ();
typedef ALint (*alcMacOSXGetMixerMaxiumumBussesProcPtr) ();
typedef ALdouble (*alcMacOSXGetMixerOutputRateProcPtr) ();

/* Render Quality. Used with alcMacOSXRenderingQuality() */
	
	#define ALC_MAC_OSX_SPATIAL_RENDERING_QUALITY_HIGH      'rqhi'
	#define ALC_MAC_OSX_SPATIAL_RENDERING_QUALITY_LOW		'rdlo'

/*	
	Render Channels. Used with alMacOSXRenderChannelCount()
	Allows a user to force OpenAL to render to stereo, regardless of the audio hardware being used
*/
	#define ALC_MAC_OSX_RENDER_CHANNEL_COUNT_STEREO         'rcst'
	#define ALC_MAC_OSX_RENDER_CHANNEL_COUNT_MULTICHANNEL   'rcmc'

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	AL_EXT_STATIC_BUFFER
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef ALvoid	AL_APIENTRY	(*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ALC_EXT_ASA : Apple Spatial Audio Extension
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
	Used with the ASA API calls: alcASAGetSource(), alcASASetSource(), alcASAGetListener(), alcASASetListener()
*/

typedef ALenum  (*alcASAGetSourceProcPtr) (const ALuint property, ALuint source, ALvoid *data, ALuint* dataSize);
typedef ALenum  (*alcASASetSourceProcPtr) (const ALuint property, ALuint source, ALvoid *data, ALuint dataSize);
typedef ALenum  (*alcASAGetListenerProcPtr) (const ALuint property, ALvoid *data, ALuint* dataSize);
typedef ALenum  (*alcASASetListenerProcPtr) (const ALuint property, ALvoid *data, ALuint dataSize);

	/* listener properties */
	#define ALC_ASA_REVERB_ON							'rvon'	// type ALboolean
	#define ALC_ASA_REVERB_GLOBAL_LEVEL					'rvgl'	// type ALfloat	-40.0 db - 40.0 db

	#define ALC_ASA_REVERB_ROOM_TYPE					'rvrt'	// type ALint	
	
	/* reverb room type presets for the ALC_ASA_REVERB_ROOM_TYPE property */
	#define ALC_ASA_REVERB_ROOM_TYPE_SmallRoom			0
	#define ALC_ASA_REVERB_ROOM_TYPE_MediumRoom			1
	#define ALC_ASA_REVERB_ROOM_TYPE_LargeRoom			2
	#define ALC_ASA_REVERB_ROOM_TYPE_MediumHall			3
	#define ALC_ASA_REVERB_ROOM_TYPE_LargeHall			4
	#define ALC_ASA_REVERB_ROOM_TYPE_Plate				5
	#define ALC_ASA_REVERB_ROOM_TYPE_MediumChamber		6
	#define ALC_ASA_REVERB_ROOM_TYPE_LargeChamber		7
	#define ALC_ASA_REVERB_ROOM_TYPE_Cathedral			8
	#define ALC_ASA_REVERB_ROOM_TYPE_LargeRoom2			9
	#define ALC_ASA_REVERB_ROOM_TYPE_MediumHall2		10
	#define ALC_ASA_REVERB_ROOM_TYPE_MediumHall3		11
	#define ALC_ASA_REVERB_ROOM_TYPE_LargeHall2			12

	#define ALC_ASA_REVERB_PRESET						'rvps'	// type ALchar* - (set only) path to an au preset file

	#define ALC_ASA_REVERB_EQ_GAIN						'rveg'	// type ALfloat
	#define ALC_ASA_REVERB_EQ_BANDWITH					'rveb'	// type ALfloat
	#define ALC_ASA_REVERB_EQ_FREQ						'rvef'	// type ALfloat

	#define ALC_ASA_REVERB_QUALITY					    'rvqt'	// type ALint
	
	/* reverb quality settings for the ALC_ASA_REVERB_QUALITY property */
	#define ALC_ASA_REVERB_QUALITY_Max					0x7F
	#define ALC_ASA_REVERB_QUALITY_High					0x60
	#define ALC_ASA_REVERB_QUALITY_Medium				0x40
	#define ALC_ASA_REVERB_QUALITY_Low					0x20
	#define ALC_ASA_REVERB_QUALITY_Min					0
	
	/* source properties */
	#define ALC_ASA_REVERB_SEND_LEVEL					'rvsl'	// type ALfloat	0.0 (dry) - 1.0 (wet) (0-100% dry/wet mix, 0.0 default)
	#define ALC_ASA_OCCLUSION							'occl'	// type ALfloat	-100.0 db (most occlusion) - 0.0 db (no occlusion, 0.0 default)
	#define ALC_ASA_OBSTRUCTION							'obst'	// type ALfloat	-100.0 db (most obstruction) - 0.0 db (no obstruction, 0.0 default)	

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ALC_EXT_ASA_ROGER_BEEP : Apple Spatial Audio Extension for Roger Beep Effect
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	
/*	
	Roger Beep : an effect to simulate effects such as Walkie Talkie noise. It is designed to replace the 
	source audio data with a specific 'tone' when falling below a specified db threshold for a specified time.
	This Extension will be present when the Roger Beep Audio Unit is present on the system (10.5 or greater)
	Use the alcASAGetSource() and alcASASetSource() APIs in the ALC_EXT_ASA extension.
*/

	/* source properties */
	#define ALC_ASA_ROGER_BEEP_ENABLE				'rben'	// type ALboolean	- initializes Roger Beep for use - returns error if source is not in a Stopped or Initial state
	#define ALC_ASA_ROGER_BEEP_ON					'rbon'	// type ALboolean	- set effect on/off (bypass) - default setting is true (on)
	#define ALC_ASA_ROGER_BEEP_GAIN					'rbgn'	// type ALfloat		- 20.0 (db) apply maximum effect :  -80.0(db) apply minimum effect amount
	#define ALC_ASA_ROGER_BEEP_SENSITIVITY			'rbsn'	// type ALint		- specifiy a predefined sensitivity setting
	#define ALC_ASA_ROGER_BEEP_TYPE					'rbtp'	// type ALint		- choose predefined specific Roger Beep tone
	#define ALC_ASA_ROGER_BEEP_PRESET				'rbps'	// type ALchar*		- path to an au preset file (set only) 

	/* settings for the ALC_ASA_ROGER_BEEP_TYPE property */
	#define ALC_ASA_ROGER_BEEP_TYPE_quindartone			0
	#define ALC_ASA_ROGER_BEEP_TYPE_whitenoise			1
	#define ALC_ASA_ROGER_BEEP_TYPE_walkietalkie		2

	/* settings for the ALC_ASA_ROGER_BEEP_SENSITIVITY property */ 

	#define ALC_ASA_ROGER_BEEP_SENSITIVITY_Light		0
	#define ALC_ASA_ROGER_BEEP_SENSITIVITY_Medium		1
	#define ALC_ASA_ROGER_BEEP_SENSITIVITY_Heavy		2

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ALC_EXT_ASA_DISTORTION : Apple Spatial Audio Extension for Distortion Effect
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
	Distortion Effect 
	This Extension will be present when the Distortion Audio Unit is present on the system (10.5 or greater)
	Use the alcASAGetSource() and alcASASetSource() APIs in the ALC_EXT_ASA extension.
*/

	/* source properties */
	#define ALC_ASA_DISTORTION_ENABLE				'dsen'	// type ALboolean	- initializes Distortion for use - returns error if source is not in a Stopped or Initial state
	#define ALC_ASA_DISTORTION_ON					'dson'	// type ALboolean	- set effect on/off (bypass) - default setting is true (on)
	#define ALC_ASA_DISTORTION_MIX					'dsmx'	// type ALfloat		- mix balance between dry signal and distortion effect - 0.0 (no effect) - 100.0 (all effect)
	#define ALC_ASA_DISTORTION_TYPE					'dstp'	// type ALint		- choose predefined distortion settings
	#define ALC_ASA_DISTORTION_PRESET				'dsps'	// type ALchar*		- path to an au preset file (set only)

	/* settings for the ALC_ASA_DISTORTION_TYPE property */
	#define ALC_ASA_DISTORTION_TYPE_BitBrush			0
	#define ALC_ASA_DISTORTION_TYPE_BufferBeats			1
	#define ALC_ASA_DISTORTION_TYPE_LoFi				2
	#define ALC_ASA_DISTORTION_TYPE_BrokenSpeaker		3
	#define ALC_ASA_DISTORTION_TYPE_Cellphone			4
	#define ALC_ASA_DISTORTION_TYPE_Decimated1			5
	#define ALC_ASA_DISTORTION_TYPE_Decimated2			6
	#define ALC_ASA_DISTORTION_TYPE_Decimated3			7
	#define ALC_ASA_DISTORTION_TYPE_Decimated4			8
	#define ALC_ASA_DISTORTION_TYPE_DistortedFunk		9
	#define ALC_ASA_DISTORTION_TYPE_DistortionCubed		10
	#define ALC_ASA_DISTORTION_TYPE_DistortionSquared	11
	#define ALC_ASA_DISTORTION_TYPE_Echo1				12
	#define ALC_ASA_DISTORTION_TYPE_Echo2				13
	#define ALC_ASA_DISTORTION_TYPE_EchoTight1			14
	#define ALC_ASA_DISTORTION_TYPE_EchoTight2			15
	#define ALC_ASA_DISTORTION_TYPE_EverythingBroken	16
	#define ALC_ASA_DISTORTION_TYPE_AlienChatter		17
	#define ALC_ASA_DISTORTION_TYPE_CosmicInteference	18
	#define ALC_ASA_DISTORTION_TYPE_GoldenPi			19
	#define ALC_ASA_DISTORTION_TYPE_RadioTower			20
	#define ALC_ASA_DISTORTION_TYPE_Waves				21

	

#endif // __MAC_OSX_OAL_EXTENSIONS_H__