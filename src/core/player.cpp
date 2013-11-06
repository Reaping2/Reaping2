#include "i_core.h"

Player::Player(std::string const& Name/*="player"*/)
	:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
}


void Player::Collide( double Seconds, ActorList& Actors )
{

}

