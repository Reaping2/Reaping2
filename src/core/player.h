#ifndef INCLUDED_CORE_PLAYER_H
#define INCLUDED_CORE_PLAYER_H

class Player : public Actor
{
public:
	Player(std::string const& Name="player");
	virtual void ClipScene();
};

struct PlayerMoveEvent : public Event
{
	float X;
	float Y;
	PlayerMoveEvent(float x, float y):X(x),Y(y){}
};

#endif//INCLUDED_CORE_PLAYER_H
