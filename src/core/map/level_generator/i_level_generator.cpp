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

} // namespace map

