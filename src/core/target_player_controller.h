#ifndef INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H

class TargetPlayerController : public Controller
{
public:
	TargetPlayerController(Actor* player);
	virtual void Update(double Seconds);
	virtual void SetActor(Actor* Obj);
private:
	Actor* mPlayer;
	double mCounter;
	double mHeadingModifier;
};

#endif//INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
