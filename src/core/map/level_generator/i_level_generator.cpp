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
    mRand.seed( unsigned( std::time( 0 ) ) );
}


ILevelGenerator::~ILevelGenerator()
{

}


void ILevelGenerator::Load( Json::Value& setters )
{
    Json::GetInt( setters["cell_size"], mCellSize );
    Json::GetInt( setters["cell_count"], mCellCount );
    Json::GetUInt( setters["seed"], mSeed );
}

void ILevelGenerator::AddPossibleRoom( int32_t roomId, int32_t possibility )
{
    for (int i = 0; i < possibility; ++i)
    {
        mPossibleRoomIds.push_back( roomId );
    }
}


DefaultLevelGenerator::DefaultLevelGenerator( int32_t Id )
    : ILevelGenerator( Id )
{

}

void DefaultLevelGenerator::Generate()
{

}

} // namespace map

