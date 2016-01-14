#ifndef INCLUDED_CORE_I_BORDER_COMPONENT_H
#define INCLUDED_CORE_I_BORDER_COMPONENT_H

#include "component.h"
#include "border_type.h"
#include <vector>

class IBorderComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IBorderComponent)
    typedef std::vector<BorderType::Type> Borders_t;
    virtual void SetBorders(Borders_t borders)=0;
    virtual Borders_t GetBorders()const=0;
    virtual void SetOuterBorders(Borders_t borders)=0;
    virtual Borders_t GetOuterBorders()const=0;
};

#endif//INCLUDED_CORE_I_BORDER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_border_component" -m "Borders_t-borders"
