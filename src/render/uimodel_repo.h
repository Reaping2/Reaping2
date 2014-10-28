#ifndef INCLUDED_RENDER_UIMODEL_REPO_H
#define INCLUDED_RENDER_UIMODEL_REPO_H

class UiModelRepo : public Repository<UiModel>, public Singleton<UiModelRepo>
{
    friend class Singleton<UiModelRepo>;
    UiModelRepo();
    static UiModel DefaultUiModel;
};

#endif//INCLUDED_RENDER_UIMODEL_REPO_H
