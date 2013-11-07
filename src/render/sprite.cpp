#include "i_render.h"

Sprite::Sprite(int32_t TexId, GLuint w, GLuint h, GLuint sx, GLuint sy, size_t steps)
: RepoBase(DefaultSpritePhase)
{
	Texture const& Tex=TextureRepo::Get()(TexId);
	mTexId=Tex.TexId();
	mTexW=(GLfloat)Tex.Width();
	mTexH=(GLfloat)Tex.Height();
	if(mTexW)mTexW=1/mTexW;
	if(mTexH)mTexH=1/mTexH;
	mWidth=w;
	mHeight=h;
	mStartX=sx;
	mStartY=sy;
	mSteps=steps-1;
	for(int32_t i=0,e=(int32_t)steps;i!=e;++i)
		mElements.insert(i,new SpritePhase(mTexId,mStartY*mTexH,(mStartX+mWidth*i)*mTexW,(mStartY+mHeight)*mTexH,(mStartX+mWidth*(i+1))*mTexW));
}

SpritePhase const& Sprite::operator()( int32_t Phase ) const
{
	int32_t SprPhase=(int32_t)(Phase/100.*mSteps+0.5);
	return RepoBase::operator()(SprPhase);
}

SpritePhase Sprite::DefaultSpritePhase=SpritePhase();
