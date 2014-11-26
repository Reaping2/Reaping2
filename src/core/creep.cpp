#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_controller_component.h"
#include "core/target_player_controller_component.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"

Creep::Creep( std::string const& Name, double x, double y, Actor* player )
    : Actor( AutoId(Name) )
    , mTriedDrop( false )
{
    Opt<IPositionComponent> positionC = Get<IPositionComponent>();
    positionC->SetX(x);
    positionC->SetY(y);
    
    AddComponent( mComponentFactory(AutoId("collision_component")));
    Opt<ICollisionComponent> collisionC = Get<ICollisionComponent>();
    collisionC->SetRadius(0.1);
    collisionC->SetCollisionClass(CollisionClass::Creep);
    collisionC->SetActor( this );

    //TODO: ofc not this way, factory should give me the right component type
    std::auto_ptr<Component> hC = ComponentFactory::Get()(AutoId("health_component"));
    IHealthComponent* healthC = static_cast<IHealthComponent*>(hC.release());
    healthC->SetHP(10);
    healthC->SetActor(this);
    AddComponent(std::auto_ptr<Component>(static_cast<Component*>(healthC)));

    // ez nem innen fog jonni, de kb itt kell beallitani
    // a string ctor param lesz
    // player param nyilvan eltunik

    AddComponent(player ? mComponentFactory(AutoId("target_player_controller_component")) : mComponentFactory(AutoId("random_controller_component")));
    Get<IControllerComponent>()->SetActor(this);
    if (player)
    {
        //TODO: this is still for testing purpose
        Get<TargetPlayerControllerComponent>()->SetPlayer(player);
    }
}


void Creep::Update(double Seconds)
{
    Actor::Update( Seconds );
    if(!Get<IHealthComponent>()->IsAlive()&&!mTriedDrop)
    {
        mTriedDrop=true;
    #ifdef DEBUG
        static const size_t Mod = 3;
    #else
        static const size_t Mod = 10;
    #endif//DEBUG
        if( rand() % Mod )
        {
            return;
        }
        // TODO: this is baaaad, you need to do this with an action
        Pickup* Pu = new Pickup( rand() % 2 ? "pistol" : "plasma_gun" );
        Opt<IPositionComponent> positionC = Get<IPositionComponent>();
        Opt<IPositionComponent> puPositionC = Pu->Get<IPositionComponent>();
        puPositionC->SetX(positionC->GetX());
        puPositionC->SetY(positionC->GetY());
        Scene::Get().AddActor( Pu );
    }
}
