#include "grid.h"
#include <json/json.h>

GridWidget::GridWidget( int32_t Id )
    : Widget( Id )
{
}

void GridWidget::Init( Json::Value& Descriptor )
{
    Descriptor["visible"] = 1;
    BaseClass::Init( Descriptor );
    ParseIntProp( PT_Columns, Descriptor["cols"], 1 );
    ParseIntProp( PT_Rows, Descriptor["rows"], 1 );
    ParseStrProp( PT_Source, Descriptor["source"], "" );
    ParseIntProp( PT_StartId, Descriptor["startid"], 0 );
    Descriptor["startid_base"] = Descriptor["startid"].asString() + ".base";
    ParseStrProp( PT_StartIdBase, Descriptor["startid_base"], "" );
    Json::Value& Template = Descriptor["template"];
    Json::Value GridJson;
    GridJson["children"] = Template;
    int32_t cols = operator()( PT_Columns );
    int32_t rows = operator()( PT_Rows );
    GridJson["w"] = 1.0 / cols;
    GridJson["h"] = 1.0 / rows;
    GridJson["visible"] = 1;
    GridJson["color"] = "0xffffff00";
    GridJson["source"] = Descriptor["source"];
    GridJson["startid"] = Descriptor["startid"];
    GridJson["startid_step"] = Descriptor["startid"].asString() + ".step";
    int32_t index = 0;
    for( int32_t x = 0; x < cols; ++x )
    {
        for( int32_t y = 0; y < rows; ++y )
        {
            std::auto_ptr<GridElem> elem( new GridElem( AutoId( "grid_elem" ) ) );
            GridJson["x"] = 1.0 - ( 1 + x ) * 1.0 / cols;
            GridJson["y"] = y * 1.0 / rows;
            GridJson["index"] = index++;
            elem->Init( GridJson );
            AddChild( elem.release() );
        }
    }
}

GridElem::GridElem( int32_t Id )
    : Widget( Id )
{
}

void GridElem::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    Json::Value dummy;
    ParseIntProp( PT_SubtreeHidden, dummy, 0 );
    ParseIntProp( PT_Index, Descriptor["index"], 0 );
    ParseStrProp( PT_Source, Descriptor["source"], "" );
    ParseIntProp( PT_StartId, Descriptor["startid"], 0 );
    ParseStrProp( PT_StartIdStep, Descriptor["startid_step"], "" );
}



