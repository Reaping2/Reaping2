#include "platform/i_platform.h"
#include "core/component.h"
#include "core/component_factory.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

const double PRECISION = 1000.0;

Component::Component()
    : mActorGUID( -1 )
    , mId( -1 )
{
}
Component::~Component()
{
}

void Component::SetActorGUID( int32_t actorGUID )
{
    mActorGUID = actorGUID;
}

void Component::SetId( int32_t id )
{
    mId = id;
}

int32_t Component::GetId() const
{
    return mId;
}

void Component::Save( Json::Value& component )
{
    std::string componentName;
    if ( IdStorage::Get().GetName( mId, componentName ) )
    {
        component["name"] = Json::Value( componentName );
    }
}

void ComponentHolder::AddComponent( std::auto_ptr<Component> Comp )
{
    int const type = Comp->GetType();
    auto it = mComponents.find( type );
    if (it != mComponents.end())
    {
        delete it->second;
        mComponents.erase( it );
    }
    mComponents.emplace( type, Comp.release() );
}

void ComponentHolder::DropComponent( int32_t id )
{
    for( auto it = std::begin( mComponents ); it != std::end( mComponents ); )
    {
        auto const& comp = *it->second;
        if( comp.GetId() == id )
        {
            delete it->second;
            it = mComponents.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

ComponentHolder::ComponentHolder()
    : mComponentFactory( ComponentFactory::Get() )
{
}

ComponentHolder::~ComponentHolder()
{
    for (auto i : mComponents)
    {
        delete i.second;
    }
    mComponents.clear();
}


DefaultComponent::DefaultComponent()
    : Component()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( DefaultComponent, DefaultComponent );
