#include "i_render.h"

DefaultModel::DefaultModel()
: mTexId(0)
{
	OsFile Png("face.png");
	PngTexture Texture(Png);
	if(!Texture.IsValid())return;
	glGenTextures(1,&mTexId);
	glBindTexture(GL_TEXTURE_2D, mTexId);
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, Texture.GetWidth(), Texture.GetHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) Texture.GetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void DefaultModel::Draw(Actor const& Object)const
{
	static const GLfloat pi_under_180=180.f/boost::math::constants::pi<float>();
	static const GLfloat sqrt3d2=std::sqrt(3.f)*0.5f;
	glPushMatrix();
	glTranslatef((GLfloat)Object.GetX(),(GLfloat)Object.GetY(),0);
	glRotatef((GLfloat)Object.GetOrientation() * pi_under_180 + 180.f, 0.f, 0.f, 1.f);
	glScalef(.2f,.2f,0);
	if(mTexId)
	{
         glEnable(GL_TEXTURE_2D);
		 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		 glEnable(GL_BLEND);
         glBegin(GL_QUADS);
         glNormal3f(0.0, 0.0, 1.0);
		 glColor3f(1.f,1.f,1.f);
         glTexCoord2d(1, 1); glVertex3f(0.0, 0.0, 0.0);
         glTexCoord2d(1, 0); glVertex3f(0.0, 1.0, 0.0);
         glTexCoord2d(0, 0); glVertex3f(1.0, 1.0, 0.0);
         glTexCoord2d(0, 1); glVertex3f(1.0, 0.0, 0.0);
         glEnd();
         glDisable(GL_TEXTURE_2D);
		 glDisable(GL_BLEND);
	}
	else
	{
		glBegin(GL_TRIANGLES);
		glColor3f(.7f, 0.7f, 0.7f);	glVertex3f(-0.5f,		  0.0f,	0);
		glColor3f(0.f, 0.7f, 0.7f);	glVertex3f(0.25f,-sqrt3d2*0.5f,	0);
		glColor3f(0.f, 0.7f, 0.7f);	glVertex3f(0.25f, sqrt3d2*0.5f,	0);
		glEnd();
	}
	glPopMatrix();
}

DefaultModel::~DefaultModel()
{
	if(mTexId)
		glDeleteTextures(1,&mTexId);
}

