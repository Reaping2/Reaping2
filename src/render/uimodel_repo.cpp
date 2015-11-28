#include "i_render.h"
#include "uimodel_repo.h"
#include "text_uimodel.h"
#include "bar_model.h"
#include "uimodel.h"

UiModelRepo::UiModelRepo()
    : RepoBase( DefaultUiModel )
{
    int32_t Id = AutoId( "text_widget" );
    mElements.insert( Id, new TextUiModel );
    Id = AutoId( "bar" );
    mElements.insert( Id, new BarModel );
}

UiModel UiModelRepo::DefaultUiModel = UiModel();
