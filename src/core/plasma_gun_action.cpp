#include "i_core.h"

PlasmaGunAction::PlasmaGunAction(int32_t Id, Actor& actor)
: WeaponAsset(Id,actor)
{
	mAreBlockedActionsExcluded=false;
	mSecsToEnd=0.5;
	mIsLoop=false;
	mCooldownReduction=200;
}

void PlasmaGunAction::Update(double Seconds) 
{
	WeaponAsset::Update(Seconds);
}

void PlasmaGunAction::Shoot() const
{
	Action& State=mActor.GetActionDesc(mId);
	double nextState = State.GetState();
	if(nextState!=100.0)return;
	State.SetState(64.0);
	PlasmaShot* ps=new PlasmaShot();
	//esetleg kiemelni :D
	ps->SetX(mActor.GetX());
	ps->SetY(mActor.GetY());
	double ori = mActor.GetOrientation()+(rand()%20)/100.-0.1;
	ps->SetOrientation(ori);
	ps->SetHeading(ori);
	Scene::Get().AddActor(ps);
}

void PlasmaGunAction::ShootAlt() const
{
	Action& State=mActor.GetActionDesc(mId);
	double nextState = State.GetState();
	if(nextState!=100.0)return;
	State.SetState(0.0);

	double ori = mActor.GetOrientation()+(rand()%6)/100.-0.03;

	PlasmaShot* ps=new PlasmaShot();
	ps->SetX(mActor.GetX());
	ps->SetY(mActor.GetY());
	double oril = ori+0.15;
	ps->SetOrientation(oril);
	ps->SetHeading(oril);
	Scene::Get().AddActor(ps);

	ps=new PlasmaShot();
	ps->SetX(mActor.GetX());
	ps->SetY(mActor.GetY());
	oril = ori-0.15;
	ps->SetOrientation(oril);
	ps->SetHeading(oril);
	Scene::Get().AddActor(ps);

	ps=new PlasmaShot();
	ps->SetX(mActor.GetX());
	ps->SetY(mActor.GetY());
	ps->SetOrientation(ori);
	ps->SetHeading(ori);
	Scene::Get().AddActor(ps);
}