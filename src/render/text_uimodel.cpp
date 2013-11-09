#include "i_render.h"

void TextUiModel::Draw( const Widget& Wdg )const
{
	static Font& Fnt(Font::Get());

	const Widget::Prop& TextProp=Wdg.GetProp(Widget::PT_Text);
	if(TextProp.Type!=Widget::Prop::T_Str)return;
	std::string Buf(TextProp.Value.ToStr);
	if(Buf.empty())return;
	glm::vec2 TexDim=Fnt.GetDim(Buf);
	if(TexDim.x<=std::numeric_limits<float>::epsilon())return;
	glm::vec4 Dim=Wdg.GetDimensions();
	Dim.z/=TexDim.x;
	glNormal3f(0.0, 0.0, 1.0);
	const Widget::Prop& ColorProp=Wdg.GetProp(Widget::PT_Color);
	uint32_t Color=(uint32_t)((ColorProp.Type==Widget::Prop::T_Int)?ColorProp.Value.ToInt:0xffffff);
	glColor3ub((Color>>16)&0xff,(Color>>8)&0xff,Color&0xff);
	glBindTexture(GL_TEXTURE_2D,Fnt.GetTexId());
	glBegin(GL_QUADS);	// UiModel QUADokat rajzol, igy egy begin-end-be beleferhetnenk akar
	for(std::string::const_iterator i=Buf.begin(),e=Buf.end();i!=e;++i)
	{
		SpritePhase const& Phase=Fnt.GetChar(*i);
		const float CharWidth=Dim.z*(Phase.Right-Phase.Left);
		const float CharHeight=Dim.z*(Phase.Bottom-Phase.Top);
		glTexCoord2f( Phase.Left,   Phase.Top); glVertex3f(Dim.x,Dim.y+CharHeight,0);
		glTexCoord2f(Phase.Right,   Phase.Top); glVertex3f(Dim.x+CharWidth,Dim.y+CharHeight,0);
		glTexCoord2f(Phase.Right,Phase.Bottom); glVertex3f(Dim.x+CharWidth,Dim.y,0);
		glTexCoord2f( Phase.Left,Phase.Bottom); glVertex3f(Dim.x,Dim.y,0);
		Dim.x+=CharWidth;
	}
	glEnd();

}

