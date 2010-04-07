/**
 * OpenAL cross platform audio library
 * Copyright (C) 2008 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA  02111-1307, USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#ifndef AL_ALEXT_H
#define AL_ALEXT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AL_LOKI_IMA_ADPCM_format
#define AL_LOKI_IMA_ADPCM_format 1
#define AL_FORMAT_IMA_ADPCM_MONO16_EXT           0x10000
#define AL_FORMAT_IMA_ADPCM_STEREO16_EXT         0x10001
#endif

#ifndef AL_LOKI_WAVE_format
#define AL_LOKI_WAVE_format 1
#define AL_FORMAT_WAVE_EXT                       0x10002
#endif

#ifndef AL_EXT_vorbis
#define AL_EXT_vorbis 1
#define AL_FORMAT_VORBIS_EXT                     0x10003
#endif

#ifndef AL_LOKI_quadriphonic
#define AL_LOKI_quadriphonic 1
#define AL_FORMAT_QUAD8_LOKI                     0x10004
#define AL_FORMAT_QUAD16_LOKI                    0x10005
#endif

#ifndef AL_EXT_float32
#define AL_EXT_float32 1
#define AL_FORMAT_MONO_FLOAT32                   0x10010
#define AL_FORMAT_STEREO_FLOAT32                 0x10011
#endif

#ifndef AL_EXT_double
#define AL_EXT_double 1
#define AL_FORMAT_MONO_DOUBLE_EXT                0x10012
#define AL_FORMAT_STEREO_DOUBLE_EXT              0x10013
#endif

#ifndef ALC_LOKI_audio_channel
#define ALC_LOKI_audio_channel 1
#define ALC_CHAN_MAIN_LOKI                       0x500001
#define ALC_CHAN_PCM_LOKI                        0x500002
#define ALC_CHAN_CD_LOKI                         0x500003
#endif

#ifndef ALC_ENUMERATE_ALL_EXT
#define ALC_ENUMERATE_ALL_EXT 1
#define ALC_DEFAULT_ALL_DEVICES_SPECIFIER        0x1012
#define ALC_ALL_DEVICES_SPECIFIER                0x1013
#endif

#ifndef AL_EXT_MCFORMATS
#define AL_EXT_MCFORMATS 1
#define AL_FORMAT_QUAD8                          0x1204
#define AL_FORMAT_QUAD16                         0x1205
#define AL_FORMAT_QUAD32                         0x1206
#define AL_FORMAT_REAR8                          0x1207
#define AL_FORMAT_REAR16                         0x1208
#define AL_FORMAT_REAR32                         0x1209
#define AL_FORMAT_51CHN8                         0x120A
#define AL_FORMAT_51CHN16                        0x120B
#define AL_FORMAT_51CHN32                        0x120C
#define AL_FORMAT_61CHN8                         0x120D
#define AL_FORMAT_61CHN16                        0x120E
#define AL_FORMAT_61CHN32                        0x120F
#define AL_FORMAT_71CHN8                         0x1210
#define AL_FORMAT_71CHN16                        0x1211
#define AL_FORMAT_71CHN32                        0x1212
#endif

#ifndef AL_EXT_MULAW_MCFORMATS
#define AL_EXT_MULAW_MCFORMATS 1
#define AL_FORMAT_MONO_MULAW                     0x10014
#define AL_FORMAT_STEREO_MULAW                   0x10015
#define AL_FORMAT_QUAD_MULAW                     0x10021
#define AL_FORMAT_REAR_MULAW                     0x10022
#define AL_FORMAT_51CHN_MULAW                    0x10023
#define AL_FORMAT_61CHN_MULAW                    0x10024
#define AL_FORMAT_71CHN_MULAW                    0x10025
#endif

#ifndef AL_EXT_IMA4
#define AL_EXT_IMA4 1
#define AL_FORMAT_MONO_IMA4                      0x1300
#define AL_FORMAT_STEREO_IMA4                    0x1301
#endif

#ifndef AL_EXT_buffer_sub_data
#define AL_EXT_buffer_sub_data 1
#define AL_BYTE_RW_OFFSETS_EXT                   0x1031
#define AL_SAMPLE_RW_OFFSETS_EXT                 0x1032
typedef ALvoid (AL_APIENTRY*PFNALBUFFERSUBDATAEXTPROC)(ALuint,ALenum,const ALvoid*,ALsizei,ALsizei);
#ifdef AL_ALEXT_PROTOTYPES
AL_API ALvoid AL_APIENTRY alBufferSubDataEXT(ALuint buffer,ALenum format,const ALvoid *data,ALsizei offset,ALsizei length);
#endif
#endif

#ifndef AL_EXT_STATIC_BUFFER
#define AL_EXT_STATIC_BUFFER 1
typedef ALvoid (AL_APIENTRY*PFNALBUFFERDATASTATICPROC)(const ALint,ALenum,ALvoid*,ALsizei,ALsizei);
#ifdef AL_ALEXT_PROTOTYPES
AL_API ALvoid AL_APIENTRY alBufferDataStatic(const ALint buffer, ALenum format, ALvoid *data, ALsizei len, ALsizei freq);
#endif
#endif

#ifndef AL_EXT_sample_buffer_object
#define AL_EXT_sample_buffer_object 1
typedef ptrdiff_t ALintptrEXT;
typedef ptrdiff_t ALsizeiptrEXT;
#define AL_SAMPLE_SOURCE_EXT                     0x1040
#define AL_SAMPLE_SINK_EXT                       0x1041
#define AL_READ_ONLY_EXT                         0x1042
#define AL_WRITE_ONLY_EXT                        0x1043
#define AL_READ_WRITE_EXT                        0x1044
#define AL_STREAM_WRITE_EXT                      0x1045
#define AL_STREAM_READ_EXT                       0x1046
#define AL_STREAM_COPY_EXT                       0x1047
#define AL_STATIC_WRITE_EXT                      0x1048
#define AL_STATIC_READ_EXT                       0x1049
#define AL_STATIC_COPY_EXT                       0x104A
#define AL_DYNAMIC_WRITE_EXT                     0x104B
#define AL_DYNAMIC_READ_EXT                      0x104C
#define AL_DYNAMIC_COPY_EXT                      0x104D
typedef ALvoid (AL_APIENTRY*PFNALGENDATABUFFERSEXTPROC)(ALsizei n,ALuint *puiBuffers);
typedef ALvoid (AL_APIENTRY*PFNALDELETEDATABUFFERSEXTPROC)(ALsizei n, const ALuint *puiBuffers);
typedef ALboolean (AL_APIENTRY*PFNALISDATABUFFEREXTPROC)(ALuint uiBuffer);
typedef ALvoid (AL_APIENTRY*PFNALDATABUFFERDATAEXTPROC)(ALuint buffer,const ALvoid *data,ALsizeiptrEXT size,ALenum usage);
typedef ALvoid (AL_APIENTRY*PFNALDATABUFFERSUBDATAEXTPROC)(ALuint buffer, ALintptrEXT start, ALsizeiptrEXT length, const ALvoid *);
typedef ALvoid (AL_APIENTRY*PFNALGETDATABUFFERSUBDATAEXTPROC)(ALuint buffer, ALintptrEXT start, ALsizeiptrEXT length, ALvoid *);
typedef ALvoid (AL_APIENTRY*PFNALDATABUFFERFEXTPROC)(ALuint buffer, ALenum eParam, ALfloat flValue);
typedef ALvoid (AL_APIENTRY*PFNALDATABUFFERFVEXTPROC)(ALuint buffer, ALenum eParam, const ALfloat* flValues);
typedef ALvoid (AL_APIENTRY*PFNALDATABUFFERIEXTPROC)(ALuint buffer, ALenum eParam, ALint lValue);
typedef ALvoid (AL_APIENTRY*PFNALDATABUFFERIVEXTPROC)(ALuint buffer, ALenum eParam, const ALint* plValues);
typedef ALvoid (AL_APIENTRY*PFNALGETDATABUFFERFEXTPROC)(ALuint buffer, ALenum eParam, ALfloat *pflValue);
typedef ALvoid (AL_APIENTRY*PFNALGETDATABUFFERFVEXTPROC)(ALuint buffer, ALenum eParam, ALfloat* pflValues);
typedef ALvoid (AL_APIENTRY*PFNALGETDATABUFFERIEXTPROC)(ALuint buffer, ALenum eParam, ALint *plValue);
typedef ALvoid (AL_APIENTRY*PFNALGETDATABUFFERIVEXTPROC)(ALuint buffer, ALenum eParam, ALint* plValues);
typedef ALvoid (AL_APIENTRY*PFNALSELECTDATABUFFEREXTPROC)(ALenum target, ALuint uiBuffer);
typedef ALvoid* (AL_APIENTRY*PFNALMAPDATABUFFEREXTPROC)(ALuint uiBuffer, ALintptrEXT start, ALsizeiptrEXT length, ALenum access);
typedef ALvoid (AL_APIENTRY*PFNALUNMAPDATABUFFEREXTPROC)(ALuint uiBuffer);
#ifdef AL_ALEXT_PROTOTYPES
AL_API ALvoid AL_APIENTRY alGenDatabuffersEXT(ALsizei n,ALuint *puiBuffers);
AL_API ALvoid AL_APIENTRY alDeleteDatabuffersEXT(ALsizei n, const ALuint *puiBuffers);
AL_API ALboolean AL_APIENTRY alIsDatabufferEXT(ALuint uiBuffer);
AL_API ALvoid AL_APIENTRY alDatabufferDataEXT(ALuint buffer,const ALvoid *data,ALsizeiptrEXT size,ALenum usage);
AL_API ALvoid AL_APIENTRY alDatabufferSubDataEXT(ALuint buffer, ALintptrEXT start, ALsizeiptrEXT length, const ALvoid *data);
AL_API ALvoid AL_APIENTRY alGetDatabufferSubDataEXT(ALuint buffer, ALintptrEXT start, ALsizeiptrEXT length, ALvoid *data);
AL_API ALvoid AL_APIENTRY alDatabufferfEXT(ALuint buffer, ALenum eParam, ALfloat flValue);
AL_API ALvoid AL_APIENTRY alDatabufferfvEXT(ALuint buffer, ALenum eParam, const ALfloat* flValues);
AL_API ALvoid AL_APIENTRY alDatabufferiEXT(ALuint buffer, ALenum eParam, ALint lValue);
AL_API ALvoid AL_APIENTRY alDatabufferivEXT(ALuint buffer, ALenum eParam, const ALint* plValues);
AL_API ALvoid AL_APIENTRY alGetDatabufferfEXT(ALuint buffer, ALenum eParam, ALfloat *pflValue);
AL_API ALvoid AL_APIENTRY alGetDatabufferfvEXT(ALuint buffer, ALenum eParam, ALfloat* pflValues);
AL_API ALvoid AL_APIENTRY alGetDatabufferiEXT(ALuint buffer, ALenum eParam, ALint *plValue);
AL_API ALvoid AL_APIENTRY alGetDatabufferivEXT(ALuint buffer, ALenum eParam, ALint* plValues);
AL_API ALvoid AL_APIENTRY alSelectDatabufferEXT(ALenum target, ALuint uiBuffer);
AL_API ALvoid* AL_APIENTRY alMapDatabufferEXT(ALuint uiBuffer, ALintptrEXT start, ALsizeiptrEXT length, ALenum access);
AL_API ALvoid AL_APIENTRY alUnmapDatabufferEXT(ALuint uiBuffer);
#endif
#endif

#ifndef ALC_EXT_EFX
#define ALC_EXT_EFX 1
#include "efx.h"
#endif

#ifndef ALC_EXT_disconnect
#define ALC_EXT_disconnect 1
#define ALC_CONNECTED                            0x313
#endif

#ifndef ALC_EXT_thread_local_context
#define ALC_EXT_thread_local_context 1
typedef ALCboolean  (ALC_APIENTRY*PFNALCSETTHREADCONTEXTPROC)(ALCcontext *context);
typedef ALCcontext* (ALC_APIENTRY*PFNALCGETTHREADCONTEXTPROC)(void);
#ifdef AL_ALEXT_PROTOTYPES
ALC_API ALCboolean  ALC_APIENTRY alcSetThreadContext(ALCcontext *context);
ALC_API ALCcontext* ALC_APIENTRY alcGetThreadContext(void);
#endif
#endif

#ifndef AL_EXT_source_distance_model
#define AL_EXT_source_distance_model 1
#define AL_SOURCE_DISTANCE_MODEL                 0x200
#endif

#ifdef __cplusplus
}
#endif

#endif
