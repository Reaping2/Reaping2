#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/move_component.h"
#include "core/i_controller_component.h"
#include "platform/auto_id.h"

using platform::AutoId;

void Actor::Update( double Seconds )
{
    for( ActionList_t::iterator i = mActions.begin(), e = mActions.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        i->second->Update( Seconds );
    }
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(); i != e; ++i )
    {
        i->Update( Seconds );
    }
}

void Actor::Collide( Actor& Other )
{
}

Actor::Actor( std::string const& Name )
    : AutoId( Name )
    , ComponentHolder ()
    , mActionFactory( ActionRepo::Get() )
    , mItemFactory( ItemRepo::Get() )
{
    memset( &mFields, 0, NUM_FIELDS * sizeof( Field_t ) );
    mFields[COLLISION_CLASS].i = CollisionClass::Player;
    mFields[TYPE_ID].i = AutoId::mId; //TODO: ugly as fck. Component has an mId that is not used yet - or ever - later development will tell
    mFields[RADIUS].d = 3.0;
    mFields[TIME_OF_DEATH].d = 0.0;
    static int32_t NextGuid = 0;
    mFields[GUID].i = ++NextGuid;
    mFields[COOLDOWN_REDUCTION].d = 1.0;
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

Actor::ItemList_t const& Actor::GetItems()const
{
    return mItems;
}

void Actor::AddItem( int32_t Id )
{
    std::auto_ptr<Item> a = mItemFactory( Id );
    a->SetActor( this );
    mItems.push_back( a );
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

bool Actor::IsAlive()const
{
    return mFields[HP].i > HP_DEAD;
}

void Actor::UpdateLifetime()
{
    if( IsAlive() )
    {
        return;
    }
    AddAction( AutoId( "death" ) );
    mFields[COLLISION_CLASS].i = CollisionClass::No_Collision;
    if( mFields[TIME_OF_DEATH].d <= 0.0 )
    {
        OnDeath();
        mFields[TIME_OF_DEATH].d = glfwGetTime();
    }
}

Actor::~Actor()
{
}

void Actor::TakeDamage( int32_t Damage )
{
    if( Damage && IsAlive() )
    {
        //TODO: ofc this takeDamage thing will be moved to DamageableComponent.
        Opt<IPositionComponent> positionC = Get<IPositionComponent>();
        EventServer<DamageTakenEvent>::Get().SendEvent( DamageTakenEvent( positionC->GetX(), positionC->GetY() ) );
        mFields[HP].i -= Damage;
    }
}


bool Actor::HasAction( int32_t Id ) const
{
    return mActions.find( Id ) != mActions.end();
}

void Actor::DropItemType( Item::ItemType Type )
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        if( i->GetType() == Type )
        {
            mItems.erase( i );
        } 
    }
}


