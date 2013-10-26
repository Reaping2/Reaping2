#include "i_core.h"

Controller::Controller() : mActor(NULL)
{

}

void Controller::SetActor(Actor* Obj)
{
	mActor=Obj;

	AfterSetActor();
}

void Controller::Update( double Seconds )
{

}

void Controller::AfterSetActor()
{

}

Controller::~Controller()
{

}

