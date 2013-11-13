#ifndef INCLUDED_CORE_SCENE_H
#define INCLUDED_CORE_SCENE_H

class Scene : public Singleton<Scene>
{
	ActorList mActors[CollisionClass::Num_Classes];
	AllActorInSceneList mAllActors;
	friend class Singleton<Scene>;
	Scene();
	glm::vec4 mDimensions;
public:
	void Update(double DeltaTime);
	void AddActor(Actor* Object);
	glm::vec4 const& GetDimensions();
	const AllActorInSceneList& GetActors() {return mAllActors;}
};

#endif//INCLUDED_CORE_SCENE_H
