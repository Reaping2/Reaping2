#ifndef INCLUDED_AUDIO_I_AUDIO_H
#define INCLUDED_AUDIO_I_AUDIO_H
#include "platform/i_platform.h"

#ifdef PORTAUDIO_CMAKE_GENERATED
#include <options_cmake.h>
#endif

#include <portaudio.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "audio_effect.h"
#include "audio_buffer.h"
#include "vorbis_file.h"
#include "vorbis_file_cache.h"
#include "audio_file.h"
#include "mixer.h"
#include "audio_player.h"

// bindings (might very well be placed into another project)
#include "audio_effect_repo.h"

#endif//INCLUDED_AUDIO_I_AUDIO_H
