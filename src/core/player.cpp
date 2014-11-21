#include "i_core.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"

Player::Player( std::string const& Name/*="player"*/ )
    : Actor( Name )
{
    mFields[COLLISION_CLASS].i = CollisionClass::Player;
    mFields[RADIUS].d = 0.05;
    //TODO: ofc not this way, factory should give me the right component type
    std::auto_ptr<Component> hC = ComponentFactory::Get()(AutoId("health_component"));
    IHealthComponent* healthC = static_cast<IHealthComponent*>(hC.release());
    healthC->SetHP(100);
    healthC->SetActor(this);
    AddComponent(std::auto_ptr<Component>(static_cast<Component*>(healthC)));
}
