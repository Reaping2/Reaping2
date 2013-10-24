#include "i_render.h"

DefaultModel::DefaultModel()
{

}

void DefaultModel::Draw(Actor const& Object)const
{
	static const GLfloat pi_under_180=180.f/boost::math::constants::pi<float>();

	// nyilvan itt push matrix/pop matrix, de perpill leszarom azt is
	glTranslatef((GLfloat)Object.GetX(),(GLfloat)Object.GetY(),0);
	glRotatef((GLfloat)Object.GetOrientation() * pi_under_180, 0.f, 0.f, 1.f);
	// meg azt is, hogy ez egy VBO
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0);
	glEnd();
	glRotatef((GLfloat)-Object.GetOrientation() * pi_under_180, 0.f, 0.f, 1.f);
	glTranslatef((GLfloat)-Object.GetX(),(GLfloat)-Object.GetY(),0);
}

