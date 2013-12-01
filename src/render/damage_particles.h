#ifndef INCLUDED_RENDER_DAMAGE_PARTICLES_H
#define INCLUDED_RENDER_DAMAGE_PARTICLES_H

class DamageParticles : public Singleton<DamageParticles>
{
	ParticleEngine& mParticleEngine;
	DamageParticles();
	void Load();
	friend class Singleton<DamageParticles>;
	Registration mOnDamageTaken;
	std::vector<int32_t> mValidIds;
	void OnDamageTaken(DamageTakenEvent const& Evt);
};

#endif//INCLUDED_RENDER_DAMAGE_PARTICLES_H
