#ifndef GAMEMODES_H
#define GAMEMODES_H

#include <string> 
namespace core
{
    struct GameModes
    {
        enum GameMode
        {
            FFA,
            CTF,
            Rogue,
            Unknown
        };

        static GameMode FromString(std::string const& s);
        static std::string ToString(GameMode g);
    };

}

#endif
