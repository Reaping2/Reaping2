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


void ILevelGenerator::Load( Json::Value const& setters )
{
    Json::GetInt( setters["cell_size"], mCellSize );
    Json::GetInt( setters["cell_count"], mCellCount );
    Json::GetUInt( setters["seed"], mSeed );
}


DefaultLevelGenerator::DefaultLevelGenerator( int32_t Id )
    : ILevelGenerator( Id )
{

}

void DefaultLevelGenerator::Generate()
{

}

} // namespace map

