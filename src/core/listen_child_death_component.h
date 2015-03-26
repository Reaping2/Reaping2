#ifndef INCLUDED_CORE_LISTEN_CHILD_DEATH_COMPONENT_H
#define INCLUDED_CORE_LISTEN_CHILD_DEATH_COMPONENT_H

#include "i_listen_child_death_component.h"
#include "core/property_loader.h"

class ListenChildDeathComponent : public IListenChildDeathComponent
{
public:
    ListenChildDeathComponent();
    virtual void SetKillerOfChildGUID(int32_t killerOfChildId);
    virtual int32_t GetKillerOfChildGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mKillerOfChildGUID;
private:
};

class ListenChildDeathComponentLoader : public ComponentLoader<ListenChildDeathComponent>
{
    virtual void BindValues();
protected:
    ListenChildDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_LISTEN_CHILD_DEATH_COMPONENT_H
