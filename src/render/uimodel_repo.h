#ifndef INCLUDED_RENDER_UIMODEL_REPO_H
#define INCLUDED_RENDER_UIMODEL_REPO_H
#include "platform/i_platform.h"
#include "ui/i_ui.h"
#include "uimodel.h"

class UiModelRepo : public Repository<UiModel>, public Singleton<UiModelRepo>
{
    friend class Singleton<UiModelRepo>;
    UiModelRepo();
    UiModel mDefaultUiModel;
};

#endif//INCLUDED_RENDER_UIMODEL_REPO_H
