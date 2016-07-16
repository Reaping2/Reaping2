#include "platform/i_platform.h"
#include "level_generator_map_element_system.h"
#include "level_generator_map_element.h"

namespace map {

LevelGeneratorMapElementSystem::LevelGeneratorMapElementSystem()
    : MapElementSystem()
    , mLevelGeneratorRepo( LevelGeneratorRepo::Get() )
{
}


void LevelGeneratorMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void LevelGeneratorMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update(DeltaTime);
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),LevelGeneratorMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator levelGeneratorMapElementIt = mapElementListFilter.begin(), levelGeneratorMapElementE = mapElementListFilter.end(); levelGeneratorMapElementIt != levelGeneratorMapElementE; ++levelGeneratorMapElementIt )
    {
        Opt<LevelGeneratorMapElement> levelGeneratorMapElement(*levelGeneratorMapElementIt);
        if (levelGeneratorMapElement->GetValueId( LevelGeneratorMapElement::GeneratorNodeId() ) > 0)
        {
            mLevelGeneratorRepo( levelGeneratorMapElement->GetGeneratorId() ).Generate();
            levelGeneratorMapElement->ResetValues();
        }
    }
}


} // namespace map

