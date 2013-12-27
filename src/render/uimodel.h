#ifndef INCLUDED_RENDER_UIMODEL_H
#define INCLUDED_RENDER_UIMODEL_H

class UiModelRepo;
class UiModel
{
protected:
	friend class UiModelRepo;
	UiModel(){}
	static glm::vec4 GetColor(Widget const& Wdg);
	static void ColoredBox(glm::vec4 const& Dim, glm::vec4 const& Col, UiVertexInserter_t& Inserter);
	static void TexturedBox(glm::vec4 const& Dim, SpritePhase const& Phase, glm::vec4 const& Col, GLuint TexId, UiVertexInserter_t& Inserter);
public:
	virtual ~UiModel(){}
	virtual void CollectVertices(Widget const& Wdg,UiVertexInserter_t& Inserter)const;
};

#endif//INCLUDED_RENDER_UIMODEL_H
