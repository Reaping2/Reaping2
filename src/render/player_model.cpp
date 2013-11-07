#include "i_render.h"

PlayerModel::PlayerModel()
	: mSprites(RenderableRepo::Get()(AutoId("player")))
{

}

void PlayerModel::Draw(Actor const& Object)const
{
	static const GLfloat pi_under_180=180.f/boost::math::constants::pi<float>();
	static const GLfloat sqrt3d2=std::sqrt(3.f)*0.5f;
	glPushMatrix();
	glTranslatef((GLfloat)Object.GetX(),(GLfloat)Object.GetY(),0);
	glRotatef((GLfloat)Object.GetOrientation() * pi_under_180 - 90.f, 0.f, 0.f, 1.f);
	glScalef(.13f,.13f,0);
	bool Drawn=false;
	Actor::ActionDescList_t const& Actions=Object.GetActions();
	for(Actor::ActionDescList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;++i)
	{
		Actor::ActionDesc_t const& Act=*i;
		SpritePhase const& Phase=mSprites(Act.GetId())((int32_t)Act.GetState());
		// todo: renderer->settexture, ellenorizzuk, hogy nem ugyanaz-e (nemtom, gl csinal-e ilyet)
		glBindTexture(GL_TEXTURE_2D, Phase.TexId);
		glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glColor3f(1.f,1.f,1.f);
		glTexCoord2d( Phase.Left,Phase.Bottom); glVertex3f(-0.5f, -0.5f, 0.0f);
		glTexCoord2d(Phase.Right,Phase.Bottom); glVertex3f(-0.5f,  0.5f, 0.0f);
		glTexCoord2d(Phase.Right,   Phase.Top); glVertex3f( 0.5f,  0.5f, 0.0f);
		glTexCoord2d( Phase.Left,   Phase.Top); glVertex3f( 0.5f, -0.5f, 0.0f);
		glEnd();
		Drawn=true;
	}
	if(!Drawn)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glBegin(GL_TRIANGLES);
		glColor3f(.7f, 0.7f, 0.7f);	glVertex3f(-0.5f,		  0.0f,	0);
		glColor3f(0.f, 0.7f, 0.7f);	glVertex3f(0.25f,-sqrt3d2*0.5f,	0);
		glColor3f(0.f, 0.7f, 0.7f);	glVertex3f(0.25f, sqrt3d2*0.5f,	0);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	}
	glPopMatrix();
}