#include "i_render.h"

void TextUiModel::Draw( const Widget& Wdg )const
{
	static Font& Fnt(Font::Get());
	double FontSize= Wdg(Widget::PT_FontSize);
	double TexFontSize=Fnt.GetFontSize();
	glm::vec4 Dim=Wdg.GetDimensions();
	if(TexFontSize<=std::numeric_limits<float>::epsilon())return;
	glm::vec2 RequiredSize;
	glm::vec2 TexDim;
	std::string Buf=Wdg(Widget::PT_Text);
	do{
		if(Buf.empty())return;
		TexDim=Fnt.GetDim(Buf);
		if(TexDim.x<=std::numeric_limits<float>::epsilon())return;
		RequiredSize=TexDim;
		RequiredSize*=(FontSize/TexFontSize);
		if(RequiredSize.x<=Dim.z&&RequiredSize.y<=Dim.w)
			break;
		if(Buf.size()<=4)return;
		Buf=Buf.substr(0,Buf.size()-4);
		Buf+="...";
	}while(true);
	RequiredSize.x/=TexDim.x;
	glNormal3f(0.0, 0.0, 1.0);
	uint32_t Color=(int32_t)(Wdg(Widget::PT_Highlight))?(int32_t)(Wdg(Widget::PT_HighlightColor)):(int32_t)(Wdg(Widget::PT_Color));
	glColor4ub((Color>>24)&0xff,(Color>>16)&0xff,(Color>>8)&0xff,Color&0xff);
	glBindTexture(GL_TEXTURE_2D,Fnt.GetTexId());
	glBegin(GL_QUADS);	// UiModel QUADokat rajzol, igy egy begin-end-be beleferhetnenk akar
	for(std::string::const_iterator i=Buf.begin(),e=Buf.end();i!=e;++i)
	{
		SpritePhase const& Phase=Fnt.GetChar(*i);
		const float CharWidth=RequiredSize.x*(Phase.Right-Phase.Left);
		const float CharHeight=RequiredSize.x*(Phase.Bottom-Phase.Top);
		glTexCoord2f( Phase.Left,   Phase.Top); glVertex3f(Dim.x,Dim.y+CharHeight,0);
		glTexCoord2f(Phase.Right,   Phase.Top); glVertex3f(Dim.x+CharWidth,Dim.y+CharHeight,0);
		glTexCoord2f(Phase.Right,Phase.Bottom); glVertex3f(Dim.x+CharWidth,Dim.y,0);
		glTexCoord2f( Phase.Left,Phase.Bottom); glVertex3f(Dim.x,Dim.y,0);
		Dim.x+=CharWidth;
	}
	glEnd();

}

