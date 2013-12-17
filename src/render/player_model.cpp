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
	//glScalef(.13f,.13f,0);
	bool Drawn=false;
	const float Radius=(float)Object.GetRadius();
	Actor::ActionList_t const& Actions=Object.GetActions();
	for(Actor::ActionList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;++i)
	{
		Action const& Act=*i->second;
		SpritePhase const& Phase=mSprites(Act.GetId())((int32_t)Act.GetState());
		// todo: renderer->settexture, ellenorizzuk, hogy nem ugyanaz-e (nemtom, gl csinal-e ilyet)
		if(!mSprites(Act.GetId()).IsValid()) continue;
		DrawPhase(Phase,Radius);
		Drawn=true;
	}

	Actor::ItemList_t const& items=Object.GetItems();
	for(Actor::ItemList_t::const_iterator i=items.begin(),e=items.end();i!=e;++i)
	{
		Item const& Act=*i;
		SpritePhase const& Phase=mSprites(Act.GetId())((int32_t)Act.GetState());
		DrawPhase(Phase,Radius);
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
void PlayerModel::DrawPhase(SpritePhase const& Phase, float Radius) const
{
	glBindTexture(GL_TEXTURE_2D, Phase.TexId);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(1.f,1.f,1.f);
	glTexCoord2d( Phase.Left,Phase.Bottom); glVertex3f(-Radius, -Radius, 0.0f);
	glTexCoord2d(Phase.Right,Phase.Bottom); glVertex3f(-Radius,  Radius, 0.0f);
	glTexCoord2d(Phase.Right,   Phase.Top); glVertex3f( Radius,  Radius, 0.0f);
	glTexCoord2d( Phase.Left,   Phase.Top); glVertex3f( Radius, -Radius, 0.0f);
	glEnd();
}