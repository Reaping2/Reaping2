#include "core/game_modes.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace core {

GameModes::GameModes()
{
    mIdToGameModesMap.insert(IdToGameModesMap_t::value_type(AutoId("ctf"),GameModes::CTF));
    mIdToGameModesMap.insert(IdToGameModesMap_t::value_type(AutoId("ffa"),GameModes::FFA));
    mIdToGameModesMap.insert(IdToGameModesMap_t::value_type(AutoId("rogue"),GameModes::Rogue));
    mIdToGameModesMap.insert(IdToGameModesMap_t::value_type(AutoId("unknown"),GameModes::Unknown));
}

GameModes::Type GameModes::operator()( int32_t Id ) const
{
    IdToGameModesMap_t::left_const_iterator i=mIdToGameModesMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToGameModesMap.left.end());
    return (i!=mIdToGameModesMap.left.end())?i->second:GameModes::CTF;
}

int32_t GameModes::operator()( Type type ) const
{
    IdToGameModesMap_t::right_const_iterator i=mIdToGameModesMap.right.find(type);
    BOOST_ASSERT(i!=mIdToGameModesMap.right.end());
    return (i!=mIdToGameModesMap.right.end())?i->second:GameModes::CTF;
}

} // namespace core

