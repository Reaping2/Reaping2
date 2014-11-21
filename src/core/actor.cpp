#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/move_component.h"
#include "core/i_controller_component.h"
#include "platform/auto_id.h"
#include "inventory_component.h"

using platform::AutoId;

void Actor::Update( double Seconds )
{
    for( ActionList_t::iterator i = mActions.begin(), e = mActions.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        i->second->Update( Seconds );
    }
    //TODO: maybe every component should have an update, and it should be called here
    //they might have priority order
    Opt<IInventoryComponent> inventoryC = Get<IInventoryComponent>();
    if(inventoryC.IsValid())
    {
        inventoryC->Update( Seconds );
    }
}

void Actor::Collide( Actor& Other )
{
}

Actor::Actor( std::string const& Name )
    : AutoId( Name )
    , ComponentHolder ()
    , mActionFactory( ActionRepo::Get() )
{
    memset( &mFields, 0, NUM_FIELDS * sizeof( Field_t ) );
    mFields[COLLISION_CLASS].i = CollisionClass::Player;
    mFields[RADIUS].d = 3.0;
    static int32_t NextGuid = 0;
    mFields[GUID].i = ++NextGuid;
    AddAction( AutoId( "default_action" ) );
    AddComponent( mComponentFactory(AutoId("position_component")) );
    AddComponent( mComponentFactory(AutoId("move_component")) );
}

bool Actor::CanAddAction( int32_t Id )const
{
    for( ActionList_t::const_iterator i = mActions.begin(), e = mActions.end(); i != e; ++i )
        if( i->second->Blocks( Id ) )
        {
            return false;
        }
    return true;
}

void Actor::AddAction( int32_t Id )
{
    if( !CanAddAction( Id ) )
    {
        return;
    }
    ActionList_t::iterator i = mActions.find( Id );
    if( i == mActions.end() )
    {
        std::auto_ptr<Action> a = mActionFactory( Id );
        a->SetActor( this );
        if ( a->Activate() )
        {
            mActions.insert( Id, a );
        }
    }
    else
    {
        Action* a = i->second;
        a->Activate();
    }
}

//TODO: there will be only one
void Actor::AddAction( std::auto_ptr<Action> Act )
{
    if( !CanAddAction( Act->GetId() ) )
    {
        return;
    }
    ActionList_t::iterator i = mActions.find( Act->GetId() );
    if( i == mActions.end() )
    {
        Act->SetActor( this );
        if ( Act->Activate() )
        {
            mActions.insert( Act->GetId(), Act );
        }
    }
    else
    {
        //TODO: this will be changed
        Action* a = i->second;
        a->Activate();
    }
}


void Actor::DropAction( int32_t Id )
{
    ActionList_t::iterator i = mActions.find( Id );
    if( i == mActions.end() )
    {
        return;
    }
    i->second->Deactivate();
    mActions.erase( i );
}

void Actor::ClipScene()
{
    glm::vec4 AllowedDimensions = Scene::Get().GetDimensions();
    float Radius = ( float )GetRadius();
    AllowedDimensions.x += Radius;
    AllowedDimensions.y += Radius;
    AllowedDimensions.z -= Radius;
    AllowedDimensions.w -= Radius;
    Opt<IPositionComponent> positionC = Get<IPositionComponent>();
    if( positionC->GetX() < AllowedDimensions.x )
    {
        positionC->SetX( AllowedDimensions.x );
    }
    else if( positionC->GetX() > AllowedDimensions.z )
    {
        positionC->SetX( AllowedDimensions.z );
    }
    if( positionC->GetY() < AllowedDimensions.y )
    {
        positionC->SetY( AllowedDimensions.y );
    }
    else if( positionC->GetY() > AllowedDimensions.w )
    {
        positionC->SetY( AllowedDimensions.w );
    }
}

Actor::~Actor()
{
}

bool Actor::HasAction( int32_t Id ) const
{
    return mActions.find( Id ) != mActions.end();
}

