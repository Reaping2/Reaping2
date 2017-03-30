#include "platform/i_platform.h"
#include "core/scene.h"
#include "core/i_position_component.h"
#include "core/i_controller_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"
#include "core/i_renderable_component.h"
#include "core/actor_factory.h"
#include "core/renderable_layer.h"
#include "core/i_remove_on_death_component.h"
#include "core/target_player_controller_component.h"
#include "core/gamemode_selected_event.h"
#include "actor_event.h"
#include "player_controller_component.h"
#include "program_state.h"
#include "ui/ui.h"
#include "ui/widget_factory.h"
#include "ui/text_widget.h"
#include "i_move_component.h"
#include "buffs/i_buff_holder_component.h"
#include "buffs/buff_factory.h"
#include "buffs/max_health_buff.h"
#include "engine/buffs_engine/max_health_buff_sub_system.h"
#include "engine/soldier_spawn_system.h"
#include "map_load_event.h"
#include "map_start_event.h"
#include "map/map_repo.h"
#include "engine/system_suppressor.h"
#include "engine/engine.h"
#include "render/renderer.h"

using core::ProgramState;

int32_t ActorHolder::ActorDefaultOrderer::operator ()( const Opt<Actor>& Obj )const
{
    return Obj->GetGUID();
}

bool ActorHolder::IsRenderable::operator ()( const Opt<Actor>& Obj )const
{
    return Obj->Get<IRenderableComponent>().IsValid();
}

int32_t ActorHolder::GetLayer::operator ()( const Opt<Actor>& Obj )const
{
    return Obj->Get<IRenderableComponent>().IsValid() ? Obj->Get<IRenderableComponent>()->GetLayerPriority() : 0;
}

int32_t ActorHolder::GetZOrder::operator ()( const Opt<Actor>& Obj )const
{
    return Obj->Get<IRenderableComponent>().IsValid() ? Obj->Get<IRenderableComponent>()->GetZOrder() : 0;
}

int32_t ActorHolder::GetCollisionClass::operator()( const Opt<Actor>& Obj ) const
{
    return Obj->Get<ICollisionComponent>().IsValid() ? int32_t( Obj->Get<ICollisionComponent>()->GetCollisionClass() ) : 0;
}


void Scene::AddActor( Actor* Object )
{
    if( NULL == Object )
    {
        L1( "Prevent adding NULL actor\n" );
        return;
    }
    L2( "AddActor called (GUID:%d)\n", Object->GetGUID() );
    mNewActors.push_back( Opt<Actor>( Object ) );
}

void Scene::Update( double DeltaTime )
{
    if (mSendMapStarted)
    {
        mSendMapStarted = false;
        EventServer<core::MapStartEvent>::Get().SendEvent( core::MapStartEvent( core::MapStartEvent::Started ) );
    }
    if( IsPaused() )
    {
        return;
    }

    Opt<Actor> player( GetActor( mProgramState.mControlledActorGUID ) );
    if ( player.IsValid() )
    {
        Opt<IHealthComponent> healthC = player->Get<IHealthComponent>();
        mMaxHP = healthC->GetMaxHP().Get();
    }
    //TODO: testing
    if ( false && ProgramState::Get().mMode != ProgramState::Client
         && RandomGenerator::global()() % 600 == 1
         && mActorHolder.mAllActors.size() < 1500 )
    {
        AddTestCreep( mDimensions.x + ( RandomGenerator::global()() % ( int )( ( mDimensions.z - mDimensions.x ) ) )
                      , mDimensions.y + ( RandomGenerator::global()() % ( int )( ( mDimensions.w - mDimensions.y ) ) ) );
    }
    //testing end

    InsertNewActors();
}

