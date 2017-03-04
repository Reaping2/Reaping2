#include "core/border_component.h"
#include "platform/id_storage.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

BorderComponent::BorderComponent()
    : mBorders()
    , mChanged(false)
    , mSpriteIndex( 0 )
{
}

void BorderComponent::SetBorders( Borders_t borders )
{
    mBorders = borders;
}

IBorderComponent::Borders_t BorderComponent::GetBorders()const
{
    return mBorders;
}

void BorderComponent::SetChanged( bool changed )
{
    mChanged = changed;
}

bool BorderComponent::IsChanged() const
{
    return mChanged;
}

void BorderComponent::SetRandomSprites( RandomSprites_t const& randomSprites )
{
    mRandomSprites = randomSprites;
}

IBorderComponent::RandomSprites_t const& BorderComponent::GetRandomSprites() const
{
    return mRandomSprites;
}

void BorderComponent::SetSpriteIndex( int32_t spriteIndex )
{
    mSpriteIndex = spriteIndex;
}

int32_t BorderComponent::GetSpriteIndex() const
{
    return mSpriteIndex;
}


void BorderComponent::Save( Json::Value& component )
{
    Component::Save( component );
    Json::Value SettersArr( Json::arrayValue );
    Json::Value Setters( Json::objectValue );
    IdStorage& idStorage = IdStorage::Get();
    BorderType& borderType = BorderType::Get();
    {
        Json::Value BordersArr( Json::arrayValue );
        for ( IBorderComponent::Borders_t::iterator i = mBorders.begin(), e = mBorders.end(); i != e; ++i )
        {
            std::string borderName;
            if ( idStorage.GetName( borderType( *i ), borderName ) )
            {
                Json::Value jName = Json::Value( borderName );
                BordersArr.append( jName );
            }
        }
        Setters["borders"] = BordersArr;
    }
    {
        Json::Value BordersArr( Json::arrayValue );
        for ( IBorderComponent::Borders_t::iterator i = mOuterBorders.begin(), e = mOuterBorders.end(); i != e; ++i )
        {
            std::string borderName;
            if ( idStorage.GetName( borderType( *i ), borderName ) )
            {
                Json::Value jName = Json::Value( borderName );
                BordersArr.append( jName );
            }
        }
        Setters["outer_borders"] = BordersArr;
    }
    SettersArr.append( Setters );

    component["set"] = SettersArr;
}

void BorderComponent::SetOuterBorders( Borders_t borders )
{
    mOuterBorders = borders;
}

IBorderComponent::Borders_t BorderComponent::GetOuterBorders() const
{
    return mOuterBorders;
}



void BorderComponentLoader::BindValues()
{
    {
        Json::Value const& json = ( *mSetters )["borders"];
        if( json.isArray() )
        {
            IBorderComponent::Borders_t borders;
            for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
            {
                Json::Value& part = *i;
                BorderType::Type typ = BorderType::Get()( AutoId( part.asString() ) );
                borders.push_back( typ );
            }
            Bind<IBorderComponent::Borders_t>( &BorderComponent::SetBorders, borders );
        }
    }
    {
        Json::Value const& json = ( *mSetters )["outer_borders"];
        if( json.isArray() )
        {
            IBorderComponent::Borders_t borders;
            for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
            {
                Json::Value& part = *i;
                BorderType::Type typ = BorderType::Get()( AutoId( part.asString() ) );
                borders.push_back( typ );
            }
            Bind<IBorderComponent::Borders_t>( &BorderComponent::SetOuterBorders, borders );
        }
    }
    IBorderComponent::RandomSprites_t randomSprites;
    auto const& json = (*mSetters)["random_sprites"];
    if (json.isArray()&&!json.empty())
    {
        for (auto& chance : json)
        {
            randomSprites.push_back( chance.asInt() );
        }
        Bind<IBorderComponent::RandomSprites_t>( &BorderComponent::SetRandomSprites, randomSprites );
    }
}

BorderComponentLoader::BorderComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( BorderComponent, BorderComponent );
