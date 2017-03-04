#ifndef INCLUDED_CORE_GAME_MODES_H
#define INCLUDED_CORE_GAME_MODES_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

namespace core {

class GameModes : public platform::Singleton<GameModes>
{
protected:
    friend class platform::Singleton<GameModes>;
    GameModes();
public:
    enum Type : int32_t
    {
        CTF=0,
        FFA,
        Rogue,
        Unknown,
        Num_Classes
    };
    GameModes::Type operator()( int32_t Id ) const;
    int32_t operator()( GameModes::Type type ) const;
private:
    typedef boost::bimap<int32_t,GameModes::Type> IdToGameModesMap_t;
    IdToGameModesMap_t mIdToGameModesMap;
};

} // namespace core

#endif//INCLUDED_CORE_GAME_MODES_H


//command:  "./classgenerator" -c "game_modes" -g "enum" -m "CTF-ctf FFA-ffa Rogue-rogue Unknown-unknown" -n "core"