Scene::Scene()
    : mDimensions( -16000, -16000, 16000, 16000 )
    , mTypeId( 0 )
    , mPaused( true )
    , mSceneModel( "scene", &RootModel::Get() )
    , mLoadModel( StringFunc( this, &Scene::Load ), "load", &mSceneModel )
    , mPauseModel( VoidFunc( this, &Scene::Pause ), "pause", &mSceneModel )
    , mResumeModel( VoidFunc( this, &Scene::Resume ), "resume", &mSceneModel )
    , mPlayerModel( "player", &RootModel::Get() )
    , mGameModeModel( "gamemode", &RootModel::Get() )
    , mSelectGameModeModel( StringFunc( this, &Scene::SelectGameMode ), "select", &mGameModeModel )
    , mMaxHP( 0 )
    , mProgramState( core::ProgramState::Get() )
{
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &Scene::OnMapStart, this, _1 ) );
}

glm::vec4 const& Scene::GetDimensions()
{
    return mDimensions;
}

Scene::~Scene()
{
    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        //EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Added ) );
        L2( "new actor inserted at destruct (GUID:%d)\n", ( *it )->GetGUID() );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();

    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it != e; ++it )
    {
        //EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Removed ) );
        L2( "actor delete at destruct (GUID:%d)\n", ( *it )->GetGUID() );
        delete ( *it ).Get();
    }
    mActorHolder.mAllActors.clear();
    mPlayerModels.clear();
    mActorMap.clear();
}

void Scene::SetType( std::string const& Type )
{
    mTypeId = AutoId( Type );
}

int32_t Scene::GetTypeId() const
{
    return mTypeId;
}

void Scene::Load( std::string const& Level )
{
    L2( "Scene load started %s\n", Level.c_str() );
    mProgramState.mGameState = core::ProgramState::NotRunning;
    EventServer<core::MapLoadEvent>::Get().SendEvent( core::MapLoadEvent( map::MapRepo::mMapDir+"/" + Level ) );
    mPaused = false;

    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Added ) );
        L2( "new actor inserted at Load (GUID:%d)\n", ( *it )->GetGUID() );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();

    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Removed ) );
        L2( "actor delete at Load (GUID:%d)\n", ( *it )->GetGUID() );
        delete ( *it ).Get();
    }
    mActorHolder.mAllActors.clear();
    mActorMap.clear();
    mSendMapStarted = true;
    L2( "Scene load ended %s\n", Level.c_str() );
}

void Scene::AddTestCreep( double X, double Y )
{
    std::auto_ptr<Actor> Obj;
    switch( RandomGenerator::global()() % 4 )
    {
    case 0:
        Obj = ActorFactory::Get()( AutoId( "spider1" ) );
        break;
    case 1:
        Obj = ActorFactory::Get()( AutoId( "spider2" ) );
        break;
    case 2:
        Obj = ActorFactory::Get()( AutoId( "spider1target" ) );
        break;
    case 3:
        Obj = ActorFactory::Get()( AutoId( "spider2target" ) );
        break;
    }
    Obj->Get<IPositionComponent>()->SetX( X );
    Obj->Get<IPositionComponent>()->SetY( Y );
    AddActor( Obj.release() );
}

void Scene::RemoveActor( ActorList_t::iterator it )
{
    L2( "removeActor it (GUID:%d)\n", ( *it )->GetGUID() );
    RemoveFromActorMap( it->Get() );
    EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Removed ) );
    mRemovedActors.emplace_back( it->Get() );
    mActorHolder.mAllActors.erase( it );
}

void Scene::RemoveActor( int32_t guid )
{
    ActorList_t::iterator it = mActorHolder.mAllActors.find( guid );
    if ( it != mActorHolder.mAllActors.end() )
    {
        L2( "removeActor from existing actors (GUID:%d)\n", ( *it )->GetGUID() );
        RemoveActor( it );
        return;
    }
    else
    {
        for( NewActorList_t::iterator i = mNewActors.begin(), e = mNewActors.end(); i != e; ++i )
        {
            if ( ( *i )->GetGUID() == guid )
            {
                L2( "removeActor from new actors (GUID:%d)\n", ( *i )->GetGUID() );
                mRemovedActors.emplace_back( i->Get() );
                mNewActors.erase( i );
                return;
            }
        }
    }
    L1( "removeActor is called on an actor that does not exist (no good at all) (GUID:%d)\n", guid );
}

