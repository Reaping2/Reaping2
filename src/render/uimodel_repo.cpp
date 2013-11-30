#include "i_render.h"

UiModelRepo::UiModelRepo()
: RepoBase(DefaultUiModel)
{
	int32_t Id=AutoId("text_widget");
	mElements.insert(Id,new TextUiModel);
	Id=AutoId("bar");
	mElements.insert(Id,new BarModel);
}

UiModel UiModelRepo::DefaultUiModel=UiModel();
