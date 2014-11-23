#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_controller_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/health_delete_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"

void Scene::AddActor( Actor* Object )
{
    mNewActors.push_back( Object );
    // mikor az obj torlodik, magatol kikerul a listabol
}

void Scene::Update( double DeltaTime )
{

    if( IsPaused() )
    {
        return;
    }
    mCollisionGrid.Clear();
    for( ActorList_t::iterator it = mAllActors.begin(), e = mAllActors.end(); it != e; ++it )
    {
        Actor& Obj = *it;
        Opt<IControllerComponent> objControllerC = Obj.Get<IControllerComponent>();
        if(objControllerC.IsValid())
        {
            objControllerC->Update(DeltaTime);
        }
        mCollisionGrid.AddActor( &Obj, DeltaTime );
    }
    PossibleCollisions_t const& PossibleCollisions = mCollisionGrid.GetPossibleCollisions();
    for( PossibleCollisions_t::const_iterator i = PossibleCollisions.begin(), e = PossibleCollisions.end(); i != e; ++i )
    {
        Actor& A = *( i->A1 );
        Actor& B = *( i->A2 );
        Opt<ICollisionComponent> ACollisionC = A.Get<ICollisionComponent>();
        Opt<ICollisionComponent> BcollisionC = B.Get<ICollisionComponent>();
        BOOST_ASSERT(ACollisionC.IsValid() && BcollisionC.IsValid()); //TODO: here this one should be true

        CollisionModel const& CollModel = mCollisionStore.GetCollisionModel( ACollisionC->GetCollisionClass(), BcollisionC->GetCollisionClass() );
        if( !CollModel.AreActorsColliding( A, B, DeltaTime ) )
        {
            continue;
        }
        ACollisionC->Collide( B );
        BcollisionC->Collide( A );
    }
    for( ActorList_t::iterator it = mAllActors.begin(), e = mAllActors.end(); it != e; ++it )
    {
        it->Update( DeltaTime );
    }
    size_t siz1= mAllActors.size();
    for( ActorList_t::iterator it = mAllActors.begin(), e = mAllActors.end(), n; ( n = it, it != e ? ( ++n, true ) : false ); it = n )
    {
        Opt<IHealthComponent> healthC = it->Get<IHealthComponent>();
        if(healthC.IsValid())
        {
            healthC->Update(DeltaTime);
            if (healthC->NeedDelete())
            {
                mAllActors.erase(it);
            }
        }
    }
    size_t siz2= mAllActors.size();
//     while( !mNewActors.empty() )
//     {
//         ActorList_t::auto_type tp=mNewActors.pop_front();
//         mAllActors.insert( mAllActors.end(), tp.get() );
        mAllActors.transfer( mAllActors.end(),mNewActors.begin(),mNewActors.end(),mNewActors);
//     }
}

Scene::Scene()
    : mDimensions( -2, -2, 2, 2 )
    , mCollisionStore( CollisionStore::Get() )
    , mTypeId( 0 )
    , mPaused( true )
    , mSceneModel( "scene", &RootModel::Get() )
    , mLoadModel( StringFunc( this, &Scene::Load ), "load", &mSceneModel )
    , mPauseModel( VoidFunc( this, &Scene::Pause ), "pause", &mSceneModel )
    , mResumeModel( VoidFunc( this, &Scene::Resume ), "resume", &mSceneModel )
{
    mCollisionGrid.Build( mDimensions, 0.4f );
}

glm::vec4 const& Scene::GetDimensions()
{
    return mDimensions;
}

Scene::~Scene()
{
//     while( !mNewActors.empty() )
//     {
//         ActorList_t::auto_type tp=mNewActors.pop_front();
//         mAllActors.insert( mAllActors.end(), tp.get() );
//     }
    mAllActors.transfer( mAllActors.end(),mNewActors.begin(),mNewActors.end(),mNewActors);

    mAllActors.clear();
//     while( !mAllActors.empty() )
//     {
//         delete &mAllActors.front();
//     }
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
//     while( !mNewActors.empty() )
//     {
//         ActorList_t::auto_type tp=mNewActors.pop_front();
//         mAllActors.insert( mAllActors.end(), tp.get() );
//     }
    mAllActors.transfer( mAllActors.end(),mNewActors.begin(),mNewActors.end(),mNewActors);

    mAllActors.clear();
//     while( !mAllActors.empty() )
//     {
//         delete &mAllActors.front();
//     }

    SetType( "grass" );
    struct point
    {
        double x;
        double y;
    };
    const size_t NumPoints = 5;
    const point points[NumPoints] =
    {
        { -1, -1}, { -1, -0.8}, { -1, -0.6}, { -0.8, -0.6}, { -0.6, -0.6},
    };

    for( size_t i = 0; i < NumPoints; ++i )
    {
        //TODO: one should not just "new" something. ofc testing reasons
        Wall* wall = new Wall( "wall" );
        Opt<IPositionComponent> wallPositionC = wall->Get<IPositionComponent>();
        wallPositionC->SetX( points[i].x );
        wallPositionC->SetY( points[i].y );

        AddActor( wall );
    }

    Player* Pl = new Player();
    Opt<IPositionComponent> positionC = Pl->Get<IPositionComponent>();
    positionC->SetX(0.0);
    positionC->SetY(0.0);
    
    Pl->AddComponent( ComponentFactory::Get()(AutoId("player_controller_component")) );
    Pl->Get<IControllerComponent>()->SetActor(Pl);
    Pl->AddComponent( ComponentFactory::Get()(AutoId("inventory_component")) );
    Opt<IInventoryComponent> inventoryC = Pl->Get<IInventoryComponent>();
    inventoryC->SetActor(Pl);
    inventoryC->AddItem(AutoId( "pistol" ));
    Pl->AddAction( AutoId( "idle_action" ) );

    AddActor( Pl );

#ifdef DEBUG
    static const size_t BenchmarkCreeps = 500;
#else
    static const size_t BenchmarkCreeps = 500;
#endif
    for( size_t i = 0; i < BenchmarkCreeps; ++i )
    {
        Creep* Obj = new Creep( rand() % 2 ? "pok1" : "pok2",
                                mDimensions.x + ( rand() % ( int )( 1000 * ( mDimensions.z - mDimensions.x ) ) ) / 1000.,
                                mDimensions.y + ( rand() % ( int )( 1000 * ( mDimensions.w - mDimensions.y ) ) ) / 1000.,
                                rand() % 2 ? Pl : ( Actor* )NULL );
        AddActor( Obj );
    }
}

