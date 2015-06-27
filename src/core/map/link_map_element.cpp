#include "core/map/link_map_element.h"
#include "platform/auto_id.h"

namespace map {

LinkMapElement::LinkMapElement()
    : MapElement()
    , mTargetInputUID(-1)
    , mTargetInputOrdinal(-1)
    , mTargetOutputUID(-1)
    , mTargetOutputOrdinal(-1)
{
}

void LinkMapElement::Load(Json::Value& setters)
{
    Json::Value const& targetOutput = setters["from"];
    if(!targetOutput.isObject())
    {
        return;
    }
    Json::Value const& targetInput = setters["to"];
    if(!targetInput.isObject())
    {
        return;
    }

    std::string targetOutputName;
    if (!Json::GetStr(targetOutput["identifier"],targetOutputName))
    {
        return;
    }
    mTargetOutputUID=AutoId(targetOutputName);
    std::string targetOutputNodeName;
    int32_t targetOutputNodeOrdinal;
    if (Json::GetStr(targetOutput["node"],targetOutputNodeName))
    {
        targetOutputNodeOrdinal=AutoId(targetOutputNodeName);
    }
    else if (Json::GetInt(targetOutput["node"],targetOutputNodeOrdinal))
    {
        //ok it was a numeric
    }
    else
    {
        targetOutputNodeOrdinal=0; //first node by default
    }
    mTargetOutputOrdinal=targetOutputNodeOrdinal;



    std::string targetInputName;
    if (!Json::GetStr(targetInput["identifier"],targetInputName))
    {
        return;
    }
    mTargetInputUID=AutoId(targetInputName);
    std::string targetInputNodeName;
    int32_t targetInputNodeOrdinal;
    if (Json::GetStr(targetInput["node"],targetInputNodeName))
    {
        targetInputNodeOrdinal=AutoId(targetInputNodeName);
    }
    else if (Json::GetInt(targetInput["node"],targetInputNodeOrdinal))
    {
        //ok it was a numeric
    }
    else
    {
        targetInputNodeOrdinal=0; //first node by default
    }
    mTargetInputOrdinal=targetInputNodeOrdinal;

}
} // namespace map
