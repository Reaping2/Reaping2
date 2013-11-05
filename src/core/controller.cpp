#include "i_core.h"

Controller::Controller() : mActor(NULL)
{

}

void Controller::SetActor(Actor* Obj)
{
	mActor=Obj;
}

void Controller::Update( double Seconds )
{

}

Controller::~Controller()
{

}

