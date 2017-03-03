#include "core/map/link_map_element.h"
#include "platform/auto_id.h"

namespace map {

LinkMapElement::LinkMapElement( int32_t Id )
    : MapElement( Id )
    , mTargetInputNodeId( -1 )
    , mTargetInputNodeOrdinal( -1 )
    , mTargetInputUID( -1 )
    , mTargetOutputNodeId( -1 )
    , mTargetOutputNodeOrdinal( -1 )
    , mTargetOutputUID( -1 )
{
}

void LinkMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    Json::Value const& targetOutput = setters["from"];
    if( !targetOutput.isObject() )
    {
        return;
    }
    Json::Value const& targetInput = setters["to"];
    if( !targetInput.isObject() )
    {
        return;
    }

    std::string targetOutputName;
    if ( !Json::GetStr( targetOutput["identifier"], targetOutputName ) )
    {
        return;
    }
    mTargetOutputUID = AutoId( targetOutputName );
    std::string targetOutputNodeName;
    if ( Json::GetStr( targetOutput["node"], targetOutputNodeName ) )
    {
        mTargetOutputNodeId = AutoId( targetOutputNodeName );
    }
    else if ( Json::GetInt( targetOutput["node"], mTargetOutputNodeOrdinal ) )
    {
        //ok it was a numeric
    }
    else
    {
        mTargetOutputNodeOrdinal = 0; //first node by default
    }




    std::string targetInputName;
    if ( !Json::GetStr( targetInput["identifier"], targetInputName ) )
    {
        return;
    }
    mTargetInputUID = AutoId( targetInputName );
    std::string targetInputNodeName;
    if ( Json::GetStr( targetInput["node"], targetInputNodeName ) )
    {
        mTargetInputNodeId = AutoId( targetInputNodeName );
    }
    else if ( Json::GetInt( targetInput["node"], mTargetInputNodeOrdinal ) )
    {
        //ok it was a numeric
    }
    else
    {
        mTargetInputNodeOrdinal = 0; //first node by default
    }
}


void LinkMapElement::SetTargetInputUID( int32_t targetInputUID )
{
    mTargetInputUID = targetInputUID;
}

int32_t LinkMapElement::GetTargetInputUID()const
{
    return mTargetInputUID;
}

void LinkMapElement::SetTargetInputNodeId( int32_t targetInputNodeId )
{
    mTargetInputNodeId = targetInputNodeId;
}

int32_t LinkMapElement::GetTargetInputNodeId()const
{
    return mTargetInputNodeId;
}

void LinkMapElement::SetTargetInputNodeOrdinal( int32_t targetInputNodeOrdinal )
{
    mTargetInputNodeOrdinal = targetInputNodeOrdinal;
}

int32_t LinkMapElement::GetTargetInputNodeOrdinal()const
{
    return mTargetInputNodeOrdinal;
}

void LinkMapElement::SetTargetOutputUID( int32_t targetOutputUID )
{
    mTargetOutputUID = targetOutputUID;
}

int32_t LinkMapElement::GetTargetOutputUID()const
{
    return mTargetOutputUID;
}

void LinkMapElement::SetTargetOutputNodeId( int32_t targetOutputNodeId )
{
    mTargetOutputNodeId = targetOutputNodeId;
}

int32_t LinkMapElement::GetTargetOutputNodeId()const
{
    return mTargetOutputNodeId;
}

void LinkMapElement::SetTargetOutputNodeOrdinal( int32_t targetOutputNodeOrdinal )
{
    mTargetOutputNodeOrdinal = targetOutputNodeOrdinal;
}

int32_t LinkMapElement::GetTargetOutputNodeOrdinal()const
{
    return mTargetOutputNodeOrdinal;
}

} // namespace map
