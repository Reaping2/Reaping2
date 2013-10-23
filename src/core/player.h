#ifndef INCLUDED_CORE_PLAYER_H
#define INCLUDED_CORE_PLAYER_H

class Player : public Actor
{
public:
	Player();
	virtual void Collide(double Seconds, ActorList& Actors);
};

#endif//INCLUDED_CORE_PLAYER_H
