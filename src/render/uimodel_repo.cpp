#include "i_render.h"
#include "uimodel_repo.h"
#include "text_uimodel.h"
#include "bar_model.h"
#include "image_uimodel.h"
#include "uimodel.h"
#include "grid_uimodel.h"
#include "ui/ui_model.h"

UiModelRepo::UiModelRepo()
    : RepoBase( mDefaultUiModel )
{
    ui::UiModel::Get();
    int32_t Id = AutoId( "text_widget" );
    mElements.insert( Id, new TextUiModel );
    Id = AutoId( "bar" );
    mElements.insert( Id, new BarModel );
    Id = AutoId( "image" );
    mElements.insert( Id, new render::ImageUiModel );
    Id = AutoId( "grid" );
    mElements.insert( Id, new render::GridUiModel );
    Id = AutoId( "grid_elem" );
    mElements.insert( Id, new render::GridElemUiModel );
}

