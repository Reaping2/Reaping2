#include "i_render.h"

Sprite::Sprite(int32_t TexId, GLuint w, GLuint h, Json::Value const& Phases)
: RepoBase(DefaultSpritePhase)
{
	Texture const& Tex=TextureRepo::Get()(TexId);
	mTexId=Tex.TexId();
	GLfloat TexW=(GLfloat)Tex.Width();
	GLfloat TexH=(GLfloat)Tex.Height();
	if(TexW)TexW=1/TexW;
	if(TexH)TexH=1/TexH;
	mSteps=Phases.size()-1;
	if(Phases.empty())return;
	int idx=0;
	for(Json::Value::const_iterator i=Phases.begin(),e=Phases.end();i!=e;++i,++idx)
	{
		Json::Value const& Phase=*i;
		uint32_t sx, sy;
		if(!Json::GetUInt(Phase["x"],sx)||
			!Json::GetUInt(Phase["y"],sy))
		{
			assert(false);
			return;
		}
		mElements.insert(idx,new SpritePhase(mTexId,sy*TexH,sx*TexW,(sy+h)*TexH,(sx+w)*TexW));
	}
}

SpritePhase const& Sprite::operator()( int32_t Phase ) const
{
	int32_t SprPhase=(int32_t)(Phase/100.*mSteps+0.5);
	return RepoBase::operator()(SprPhase);
}

SpritePhase Sprite::DefaultSpritePhase=SpritePhase();
