#ifndef INCLUDED_CORE_SHOT_H
#define INCLUDED_CORE_SHOT_H

class Shot : public Actor
{
public:
	Shot(std::string const& Name);
	virtual void ClipScene();
	virtual void Collide(Actor& Other);
	virtual void UpdateLifetime();
};

#endif//INCLUDED_CORE_SHOT_H
