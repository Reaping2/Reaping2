#include "i_core.h"

Player::Player()
{
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
	mFields[TYPE_ID].i=2;
}

void Player::Collide( double Seconds, ActorList& Actors )
{

}

