#ifndef INCLUDED_RENDER_DAMAGE_DECALS_H
#define INCLUDED_RENDER_DAMAGE_DECALS_H

class DamageDecals : public Singleton<DamageDecals>
{
	DecalEngine& mDecalEngine;
	DamageDecals();
	void Load();
	friend class Singleton<DamageDecals>;
	Registration mOnDamageTaken;
	std::vector<int32_t> mValidIds;
	void OnDamageTaken(DamageTakenEvent const& Evt);
};

#endif//INCLUDED_RENDER_DAMAGE_DECALS_H
