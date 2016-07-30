#include "platform/i_platform.h"
#include "link_map_element_system.h"
#include "map_system.h"
#include "link_map_element.h"
#include "i_input.h"
#include "i_output.h"
#include "function_declarations.h"

namespace map {

LinkMapElementSystem::LinkMapElementSystem()
    : MapElementSystem()
{
}


void LinkMapElementSystem::Init()
{
    MapElementSystem::Init();
    mOnMapLoaded = EventServer<core::MapLoadedEvent>::Get().Subscribe( boost::bind( &LinkMapElementSystem::OnMapLoaded, this, _1 ) );
}


void LinkMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
}

void LinkMapElementSystem::OnMapLoaded( core::MapLoadedEvent const& Evt )
{
    for( Opt<LinkMapElement> linkMapElement : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), LinkMapElement::GetType_static() ))
    {
        for ( Opt<IInput> targetInput : MapElementListFilter<MapSystem::Identifier>( mMapSystem->GetMapElementList(), linkMapElement->GetTargetInputUID() ))
        {
            if( !targetInput.IsValid() )
            {
                L1( "Tried to link an input node that has no input nodes!" );
                continue;
            }

            int_function_t inputNode;
            if ( linkMapElement->GetTargetInputNodeId() != -1 )
            {
                inputNode = targetInput->GetInputNodeId( linkMapElement->GetTargetInputNodeId() );
            }
            else if ( linkMapElement->GetTargetInputNodeOrdinal() != -1 )
            {
                inputNode = targetInput->GetInputNode( linkMapElement->GetTargetInputNodeOrdinal() );
            }

            for( Opt<IOutput> targetOutput : MapElementListFilter<MapSystem::Identifier>( mMapSystem->GetMapElementList(), linkMapElement->GetTargetOutputUID() ))
            {
                if( !targetOutput.IsValid() )
                {
                    L1( "Tried to link to an output node that has no output nodes!" );
                    continue;
                }
                if( linkMapElement->GetTargetOutputNodeId() != -1 )
                {
                    targetOutput->PlugInNodeId( linkMapElement->GetTargetOutputNodeId(), inputNode );
                }
                else if ( linkMapElement->GetTargetOutputNodeOrdinal() != -1 )
                {
                    targetOutput->PlugInNode( linkMapElement->GetTargetOutputNodeOrdinal(), inputNode );
                }
            }

        }
    }
}


} // namespace map

