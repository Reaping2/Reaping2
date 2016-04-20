#include "move_message.h"
#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>

namespace network {

bool MoveMessage::operator==( MoveMessage const& other )
{
    return mActorGUID == other.mActorGUID
        && mHeadingModifier == other.mHeadingModifier
        && mSpeed == other.mSpeed
        && mPercent == other.mPercent
        && mFlat == other.mFlat
        && mMoving == other.mMoving
        && mRooted == other.mRooted;
}

}//namepsace network
REAPING2_CLASS_EXPORT_IMPLEMENT( network__MoveMessage, network::MoveMessage );
