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
}

void RandomController::Update( double Seconds )
{
	if(!mActor)return;
	mCounter+=Seconds;
	if (mCounter>2)
	{
		mCounter=0;
		mHeadingModifier=(rand()%10-5)*0.1;

		mActor->SetSpeed((rand()%10)*0.1);
	}
	mActor->SetHeading(mActor->GetHeading()+Seconds*mHeadingModifier);
	mActor->SetOrientation(mActor->GetHeading());
}

