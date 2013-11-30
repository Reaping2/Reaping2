#include "i_render.h"

void BarModel::Draw( const Widget& Wdg ) const
{
	glm::vec4 const& Dim=Wdg.GetDimensions();
	int32_t MaxProg=Wdg(Widget::PT_MaxProgress);
	int32_t Prog=Wdg(Widget::PT_Progress);
	float Percent=std::min<float>(1.0f,std::max<float>(0.0f,MaxProg?((float)Prog/MaxProg):1.0f));
	uint32_t Color=(int32_t)(Wdg(Widget::PT_Highlight))?(int32_t)(Wdg(Widget::PT_HighlightColor)):(int32_t)(Wdg(Widget::PT_Color));
	glColor4ub((Color>>24)&0xff,(Color>>16)&0xff,(Color>>8)&0xff,Color&0xff);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);	// UiModel QUADokat rajzol, igy egy begin-end-be beleferhetnenk akar
	glVertex3f(Dim.x,Dim.y,0);
	glVertex3f(Dim.x,Dim.y+Dim.w,0);
	glVertex3f(Dim.x+Dim.z*Percent,Dim.y+Dim.w,0);
	glVertex3f(Dim.x+Dim.z*Percent,Dim.y,0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

