#include "core/border_component.h"
#include "platform/id_storage.h"

BorderComponent::BorderComponent()
    : mBorders()
{
}

void BorderComponent::SetBorders(Borders_t borders)
{
    mBorders=borders;
}

IBorderComponent::Borders_t BorderComponent::GetBorders()const
{
    return mBorders;
}

void BorderComponent::Save(Json::Value& component)
{
    Component::Save(component);
    Json::Value SettersArr(Json::arrayValue);
    Json::Value Setters(Json::objectValue);
    Json::Value BordersArr(Json::arrayValue);
    IdStorage& idStorage=IdStorage::Get();
    BorderType& borderType=BorderType::Get();
    for (IBorderComponent::Borders_t::iterator i=mBorders.begin(),e=mBorders.end();i!=e;++i)
    {
        std::string borderName;
        if (idStorage.GetName(borderType(*i),borderName))
        {
            Json::Value jName=Json::Value(borderName);
            BordersArr.append(jName);
        }
    }
    Setters["borders"]=BordersArr;
    SettersArr.append(Setters);
    component["set"]=SettersArr;
}



void BorderComponentLoader::BindValues()
{
    IBorderComponent::Borders_t borders;
    Json::Value const& json = (*mSetters)["borders"];
    if( !json.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
    {
        Json::Value& part = *i;
        BorderType::Type typ = BorderType::Get()( AutoId( part.asString() ) );
        borders.push_back( typ );
    }
    Bind<IBorderComponent::Borders_t>( &BorderComponent::SetBorders, borders );

}

BorderComponentLoader::BorderComponentLoader()
{
}
