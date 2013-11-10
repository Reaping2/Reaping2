#include "i_core.h"

PlasmaGunAction::PlasmaGunAction()
: WeaponAsset("plasma_gun")
{
	mAreBlockedActionsExcluded=false;
	mSecsToEnd=0.5;
	mIsLoop=false;
	mCooldownReduction=200;
}

void PlasmaGunAction::Update(Actor& Actor, double Seconds) const
{
	WeaponAsset::Update(Actor,Seconds);
}

void PlasmaGunAction::Shoot(Actor& Actor) const
{
	Actor::ActionDesc_t* State=Actor.GetActionDesc(mId);
	if(!State)return;
	double nextState = State->GetState();
	if(nextState!=100.0)return;
	State->SetState(64.0);
	PlasmaShot* ps=new PlasmaShot();
	//esetleg kiemelni :D
	ps->SetX(Actor.GetX());
	ps->SetY(Actor.GetY());
	double ori = Actor.GetOrientation()+(rand()%20)/100.-0.1;
	ps->SetOrientation(ori);
	ps->SetHeading(ori);
	Scene::Get().AddActor(ps);
}

void PlasmaGunAction::ShootAlt(Actor& Actor) const
{
	Actor::ActionDesc_t* State=Actor.GetActionDesc(mId);
	if(!State)return;
	double nextState = State->GetState();
	if(nextState!=100.0)return;
	State->SetState(0.0);

	double ori = Actor.GetOrientation()+(rand()%6)/100.-0.03;

	PlasmaShot* ps=new PlasmaShot();
	ps->SetX(Actor.GetX());
	ps->SetY(Actor.GetY());
	double oril = ori+0.15;
	ps->SetOrientation(oril);
	ps->SetHeading(oril);
	Scene::Get().AddActor(ps);

	ps=new PlasmaShot();
	ps->SetX(Actor.GetX());
	ps->SetY(Actor.GetY());
	oril = ori-0.15;
	ps->SetOrientation(oril);
	ps->SetHeading(oril);
	Scene::Get().AddActor(ps);

	ps=new PlasmaShot();
	ps->SetX(Actor.GetX());
	ps->SetY(Actor.GetY());
	ps->SetOrientation(ori);
	ps->SetHeading(ori);
	Scene::Get().AddActor(ps);
}