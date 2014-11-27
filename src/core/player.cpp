#include "i_core.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"
#include "core/i_renderable_component.h"

Player::Player( std::string const& Name/*="player"*/ )
    : Actor( AutoId(Name) )
{


}
