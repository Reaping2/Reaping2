#include "i_core.h"

void Scene::AddActor( Actor* Object )
{
	mNewActors.push_back(*Object);
	// mikor az obj torlodik, magatol kikerul a listabol
}

void Scene::Update( double DeltaTime )
{
	static const uint32_t Collisions[]={
		0,																						// no collision
		1<<CollisionClass::Creep | 1<<CollisionClass::Mine | CollisionClass::Player,			// projectile
		1<<CollisionClass::Projectile | 1<<CollisionClass::Mine | 1<<CollisionClass::Player,	// creep
		1<<CollisionClass::Projectile | 1<<CollisionClass::Creep,								// mine
		1<<CollisionClass::Projectile | 1<<CollisionClass::Creep | 1<<CollisionClass::Player,	// player
	};
	for(ActorList_t::iterator it1=mAllActors.begin(),e1=mAllActors.end();it1!=e1;++it1)
	{
		for(ActorList_t::iterator it2=mAllActors.begin(),e2=mAllActors.end();it2!=e2;++it2)
		{
			Actor& A=*it1;
			Actor& B=*it2;
			if(!(Collisions[A.GetCC()]&(1<<B.GetCC())))continue;
			CollisionModel const& CollModel=mCollisionStore.GetCollisionModel(A.GetCC(),B.GetCC());
			if(&A==&B)continue;
			if(!CollModel.AreActorsColliding(A,B,DeltaTime))continue;
			A.Collide(B);
			B.Collide(A);
		}
	}
	for(ActorList_t::iterator it=mAllActors.begin(),e=mAllActors.end();it!=e;++it)
		it->Update(DeltaTime);
	for(ActorList_t::iterator it=mAllActors.begin(),e=mAllActors.end(),n;(n=it,it!=e?(++n,true):false);it=n)
		it->UpdateLifetime();
	if(!mNewActors.empty())
		mAllActors.splice(mAllActors.end(),mNewActors);
}

Scene::Scene()
: mDimensions(-1,-1,1,1)
, mCollisionStore(CollisionStore::Get())
{
}

glm::vec4 const& Scene::GetDimensions()
{
	return mDimensions;
}

Scene::~Scene()
{
	mAllActors.splice(mAllActors.end(),mNewActors);
	while(!mAllActors.empty())
		delete &mAllActors.front();
}