Opt<Actor> Scene::GetActor( int32_t guid )
{
    ActorList_t::iterator it = mActorHolder.mAllActors.find( guid );
    if ( it != mActorHolder.mAllActors.end() )
    {
        return ( *it );
    }
    for( NewActorList_t::iterator i = mNewActors.begin(), e = mNewActors.end(); i != e; ++i )
    {
        if ( ( *i )->GetGUID() == guid )
        {
            return ( *i );
        }
    }
    return Opt<Actor>( NULL );
}

namespace {
int32_t GetHP( Actor* a )
{
    Opt<IHealthComponent> healthC = a->Get<IHealthComponent>();
    return healthC->GetHP();
}
double GetX( Actor* a )
{
    Opt<IPositionComponent> positionC = a->Get<IPositionComponent>();
    return positionC->GetX();
}
double GetY( Actor* a )
{
    Opt<IPositionComponent> positionC = a->Get<IPositionComponent>();
    return positionC->GetY();
}
int32_t GetWeaponId( Actor* a )
{
    Opt<IInventoryComponent> inventoryC = a->Get<IInventoryComponent>();
    if( !inventoryC.IsValid() )
    {
        return 0;
    }
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    return weapon.IsValid() ? weapon->GetId() : 0;
}
int32_t GetSumBullets( Actor* a )
{
    Opt<IInventoryComponent> inventoryC = a->Get<IInventoryComponent>();
    if (!inventoryC.IsValid())
    {
        return 0;
    }
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    return weapon.IsValid() ? weapon->GetSumBullets().Get() : 0;
}
int32_t GetSumBulletsMax( Actor* a )
{
    Opt<IInventoryComponent> inventoryC = a->Get<IInventoryComponent>();
    if (!inventoryC.IsValid())
    {
        return 0;
    }
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    return weapon.IsValid() ? weapon->GetSumBullets().GetMax() : 0.0;
}
int32_t GetDarkMatters( Actor* a )
{
    Opt<IInventoryComponent> inventoryC = a->Get<IInventoryComponent>();
    return inventoryC.IsValid()?inventoryC->GetDarkMatters():0;
}
double GetSpecialId( Actor* a )
{
    Opt<IInventoryComponent> inventoryC = a->Get<IInventoryComponent>();
    if( !inventoryC.IsValid() )
    {
        return 0;
    }
    Opt<NormalItem> item = inventoryC->GetSelectedItem( ItemType::Normal );
    return item.IsValid() ? item->GetId() : 0;
}
std::vector<int32_t> GetBuffs( Actor* a )
{
    std::vector<int32_t> rv;
    Opt<IBuffHolderComponent> buffHolderC = a->Get<IBuffHolderComponent>();
    if( !buffHolderC.IsValid() )
    {
        return rv;
    }
    BuffList_t::nth_index<1>::type const& buffList = buffHolderC->GetBuffList().get<1>();
    for( BuffList_t::nth_index_const_iterator<1>::type i = buffList.begin(), e = buffList.end(); i != e; ++i )
    {
        Buff const& b = **i;
        if( b.IsAutoRemove() )
        {
            rv.push_back( b.GetType() );
        }
    }
    return rv;
}
}

