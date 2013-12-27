#ifndef INCLUDED_RENDER_BAR_MODEL_H
#define INCLUDED_RENDER_BAR_MODEL_H

class BarModel : public UiModel
{
	virtual void CollectVertices(const Widget& Wdg,UiVertexInserter_t& Inserter)const;
};

#endif//INCLUDED_RENDER_BAR_MODEL_H
