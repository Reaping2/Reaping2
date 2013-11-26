#include "i_core.h"

void Scene::AddActor( Actor* Object )
{
	mNewActors.push_back(*Object);
	// mikor az obj torlodik, magatol kikerul a listabol
}

void Scene::Update( double DeltaTime )
{
	for(ActorList_t::iterator it=mAllActors.begin(),e=mAllActors.end();it!=e;++it)
		it->DoControlling(DeltaTime);
	static const uint32_t Collisions[]={
		0,																												// no collision
		1<<CollisionClass::Creep | 1<<CollisionClass::Mine | 1<<CollisionClass::Player | 1<<CollisionClass::Wall,		// projectile
		1<<CollisionClass::Projectile | 1<<CollisionClass::Mine | 1<<CollisionClass::Player | 1<<CollisionClass::Wall,	// creep
		1<<CollisionClass::Projectile | 1<<CollisionClass::Creep | 1<<CollisionClass::Wall,								// mine
		1<<CollisionClass::Projectile | 1<<CollisionClass::Creep | 1<<CollisionClass::Player | 1<<CollisionClass::Wall,	// player
		1<<CollisionClass::Projectile | 1<<CollisionClass::Creep | 1<<CollisionClass::Mine | 1<<CollisionClass::Player,	// wall
	};
	for(ActorList_t::iterator it1=mAllActors.begin(),e1=mAllActors.end();it1!=e1;++it1)
	{
		for(ActorList_t::iterator it2=it1;it2!=e1;++it2)
		{
			if(it1==it2)continue;
			Actor& A=*it1;
			Actor& B=*it2;
			if(!(Collisions[A.GetCC()]&(1<<B.GetCC())))continue;
			CollisionModel const& CollModel=mCollisionStore.GetCollisionModel(A.GetCC(),B.GetCC());
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
: mDimensions(-2,-2,2,2)
, mCollisionStore(CollisionStore::Get())
, mTypeId(0)
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

void Scene::SetType( std::string const& Type )
{
	mTypeId=AutoId(Type);
}

int32_t Scene::GetTypeId() const
{
	return mTypeId;
}

