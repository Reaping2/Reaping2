#ifndef INCLUDED_CORE_COLLISION_STORE_H
#define INCLUDED_CORE_COLLISION_STORE_H

class CollisionStore : public Singleton<CollisionStore>
{
	typedef boost::shared_ptr<CollisionModel> Elem_t;
	typedef std::vector<Elem_t> Models_t;
	friend class Singleton<CollisionStore>;
	Models_t mModels;
	CollisionStore();
	static size_t MapTypes(CollisionClass::Type T1, CollisionClass::Type T2);
	void SetCollisionModel(CollisionClass::Type T1, CollisionClass::Type T2,Elem_t Model);
public:
	CollisionModel const& GetCollisionModel(CollisionClass::Type T1, CollisionClass::Type T2) const;
};

#endif//INCLUDED_CORE_COLLISION_STORE_H
