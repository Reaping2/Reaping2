#ifndef INCLUDED_CORE_SCENE_H
#define INCLUDED_CORE_SCENE_H

class Scene : public Singleton<Scene>
{
	ActorList_t mAllActors;
	ActorList_t mNewActors;
	CollisionStore& mCollisionStore;
	glm::vec4 mDimensions;
	friend class Singleton<Scene>;
	Scene();
public:
	~Scene();
	void Update(double DeltaTime);
	void AddActor(Actor* Object);
	glm::vec4 const& GetDimensions();
	const ActorList_t& GetActors() {return mAllActors;}
};

#endif//INCLUDED_CORE_SCENE_H
