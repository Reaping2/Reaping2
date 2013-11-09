#include "i_render.h"

void UiModel::Draw( const Widget& Wdg )const
{
	glm::vec4 const& Dim=Wdg.GetDimensions();
	GLfloat a=0.25f+(!!(int32_t)Wdg(Widget::PT_Flagged)?0.5f:0.f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);	// UiModel QUADokat rajzol, igy egy begin-end-be beleferhetnenk akar
	glColor4f(.7f, 0.7f, 0.7f,a);	glVertex3f(Dim.x,Dim.y,0);
	glColor4f(.7f, 0.7f, 0.7f,a);	glVertex3f(Dim.x,Dim.y+Dim.w,0);
	glColor4f(0.f, 0.7f, 0.7f,a);	glVertex3f(Dim.x+Dim.z,Dim.y+Dim.w,0);
	glColor4f(0.f, 0.7f, 0.7f,a);	glVertex3f(Dim.x+Dim.z,Dim.y,0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

