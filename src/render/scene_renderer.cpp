#include "i_render.h"

SceneRenderer::SceneRenderer()
:mRenderables(RenderableRepo::Get())
{

}

void SceneRenderer::Draw( Scene& Object )
{
	static int32_t DefaultActId=AutoId("default_action");
	Sprite const& Spr=mRenderables(Object.GetTypeId())(DefaultActId);
	if(!Spr.IsValid())return;
	SpritePhase const& Phase=Spr(0);
	glm::vec4 const& Dim=Object.GetDimensions();
	glBindTexture(GL_TEXTURE_2D, Phase.TexId);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(1.f,1.f,1.f);
	glTexCoord2d( Phase.Left,Phase.Bottom); glVertex3f(Dim.x, Dim.y, 0.0f);
	glTexCoord2d(Phase.Right,Phase.Bottom); glVertex3f(Dim.x, Dim.w, 0.0f);
	glTexCoord2d(Phase.Right,   Phase.Top); glVertex3f(Dim.z, Dim.w, 0.0f);
	glTexCoord2d( Phase.Left,   Phase.Top); glVertex3f(Dim.z, Dim.y, 0.0f);
	glEnd();
}
