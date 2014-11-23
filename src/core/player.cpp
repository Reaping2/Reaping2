#include "i_core.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"
#include "core/i_renderable_component.h"

Player::Player( std::string const& Name/*="player"*/ )
    : Actor( Name )
{

    AddComponent( mComponentFactory(AutoId("collision_component")));
    Opt<ICollisionComponent> collisionC = Get<ICollisionComponent>();
    collisionC->SetRadius(0.05);
    collisionC->SetCollisionClass(CollisionClass::Player);
    collisionC->SetActor( this );

    //TODO: ofc not this way, factory should give me the right component type
    std::auto_ptr<Component> hC = ComponentFactory::Get()(AutoId("health_component"));
    IHealthComponent* healthC = static_cast<IHealthComponent*>(hC.release());
    healthC->SetHP(100);
    healthC->SetActor(this);
    AddComponent(std::auto_ptr<Component>(static_cast<Component*>(healthC)));
    Opt<IRenderableComponent> renderableC = Get<IRenderableComponent>();
    renderableC->SetLayer(IRenderableComponent::Players);
}
