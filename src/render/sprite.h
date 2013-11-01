#ifndef INCLUDED_RENDER_SPRITE_H
#define INCLUDED_RENDER_SPRITE_H

class TextureRepo;
class Texture;

struct SpritePhase{
	GLuint TexId;
	GLfloat Top;
	GLfloat Left;
	GLfloat Bottom;
	GLfloat Right;
	SpritePhase(GLuint i=0,GLfloat t=0.f,GLfloat l=0.f,GLfloat b=0.f,GLfloat r=0.f)
		:TexId(i),Top(t),Left(l),Bottom(b),Right(r){}
};

class Sprite
{
	friend class TextureRepo;
	GLuint mTexId;
	GLfloat mTexW;
	GLfloat mTexH;
	GLuint mWidth;
	GLuint mHeight;
	GLuint mStartX;
	GLuint mStartY;
	size_t mSteps;
	SpritePhase* mPhases;
	Sprite(Texture* t, GLuint w, GLuint h, GLuint sx, GLuint sy, size_t steps);
	void CreatePhase(int32_t Phase);
public:
	~Sprite();
	SpritePhase const& GetPhase(int32_t Phase)const;	// [0..99]
};

#endif//INCLUDED_RENDER_SPRITE_H
