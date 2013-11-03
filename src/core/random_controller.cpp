#include "i_core.h"

RandomController::RandomController():Controller()
	,mCounter(2)
	,mHeadingModifier(0)
{

}

void RandomController::AfterSetActor()
{
	if(!mActor)return;
	mActor->SetHeading(rand()%180);
	ActionHolder::Get().AddAction(*mActor,ActionHolder::MOVE);
}

void RandomController::Update( double Seconds )
{
	if(!mActor)return;
	mCounter+=Seconds;
	if (mCounter>2)
	{
		mCounter=0;
		mHeadingModifier=(rand()%10-5)*.33;

		mActor->SetSpeed((rand()%10)*.03);
	}
	mActor->SetHeading(mActor->GetHeading()+Seconds*mHeadingModifier);
	mActor->SetOrientation(mActor->GetHeading());
}

