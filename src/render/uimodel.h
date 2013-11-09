#ifndef INCLUDED_RENDER_UIMODEL_H
#define INCLUDED_RENDER_UIMODEL_H

class UiModelRepo;
class UiModel
{
protected:
	friend class UiModelRepo;
	UiModel(){}
public:
	virtual ~UiModel(){}
	virtual void Draw(const Widget& Wdg)const;
};

#endif//INCLUDED_RENDER_UIMODEL_H
