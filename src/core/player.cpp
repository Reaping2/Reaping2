#include "i_core.h"

Player::Player(std::string const& Name/*="player"*/)
	:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
	mFields[RADIUS].d=0.05;
}

void Player::ClipScene()
{
	Actor::ClipScene();
	// todo: send only if it's the controlled player (when multiplayer is available)
	// it's not okay to send this from the controller
	EventServer<PlayerMoveEvent>::Get().SendEvent(PlayerMoveEvent((float)GetX(),(float)GetY()));
}

