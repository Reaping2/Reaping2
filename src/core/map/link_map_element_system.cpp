#include "platform/i_platform.h"
#include "link_map_element_system.h"
#include "map_system.h"
#include "link_map_element.h"
#include "i_input.h"
#include "i_output.h"
#include "function_declarations.h"

namespace map {

LinkMapElementSystem::LinkMapElementSystem()
    : mScene( Scene::Get() )
{
}


void LinkMapElementSystem::Init()
{
    mMapSystem=MapSystem::Get();
    mOnMapLoaded=EventServer<core::MapLoadedEvent>::Get().Subscribe( boost::bind( &LinkMapElementSystem::OnMapLoaded, this, _1 ) );
}


void LinkMapElementSystem::Update(double DeltaTime)
{

}

void LinkMapElementSystem::OnMapLoaded(core::MapLoadedEvent const& Evt)
{
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),LinkMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator linkMapElementIt = mapElementListFilter.begin(), linkMapElementE = mapElementListFilter.end(); linkMapElementIt != linkMapElementE; ++linkMapElementIt )
    {
        Opt<LinkMapElement> linkMapElement(*linkMapElementIt);
        MapElementListFilter<MapSystem::UID> targetInputUIDs(mMapSystem->GetMapElementList(),linkMapElement->GetTargetInputUID());
        for( MapElementListFilter<MapSystem::UID>::const_iterator targetInputUIDIt = targetInputUIDs.begin(), targetInputUIDE = targetInputUIDs.end(); targetInputUIDIt != targetInputUIDE; ++targetInputUIDIt )
        {
            Opt<IInput> targetInput(*targetInputUIDIt);
            if(!targetInput.IsValid())
            {
                L1("Tried to link an input node that has no input nodes!");
                continue;
            }

            int_function_t inputNode;
            if (linkMapElement->GetTargetInputNodeId()!=-1)
            {
                inputNode=targetInput->GetInputNodeId(linkMapElement->GetTargetInputNodeId());
            }
            else if (linkMapElement->GetTargetInputNodeOrdinal()!=-1)
            {
                inputNode=targetInput->GetInputNode(linkMapElement->GetTargetInputNodeOrdinal());
            }

            MapElementListFilter<MapSystem::UID> targetOutputUIDs(mMapSystem->GetMapElementList(),linkMapElement->GetTargetOutputUID());
            for( MapElementListFilter<MapSystem::UID>::const_iterator targetOutputUIDIt = targetOutputUIDs.begin(), targetOutputUIDE = targetOutputUIDs.end(); targetOutputUIDIt != targetOutputUIDE; ++targetOutputUIDIt )
            {
                Opt<IOutput> targetOutput(*targetOutputUIDIt);
                if(!targetOutput.IsValid())
                {
                    L1("Tried to link to an output node that has no output nodes!");
                    continue;
                }
                if(linkMapElement->GetTargetOutputNodeId()!=-1)
                {
                    targetOutput->PlugInNodeId(linkMapElement->GetTargetInputNodeId(),inputNode);
                }
                else if (linkMapElement->GetTargetOutputNodeOrdinal()!=-1)
                {
                    targetOutput->PlugInNode(linkMapElement->GetTargetOutputNodeOrdinal(),inputNode);
                }
            }

        }
    }
}


} // namespace map

