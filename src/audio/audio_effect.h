#pragma once
#ifndef INCLUDED_AUDIOEFFECT_H
#define INCLUDED_AUDIOEFFECT_H

#include "platform/rstdint.h"
#include <boost/filesystem/path.hpp>

namespace audio {
enum PlayMode
{
    PlayOnce = 0,
    Repeat,
};
enum AudioType
{
    Music = 0,
    Effect,
    Speech,
    NumTypes,
};
struct AudioEffect
{
    boost::filesystem::path Path;
    bool Interruptable;
    AudioType Type;
    PlayMode Mode;
    AudioEffect( boost::filesystem::path const& p, bool i, AudioType t, PlayMode m )
        : Path( p ), Interruptable( i ), Type( t ), Mode( m ) {}
};
}

#endif // INCLUDED_AUDIOEFFECT_H

