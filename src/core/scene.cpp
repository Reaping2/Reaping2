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
    for( ActorList_t::iterator it = mActorHolder.mAllActors.begin(), e = mActorHolder.mAllActors.end(); it != e; ++it )
    {
        (*it)->Update( DeltaTime );
    }

    for( NewActorList_t::iterator it = mNewActors.begin(), e = mNewActors.end(); it != e; ++it )
    {
        EventServer<ActorEvent>::Get().SendEvent( ActorEvent( (*it), ActorEvent::Added ) );
        mActorHolder.mAllActors.insert( *it );
    }
    mNewActors.clear();
}

Scene::Scene()
    : mDimensions( -2, -2, 2, 2 )
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
    struct point
    {
        double x;
        double y;
    };
    const size_t NumPoints = 18;
    const point points[NumPoints] =
    {
        {-1,-0.4},                                      { 0.0, -0.4}, 
        {-1,-0.6},{-0.8,-0.6},{-0.6,-0.6},              { 0.0, -0.6},
        {-1,-0.8},                                      { 0.0, -0.8}, 
        {-1,-1},                                        { 0.0, -1},
        {-1,-1.2},                                      { 0.0, -1.2},{ 0.2, -1.2},{ 0.4, -1.2},
        {-1,-1.4},            {-0.6,-1.4},{-0.4,-1.4},  { 0.0, -1.4},
    };

    for( size_t i = 0; i < NumPoints; ++i )
    {
        std::auto_ptr<Actor> wall = ActorFactory::Get()(AutoId("wall"));
        Opt<IPositionComponent> wallPositionC = wall->Get<IPositionComponent>();
        wallPositionC->SetX( points[i].x+0.6 );
        wallPositionC->SetY( points[i].y+0.4 );

        AddActor( wall.release() );
    }

    std::auto_ptr<Actor> Pl = ActorFactory::Get()(AutoId("player"));
    
    Opt<IPositionComponent> positionC = Pl->Get<IPositionComponent>();
    positionC->SetX(0.0);
    positionC->SetY(0.0);
    
    Opt<IInventoryComponent> inventoryC = Pl->Get<IInventoryComponent>();
    inventoryC->AddItem(AutoId( "plasma_gun" ));
    inventoryC->SetSelectedWeapon(AutoId( "plasma_gun" ));


#ifdef DEBUG
    static const size_t BenchmarkCreeps = 500;
#else
    static const size_t BenchmarkCreeps = 50;
#endif
    for( size_t i = 0; i < BenchmarkCreeps; ++i )
    {
        AddTestCreep(Pl.get()
            , mDimensions.x + ( rand() % ( int )( 1000 * ( mDimensions.z - mDimensions.x ) ) ) / 1000.
            , mDimensions.y + ( rand() % ( int )( 1000 * ( mDimensions.w - mDimensions.y ) ) ) / 1000.);

    }

    mPlayerModels.clear();
    mPlayerModels.push_back( new ModelValue( Pl->Get<IHealthComponent>()->GetHP(), "hp", &mPlayerModel ) );
    Opt<IPositionComponent> objPositionC = Pl->Get<IPositionComponent>();
    mPlayerModels.push_back( new ModelValue( objPositionC->GetX(), "x", &mPlayerModel ) );
    mPlayerModels.push_back( new ModelValue( objPositionC->GetY(), "y", &mPlayerModel ) );


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

