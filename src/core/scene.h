#ifndef INCLUDED_CORE_SCENE_H
#define INCLUDED_CORE_SCENE_H

class Scene : public Singleton<Scene>
{
	ActorList mActors[CollisionClass::Num_Classes];
	AllActorInSceneList mAllActors;
	friend class Singleton<Scene>;
	Scene(){}
public:
	void Update(double DeltaTime);
	void AddActor(Actor* Object);
	const AllActorInSceneList& GetActors() {return mAllActors;}
};

#endif//INCLUDED_CORE_SCENE_H
