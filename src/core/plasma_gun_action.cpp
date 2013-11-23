#include "i_core.h"
//--temporary--
#include "audio/i_audio.h"
//-------------
PlasmaGunAction::PlasmaGunAction(int32_t Id)
: WeaponAsset(Id)
{
	mCooldownReduction=400;
}

void PlasmaGunAction::Update(double Seconds) 
{
	WeaponAsset::Update(Seconds);
}

void PlasmaGunAction::Shoot()
{
	if(!mActor)return;
	if(mCooldown!=0.0)return;
	//--temporary--
	AudioPlayer::Get().Play("sounds/plasmagun.ogg",AudioFile::Effect);
	//-------------
	mCooldown=30.0;
	PlasmaShot* ps=new PlasmaShot();
	//esetleg kiemelni :D
	ps->SetX(mActor->GetX());
	ps->SetY(mActor->GetY());
	double ori = mActor->GetOrientation()+(rand()%20)/100.-0.1;
	ps->SetOrientation(ori);
	ps->SetHeading(ori);
	Scene::Get().AddActor(ps);
}

void PlasmaGunAction::ShootAlt()
{
	if(!mActor)return;
	if(mCooldown!=0.0)return;
	//--temporary--
	AudioPlayer::Get().Play("sounds/plasmagunSecondary.ogg",AudioFile::Effect);
	//-------------
	mCooldown=100.0;


	double ori = mActor->GetOrientation()+(rand()%6)/100.-0.03;

	PlasmaShot* ps=new PlasmaShot();
	ps->SetX(mActor->GetX());
	ps->SetY(mActor->GetY());
	double oril = ori+0.15;
	ps->SetOrientation(oril);
	ps->SetHeading(oril);
	Scene::Get().AddActor(ps);

	ps=new PlasmaShot();
	ps->SetX(mActor->GetX());
	ps->SetY(mActor->GetY());
	oril = ori-0.15;
	ps->SetOrientation(oril);
	ps->SetHeading(oril);
	Scene::Get().AddActor(ps);

	ps=new PlasmaShot();
	ps->SetX(mActor->GetX());
	ps->SetY(mActor->GetY());
	ps->SetOrientation(ori);
	ps->SetHeading(ori);
	Scene::Get().AddActor(ps);
}