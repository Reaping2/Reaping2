#include "i_level_generator.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace map {

ILevelGenerator::ILevelGenerator( int32_t Id )
    : mId( Id )
    , mScene( Scene::Get() )
{
}


ILevelGenerator::~ILevelGenerator()
{

}


bool ILevelGenerator::IsInBounds( int32_t x, int32_t y ) const
{
    return x >= 0 && y >= 0 && x < mCellCount && y < mCellCount;
}

void ILevelGenerator::AddPossibleRoom( int32_t roomId, int32_t possibility )
{
    for (int i = 0; i < possibility; ++i)
    {
        mPossibleRooms.push_back( roomId );
    }
}

PossibleRoom::PossibleRoom( int32_t roomId, int32_t possibility )
    : mRoomId( roomId )
    , mPossibility( possibility )
{

}

} // namespace map

