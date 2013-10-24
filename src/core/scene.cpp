#include "i_core.h"

void Scene::AddActor( Actor* Object )
{
	mActors[Object->GetCC()].push_back(*Object);
	mAllActors.push_back(*Object);
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
	// eloszor lecollideolunk, aztan update, nem akarom, hogy menetkozben megszunjenek actorok
	for(size_t i=0;i<CollisionClass::Num_Classes;++i)
	{
		ActorList& ListI=mActors[i];
		for(size_t j=i;j<CollisionClass::Num_Classes;++j)
		{
			if(!(Collisions[i]&(1<<j)))
				continue;
			ActorList& ListJ=mActors[j];
			for(ActorList::iterator it=ListI.begin(),e=ListI.end();it!=e;++it)
				it->Collide(DeltaTime,ListJ);
		}
	}
	for(size_t i=0;i<CollisionClass::Num_Classes;++i)
	{
		ActorList& ListI=mActors[i];
		for(ActorList::iterator it=ListI.begin(),e=ListI.end();it!=e;++it)
			it->Update(DeltaTime);
	}
}

