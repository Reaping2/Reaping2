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
#include "actor_event.h"
#include "player_controller_component.h"
#include "program_state.h"
using core::ProgramState;

int32_t ActorHolder::ActorDefaultOrderer::operator ()(const Opt<Actor>& Obj)const
{
    return Obj->GetGUID();
}

bool ActorHolder::IsRenderable::operator ()(const Opt<Actor>& Obj)const
{
    return Obj->Get<IRenderableComponent>().IsValid();
}

int32_t ActorHolder::GetLayer::operator ()(const Opt<Actor>& Obj)const
{
    return Obj->Get<IRenderableComponent>().IsValid()?Obj->Get<IRenderableComponent>()->GetLayer():0;
}

int32_t ActorHolder::GetZOrder::operator ()(const Opt<Actor>& Obj)const
{
    return Obj->Get<IRenderableComponent>().IsValid()?Obj->Get<IRenderableComponent>()->GetZOrder():0;
}

void Scene::AddActor( Actor* Object )
{
    mNewActors.push_back( Opt<Actor>(Object) );
}

void Scene::Update( double DeltaTime )
{

    if( IsPaused() )
    {
        return;
    }

    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Added ) );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();
}

Scene::Scene()
    : mDimensions( -2000, -2000, 2000, 2000 )
    , mTypeId( 0 )
    , mPaused( true )
    , mSceneModel( "scene", &RootModel::Get() )
    , mLoadModel( StringFunc( this, &Scene::Load ), "load", &mSceneModel )
    , mPauseModel( VoidFunc( this, &Scene::Pause ), "pause", &mSceneModel )
    , mResumeModel( VoidFunc( this, &Scene::Resume ), "resume", &mSceneModel )
    , mPlayerModel( "player", &RootModel::Get() )
{
}

glm::vec4 const& Scene::GetDimensions()
{
    return mDimensions;
}

Scene::~Scene()
{
    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Added ) );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();

    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it!=e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Removed ) );
        
        delete &**it;
    }
    mActorHolder.mAllActors.clear();
    mPlayerModels.clear();
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
    mPaused = false;
    
    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Added ) );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();

    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it!=e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Removed ) );
        delete &**it;
    }
    mActorHolder.mAllActors.clear();
    SetType( "grass" );
    if (ProgramState::Get().mMode==ProgramState::Client)
    {
        return;
    }

    struct point
    {
        double x;
        double y;
    };
    const size_t NumPoints = 18;
    const point points[NumPoints] =
    {
        {-1000,-400},                                      { 0.0, -400}, 
        {-1000,-600},{-800,-600},{-600,-600},              { 0.0, -600},
        {-1000,-800},                                      { 0.0, -800}, 
        {-1000,-1000},                                        { 0.0, -1000},
        {-1000,-1200},                                      { 0.0, -1200},{ 200, -1200},{ 400, -1200},
        {-1000,-1400},            {-600,-1400},{-400,-1400},  { 0.0, -1400},
    };

    for( size_t i = 0; i < NumPoints; ++i )
    {
        std::auto_ptr<Actor> wall = ActorFactory::Get()(AutoId("wall"));
        Opt<IPositionComponent> wallPositionC = wall->Get<IPositionComponent>();
        wallPositionC->SetX( points[i].x+600 );
        wallPositionC->SetY( points[i].y+400 );

        AddActor( wall.release() );
    }

    for( int32_t i = -5; i < 5; ++i )
    {
        std::auto_ptr<Actor> wall = ActorFactory::Get()(AutoId("wall"));
        Opt<IPositionComponent> wallPositionC = wall->Get<IPositionComponent>();
        wallPositionC->SetX( i*200 );
        wallPositionC->SetY( std::abs(i)*-100+1200 );

        AddActor( wall.release() );
        wall = ActorFactory::Get()(AutoId("wall"));
        wallPositionC = wall->Get<IPositionComponent>();
        wallPositionC->SetX( (i+2)*100 );
        wallPositionC->SetY( (i-5)*50-1400 );

        AddActor( wall.release() );
    }

    if (ProgramState::Get().mMode==ProgramState::Server)
    {
        return;
    }
    std::auto_ptr<Actor> Pl = ActorFactory::Get()(AutoId("player"));
    
    Opt<IPositionComponent> positionC = Pl->Get<IPositionComponent>();
    positionC->SetX(0.0);
    positionC->SetY(0.0);
    
    Opt<IInventoryComponent> inventoryC = Pl->Get<IInventoryComponent>();
