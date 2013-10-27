#include "i_render.h"

PlayerModel::PlayerModel()
{

}

void PlayerModel::Draw(Actor const& Object)const
{
	static const GLfloat pi_under_180=180.f/boost::math::constants::pi<float>();
	static const GLfloat sqrt3d2=std::sqrt(3.f)*0.5f;
	glPushMatrix();
	glTranslatef((GLfloat)Object.GetX(),(GLfloat)Object.GetY(),0);
	glRotatef((GLfloat)Object.GetOrientation() * pi_under_180 + 180.f, 0.f, 0.f, 1.f);
	glScalef(.2f,.2f,0);
	glBegin(GL_TRIANGLES);
	glColor3f(0.7f, .3f, .7f);	glVertex3f(-0.5f,		  0.0f,	0);
	glColor3f(0.4f, .2f, .3f);	glVertex3f(0.25f,-sqrt3d2*0.5f,	0);
	glColor3f(0.4f, .2f, .3f);	glVertex3f(0.25f, sqrt3d2*0.5f,	0);
	glEnd();
	glPopMatrix();
}