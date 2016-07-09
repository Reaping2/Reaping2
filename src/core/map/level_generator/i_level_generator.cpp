#include "i_level_generator.h"
#include "platform/auto_id.h"
#include "random.h"
#include "platform/settings.h"

using platform::AutoId;

namespace map {

ILevelGenerator::ILevelGenerator( int32_t Id )
    : mId( Id )
    , mScene( Scene::Get() )
{
    mCellSize = Settings::Get().GetInt( "generator.cell_size", 1000 );
    mCellCount = Settings::Get().GetInt( "generator.cell_count", 5 );;
    mRand.seed( Settings::Get().GetUInt( "generator.seed", unsigned( std::time( 0 ) ) ) );
}


ILevelGenerator::~ILevelGenerator()
{

}

void ILevelGenerator::AddPossibleRoom( int32_t roomId, int32_t possibility )
{
    for (int i = 0; i < possibility; ++i)
    {
        mPossibleRoomIds.push_back( roomId );
    }
}


} // namespace map

