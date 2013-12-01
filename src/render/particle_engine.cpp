#include "i_render.h"

ParticleEngine::ParticleEngine()
:mRenderables(RenderableRepo::Get())
{
	for(size_t i=0;i<NumTypes;++i)
		mMaxParticles[i]=10000;
}

void ParticleEngine::Add( std::auto_ptr<Particle> Part, ParticleType Typ )
{
	Particles_t& Parts=mParticles[Typ];
	if(Parts.size()>=mMaxParticles[Typ])
		Parts.erase(Parts.begin());
	Parts.push_back(Part);
}

void ParticleEngine::Draw( ParticleType Type )const
{
	Particles_t const& Parts=mParticles[Type];
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_QUADS);
	GLuint mPrevTexId=0;
	for(Particles_t::const_iterator i=Parts.begin(),e=Parts.end();i!=e;++i)
	{
		Particle const& Part=*i;
		static int32_t DefaultActId=AutoId("default_action");
		Sprite const& Spr=mRenderables(Part.mId)(DefaultActId);
		if(!Spr.IsValid())return;
		SpritePhase const& Phase=Spr(0);
		if(Phase.TexId!=mPrevTexId)
		{
			glEnd();
			mPrevTexId=Phase.TexId;
			glBindTexture(GL_TEXTURE_2D, mPrevTexId);
			glBegin(GL_QUADS);
		}
		glColor4fv(glm::value_ptr(Part.mColor));
		glTexCoord2d( Phase.Left,Phase.Bottom); glVertex3f(Part.mQuad[0].x, Part.mQuad[0].y, 0.0f);
		glTexCoord2d(Phase.Right,Phase.Bottom); glVertex3f(Part.mQuad[1].x, Part.mQuad[1].y, 0.0f);
		glTexCoord2d(Phase.Right,   Phase.Top); glVertex3f(Part.mQuad[2].x, Part.mQuad[2].y, 0.0f);
		glTexCoord2d( Phase.Left,   Phase.Top); glVertex3f(Part.mQuad[3].x, Part.mQuad[3].y, 0.0f);
	}
	glEnd();
}

