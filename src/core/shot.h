#ifndef INCLUDED_CORE_SHOT_H
#define INCLUDED_CORE_SHOT_H

class Shot : public Actor
{
protected:
	int32_t mParentGuid;
public:
	Shot(std::string const& Name);
	virtual void ClipScene();
	virtual void Collide(Actor& Other);
	virtual void UpdateLifetime();
	virtual void SetParent(Actor& P);
};

#endif//INCLUDED_CORE_SHOT_H
