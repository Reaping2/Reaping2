#ifndef INCLUDED_AUDIO_AUDIO_EFFECT_PLAYER_H
#define INCLUDED_AUDIO_AUDIO_EFFECT_PLAYER_H

// needed for AudibleEvent
#include "core/i_core.h"

class AudioEffectPlayer : public Singleton<AudioEffectPlayer>
{
	friend class Singleton<AudioEffectPlayer>;
	AudioEffectPlayer();
	void OnAudibleEvent(AudibleEvent const& Evt);
	void Load();
	typedef std::map<int32_t,boost::filesystem::path> Events_t;
	Events_t mEvents;
	AutoReg mOnAudibleEventId;
};

#endif//INCLUDED_AUDIO_AUDIO_EFFECT_PLAYER_H
