#include "i_render.h"

DamageParticles::DamageParticles()
:mParticleEngine(ParticleEngine::Get())
{
	Load();
	mOnDamageTaken=EventServer<DamageTakenEvent>::Get().Subscribe(boost::bind(&DamageParticles::OnDamageTaken,this,_1));
}

void DamageParticles::Load()
{
	RenderableRepo& Rends(RenderableRepo::Get());
	size_t c=0;
	while(true)
	{
		int32_t Id=AutoId("blood_particle"+boost::lexical_cast<std::string>(c++));
		if(!Rends.HasElem(Id))break;
		mValidIds.push_back(Id);
	}
}

void DamageParticles::OnDamageTaken( DamageTakenEvent const& Evt )
{
	if(mValidIds.empty())
		return;
	if(!(rand()%3))
		return;
	glm::vec2 Pos=Evt.Pos+glm::vec2((rand()%10-5.f)/200.f,(rand()%10-5.f)/200.f);
	float Rotation=(float)(rand()%360);
	static const float MaxBloodRadius=0.2f;
	glm::vec2 v=glm::rotate(glm::vec2(0,(10+rand()%90)*0.01f*MaxBloodRadius),Rotation);
	std::auto_ptr<Particle> Part(new Particle);
	for(size_t i=0;i<4;++i)
	{
		Part->mQuad[i]=Pos+v;
		v=glm::rotate(v,90.f);
	}
	Part->mColor.w=(10+rand()%91)*0.01f;
	Part->mId=mValidIds[rand()%mValidIds.size()];
	mParticleEngine.Add(Part,ParticleEngine::GroundParticle);
}