//    inventoryC->AddItem(AutoId( "plasma_gun" ));
    inventoryC->SetSelectedWeapon(AutoId( "plasma_gun" ));

    Pl->Get<PlayerControllerComponent>()->SetEnabled(true);
    Pl->Get<PlayerControllerComponent>()->mActive=true;

#ifdef DEBUG
    static const size_t BenchmarkCreeps = 500;
#else
    static const size_t BenchmarkCreeps = 500;
#endif
    for( size_t i = 0; i < BenchmarkCreeps; ++i )
    {
        AddTestCreep(Pl.get()
            , mDimensions.x + ( rand() % ( int )( ( mDimensions.z - mDimensions.x ) ) ) 
            , mDimensions.y + ( rand() % ( int )( ( mDimensions.w - mDimensions.y ) ) ) );

    }

    SetPlayerModels(Opt<Actor>(Pl.get()));
    AddActor( Pl.release() );

}

void Scene::AddTestCreep(Actor* Pl, double X, double Y)
{
    std::auto_ptr<Actor> Obj;
    switch(rand()%4)
    {
    case 0:
        Obj=ActorFactory::Get()(AutoId("spider1"));
        break;
    case 1:
        Obj=ActorFactory::Get()(AutoId("spider2"));
        break;
    case 2:
        Obj=ActorFactory::Get()(AutoId("spider1target"));
        Obj->Get<TargetPlayerControllerComponent>()->SetPlayer(Pl);
        break;
    case 3:
        Obj=ActorFactory::Get()(AutoId("spider2target"));
        Obj->Get<TargetPlayerControllerComponent>()->SetPlayer(Pl);
        break;
    }
    Obj->Get<IPositionComponent>()->SetX(X);
    Obj->Get<IPositionComponent>()->SetY(Y);
    AddActor( Obj.release() );
}

void Scene::RemoveActor(ActorList_t::iterator it)
{
    EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Removed ) );

    delete (*it).Get();
    mActorHolder.mAllActors.erase(it);
}

void Scene::RemoveActor(Actor* Object)
{
    ActorList_t::iterator it = mActorHolder.mAllActors.find(Object->GetGUID());
    BOOST_ASSERT(it!=mActorHolder.mAllActors.end());
    RemoveActor(it);
}

Opt<Actor> Scene::GetActor(int32_t guid)
{
    ActorList_t::iterator it = mActorHolder.mAllActors.find(guid);
    if (it!=mActorHolder.mAllActors.end())
    {
        return (*it);
    }
    for(NewActorList_t::iterator i=mNewActors.begin(),e=mNewActors.end();i!=e;++i)
    {
        if ((*i)->GetGUID()==guid)
        {
            return (*i);
        }
    }
    return Opt<Actor>(NULL);
}

void Scene::SetPlayerModels(Opt<Actor> actor)
{
    mPlayerModels.clear();
    mPlayerModels.push_back( new ModelValue( actor->Get<IHealthComponent>()->GetHP(), "hp", &mPlayerModel ) );
    Opt<IPositionComponent> objPositionC = actor->Get<IPositionComponent>();
    mPlayerModels.push_back( new ModelValue( objPositionC->GetX(), "x", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( objPositionC->GetY(), "y", &mPlayerModel ) );
}