void Scene::SetPlayerModels( Opt<Actor> actor )
{
    mPlayerModels.clear();
    if ( !actor.IsValid() )
    {
        return;
    }
    mPlayerModels.push_back( new ModelValue( ( ModelValue::get_int_t ) boost::lambda::bind( &GetHP, actor.Get() ), "hp", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( ( ModelValue::get_double_t ) boost::lambda::bind( &GetX, actor.Get() ), "x", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( ( ModelValue::get_double_t ) boost::lambda::bind( &GetY, actor.Get() ), "y", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( ( ModelValue::get_int_t ) boost::lambda::bind( &GetWeaponId, actor.Get() ), "weapon", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( ( ModelValue::get_int_t ) boost::lambda::bind( &GetSpecialId, actor.Get() ), "special", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( ( ModelValue::get_int_vec_t ) boost::lambda::bind( &GetBuffs, actor.Get() ), "buffs", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( RefTo( mMaxHP ), "max_hp", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( (ModelValue::get_int_t) boost::lambda::bind( &GetSumBullets, actor.Get() ), "sum_bullets", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( (ModelValue::get_int_t) boost::lambda::bind( &GetSumBulletsMax, actor.Get() ), "sum_bullets_max", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( (ModelValue::get_int_t) boost::lambda::bind( &GetDarkMatters, actor.Get() ), "dark_matters", &mPlayerModel ) );
}


void Scene::InsertNewActors()
{
    // do the deletion in cycles so potential dangling Actor*s don't wrak havoc
    mRemovedActors.clear();
    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Added ) );
        L2( "new actor inserted at update (GUID:%d)\n", ( *it )->GetGUID() );
        mActorHolder.mAllActors.insert( *it );
        AddToActorMap( it->Get() );
    }
    mNewActors.clear();
}

ActorList_t const& Scene::GetActors() const
{
    return mActorHolder.mAllActors;
}

ActorList_t& Scene::GetActors()
{
    return mActorHolder.mAllActors;
}

void Scene::SetActors( ActorList_t& actors, bool withAddActorEvents/*=true*/ )
{
    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();

    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it != e; ++it )
    {
        delete ( *it ).Get();
    }
    mActorHolder.mAllActors.clear();
    mActorMap.clear();
    mActorHolder.mAllActors = actors;
    for (auto actor : mActorHolder.mAllActors)
    {
        AddToActorMap( actor.Get() );
    }
    if ( withAddActorEvents )
    {
        for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it != e; ++it )
        {
            EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Added ) );
        }
    }
}

void Scene::ClearActors( bool withEvents/*=true*/ )
{
    mPaused = false;

    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        if ( withEvents )
        {
            EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Added ) );
        }
        L2( "new actor inserted at Clear (GUID:%d)\n", ( *it )->GetGUID() );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();
    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it != e; ++it )
    {
        if ( withEvents )
        {
            EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( *it ), ActorEvent::Removed ) );
        }
        L2( "actor deleted at Clear (GUID:%d)\n", ( *it )->GetGUID() );
        delete ( *it ).Get();
    }
    mActorHolder.mAllActors.clear();
    mActorMap.clear();
}

void Scene::SelectGameMode( std::string const& GameMode )
{
    core::GameModes::Type mode = core::GameModes::Get()( AutoId(GameMode) );
    mProgramState.mGameMode = mode;
    core::GamemodeSelectedEvent event;
    event.mGameMode = mode;
    EventServer<core::GamemodeSelectedEvent>::Get().SendEvent( event );
}

void Scene::AddValidator( int32_t Id, Validator_t validator )
{
    mValidatorMap.emplace( Id, validator );
}

void Scene::AddToActorMap( Actor* actor )
{
    for (auto&& validator : mValidatorMap)
    {
        if (validator.second( *actor ))
        {
            mActorMap[validator.first].push_back( actor );
        }
    }
}

void Scene::RemoveFromActorMap( Actor* actor )
{
    for (auto& actors : mActorMap)
    {
        auto& vec = actors.second;
        vec.erase( std::remove( vec.begin(), vec.end(), actor ), vec.end() );

   }
}

Scene::Actors_t& Scene::GetActorsFromMap( int32_t Id )
{
    auto found = mActorMap.find( Id );
    static Actors_t emptyActors;
    return ( found != mActorMap.end() ? found->second : emptyActors );
}

void Scene::OnMapStart( core::MapStartEvent const& Evt )
{
    L2( "Scene Map start\n" );
    if (Evt.mState == core::MapStartEvent::Ready)
    {
        L2( "Scene Map start READY\n" );
        mProgramState.mGameState = core::ProgramState::Running;
    }
}

