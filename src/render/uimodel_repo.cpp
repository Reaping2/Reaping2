#include "i_render.h"

UiModelRepo::UiModelRepo()
: RepoBase(DefaultUiModel)
{
	int32_t Id=AutoId("text_widget");
	mElements.insert(Id,new TextUiModel);
}

UiModel UiModelRepo::DefaultUiModel=UiModel();
