#ifndef INCLUDED_AUDIO_AUDIO_FILE_H
#define INCLUDED_AUDIO_AUDIO_FILE_H

#include "platform/i_platform.h"
#include "audio/audio_effect.h"
#include "vorbis_file_cache.h"

// todo: rename
class AudioFile
{
private:
    int32_t mUID;
    audio::AudioEffect const& mEffect;
    static const size_t TargetBufferSize;
    VorbisFileCache::Elem mSource;
    size_t mPosInSource;
    AudioBuffer mBuffer;
    bool mFinishedPlaying;
    glm::vec2 mPos;
    AudioFile( int32_t uid, VorbisFileCache::Elem Source, audio::AudioEffect const& Effect, glm::vec2 const& pos );
public:
    bool FillBufferIfNeeded();
    AudioBuffer& GetBuffer();
    audio::AudioType GetType() const;
    audio::PlayMode GetMode() const;
    double GetVolume() const;
    int32_t GetUID() const;
    bool IsInterruptable() const;
    bool IsFinishedPlaying() const;
    glm::vec2 const& GetPosition() const;
    void SetPosition( glm::vec2 const& pos );
    static std::auto_ptr<AudioFile> Create( int32_t uid, audio::AudioEffect const& Effect, glm::vec2 const& pos );
};

typedef boost::ptr_vector<AudioFile> AudioFiles_t;

#endif//INCLUDED_AUDIO_AUDIO_FILE_H
