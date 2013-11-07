#include "i_render.h"

Sprite::Sprite(Texture* t, GLuint w, GLuint h, GLuint sx, GLuint sy, size_t steps)
: mTexId(t->TexId())
, mTexW((GLfloat)t->Width())
, mTexH((GLfloat)t->Height())
, mWidth(w)
, mHeight(h)
, mStartX(sx)
, mStartY(sy)
, mSteps(steps-1)
, mPhases(NULL)
{
	if(mTexH)mTexH=1/mTexH;
	if(mTexW)mTexW=1/mTexW;
	if(steps)mPhases=new SpritePhase[steps];
	// ez a cacheeles debugban brutalis gyorsulast eredmenyez
	for(int32_t i=0,e=(int32_t)steps;i!=e;++i)
		CreatePhase(i);
}

SpritePhase const& Sprite::GetPhase(int32_t Phase) const
{
	GLuint SprPhase=(GLuint)(Phase/100.*mSteps+0.5);
	return mPhases[SprPhase];
}

void Sprite::CreatePhase(int32_t Phase)
{
	SpritePhase& OnePhase=mPhases[Phase];
	OnePhase.TexId=mTexId;
	OnePhase.Bottom=(mStartY+mHeight)*mTexH;
	OnePhase.Top=mStartY*mTexH;
	OnePhase.Left=(mStartX+mWidth*Phase)*mTexW;
	OnePhase.Right=(mStartX+mWidth*(Phase+1))*mTexW;
}

Sprite::~Sprite()
{
	delete[] mPhases;
}

