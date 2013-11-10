#ifndef INCLUDED_RENDER_SPRITE_H
#define INCLUDED_RENDER_SPRITE_H

class SpriteCollection;
class Texture;

class Sprite : public Repository<SpritePhase>
{
	static SpritePhase DefaultSpritePhase;
	friend class SpriteCollection;
	GLuint mTexId;
	GLfloat mTexW;
	GLfloat mTexH;
	GLuint mWidth;
	GLuint mHeight;
	GLuint mStartX;
	GLuint mStartY;
	size_t mSteps;
	Sprite(int32_t tid, GLuint w, GLuint h, GLuint sx, GLuint sy, size_t steps);
	void CreatePhase(int32_t Phase);
public:
	bool IsValid() const
	{
		return mTexId!=0;
	}
	virtual SpritePhase const& operator()(int32_t Phase) const;
};

#endif//INCLUDED_RENDER_SPRITE_H
