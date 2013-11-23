#ifndef INCLUDED_AUDIO_MIXER_H
#define INCLUDED_AUDIO_MIXER_H

class Mixer
{
	float mAudioTypeRelativeWeights[AudioFile::NumTypes];
public:
	Mixer();
	void Mix(AudioBuffer& Dest, AudioFiles_t& Files, size_t const Size);
};

#endif//INCLUDED_AUDIO_MIXER_H
