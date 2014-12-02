#include "core/renderable_layer.h"
#include "platform/auto_id.h"

using platform::AutoId;

RenderableLayer::RenderableLayer()
{
    mIdToRendLayerMap[AutoId("background")]=RenderableLayer::Background;
    mIdToRendLayerMap[AutoId("corpses")]=RenderableLayer::Corpses;
    mIdToRendLayerMap[AutoId("creeps")]=RenderableLayer::Creeps;
    mIdToRendLayerMap[AutoId("players")]=RenderableLayer::Players;
    mIdToRendLayerMap[AutoId("buildings")]=RenderableLayer::Buildings;
}

RenderableLayer::Type RenderableLayer::operator()( int32_t Id ) const
{
    IdToRendLayerMap_t::const_iterator i=mIdToRendLayerMap.find(Id);
    BOOST_ASSERT(i!=mIdToRendLayerMap.end());
    return (i!=mIdToRendLayerMap.end())?i->second:RenderableLayer::Background;
}
