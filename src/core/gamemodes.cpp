#include "gamemodes.h"
using namespace core;

GameModes::GameMode GameModes::FromString(std::string const& s)
{
    GameMode mode = Unknown;
    if ( "ffa" == s )
    {
        mode = FFA;
    }
    else if ( "ctf" == s )
    {
        mode = CTF;
    }
    else if ( "rogue" == s )
    {
        mode = Rogue;
    }
    return mode;
}

std::string GameModes::ToString( GameMode mode )
{
    std::string s;
    switch( mode )
    {
        case FFA:
            s = "ffa";
            break;
        case CTF:
            s = "ctf";
            break;
        case Rogue:
            s = "rogue";
            break;
        case Unknown:
        default:
            s = "unknown";
            break;
    }
    return s;
}
