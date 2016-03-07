#ifndef INCLUDED_AUDIO_VORBIS_FILE_H
#define INCLUDED_AUDIO_VORBIS_FILE_H

#include "i_audio.h"

class VorbisFileCache;
class VorbisFile
{
    friend class VorbisFileCache;
    enum Status
    {
        St_None,
        St_Loading,
        St_Closed,
    };

    OggVorbis_File mVorbisFile;
    vorbis_info mInfo;
    AutoFile mSource;
    boost::mutex mMtx;
    std::auto_ptr<AudioBuffer> mBuffer;
    Status mStatus;

    bool Init();
    bool ReadInfo();
    bool ReadBlock();

    static size_t ReadFunc( void* Buffer, size_t Size, size_t Count, void* This );
    static int CloseFunc( void* This );
    static std::auto_ptr<VorbisFile> Create( AutoFile File );
    VorbisFile( AutoFile File );
public:
    ~VorbisFile();
    AudioBuffer const& GetBuffer() const;
    boost::mutex& GetMutex();
    bool IsFinishedLoading()const;
};

#endif//INCLUDED_AUDIO_VORBIS_FILE_H
