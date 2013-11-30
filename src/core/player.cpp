#include "i_core.h"

Player::Player(std::string const& Name/*="player"*/)
	:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
	mFields[RADIUS].d=0.05;
	mFields[HP].i=100;
}
