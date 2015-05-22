#ifndef INCLUDED_CORE_ACCURACY_COMPONENT_H
#define INCLUDED_CORE_ACCURACY_COMPONENT_H

#include "i_accuracy_component.h"
#include "core/property_loader.h"

class AccuracyComponent : public IAccuracyComponent
{
public:
    AccuracyComponent();
    void SetAccuracyBase(int32_t accuracy);
    virtual Buffable<int32_t>& GetAccuracy();
protected:
    friend class ComponentFactory;
    Buffable<int32_t> mAccuracy;
private:
};

class AccuracyComponentLoader : public ComponentLoader<AccuracyComponent>
{
    virtual void BindValues();
protected:
    AccuracyComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_ACCURACY_COMPONENT_H

