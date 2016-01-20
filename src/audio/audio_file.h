#ifndef INCLUDED_AUDIO_AUDIO_FILE_H
#define INCLUDED_AUDIO_AUDIO_FILE_H

#include "platform/i_platform.h"
#include "audio/audio_effect.h"
#include "vorbis_file_cache.h"

// todo: rename
class AudioFile
{
private:
    static const size_t TargetBufferSize;
    VorbisFileCache::Elem mSource;
    size_t mPosInSource;
    AudioBuffer mBuffer;
    audio::PlayMode const mMode;
    audio::AudioType const mType;
    bool mFinishedPlaying;
    glm::vec2 mPos;
    AudioFile( VorbisFileCache::Elem Source, audio::PlayMode Mode, audio::AudioType Type, glm::vec2 const& pos );
public:
    bool FillBufferIfNeeded();
    AudioBuffer& GetBuffer();
    audio::AudioType GetType() const;
    bool IsFinishedPlaying() const;
    glm::vec2 const& GetPosition() const;
    static std::auto_ptr<AudioFile> Create( boost::filesystem::path const& Path, audio::PlayMode Mode, audio::AudioType Type, glm::vec2 const& pos );
};

typedef boost::ptr_vector<AudioFile> AudioFiles_t;

#endif//INCLUDED_AUDIO_AUDIO_FILE_H
