#ifndef INCLUDED_RENDER_PARTICLE_ENGINE_H
#define INCLUDED_RENDER_PARTICLE_ENGINE_H

class ParticleEngine : public Singleton<ParticleEngine>
{
public:
	enum ParticleType {
		GroundParticle,
		NumTypes,
	};
	void Add(std::auto_ptr<Particle> Part, ParticleType Typ);
	void Draw(ParticleType Type)const;
protected:
	friend class Singleton<ParticleEngine>;
	typedef boost::ptr_vector<Particle> Particles_t;
	RenderableRepo& mRenderables;
	Particles_t mParticles[NumTypes];
	size_t mMaxParticles[NumTypes];
	ParticleEngine();
};

#endif//INCLUDED_RENDER_PARTICLE_ENGINE_H
