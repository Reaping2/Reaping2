#ifndef INCLUDED_CORE_CONTROLLER_H
#define INCLUDED_CORE_CONTROLLER_H

class Actor;
class Controller
{
public:
	Controller();
	virtual ~Controller();
	void SetActor(Actor* Obj);
	virtual void Update(double Seconds);
protected:
	virtual void AfterSetActor();
	Actor* mActor;
};

#endif//INCLUDED_CORE_CONTROLLER_H
