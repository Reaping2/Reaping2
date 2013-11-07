#ifndef INCLUDED_RENDER_SPRITE_PHASE_H
#define INCLUDED_RENDER_SPRITE_PHASE_H

class Sprite;
struct SpritePhase{
	GLuint TexId;
	GLfloat Top;
	GLfloat Left;
	GLfloat Bottom;
	GLfloat Right;
	friend class Sprite;
private:
	SpritePhase(GLuint i=0,GLfloat t=0.f,GLfloat l=0.f,GLfloat b=0.f,GLfloat r=0.f);
};

#endif//INCLUDED_RENDER_SPRITE_PHASE_H
