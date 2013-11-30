#include "i_render.h"

void UiModel::Draw( const Widget& Wdg )const
{
	glm::vec4 const& Dim=Wdg.GetDimensions();
	uint32_t Color=(int32_t)(Wdg(Widget::PT_Highlight))?(int32_t)(Wdg(Widget::PT_HighlightColor)):(int32_t)(Wdg(Widget::PT_Color));
	glColor4ub((Color>>24)&0xff,(Color>>16)&0xff,(Color>>8)&0xff,Color&0xff);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);	// UiModel QUADokat rajzol, igy egy begin-end-be beleferhetnenk akar
	glVertex3f(Dim.x,Dim.y,0);
	glVertex3f(Dim.x,Dim.y+Dim.w,0);
	glVertex3f(Dim.x+Dim.z,Dim.y+Dim.w,0);
	glVertex3f(Dim.x+Dim.z,Dim.y,0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

