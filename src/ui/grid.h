#pragma once
#ifndef INCLUDED_GRID_H
#define INCLUDED_GRID_H

#include "widget.h"

namespace Json {
class Value;
}

class GridElem;
class GridWidget : public Widget
{
    typedef Widget BaseClass;
public:
    GridWidget( int32_t Id );
    virtual void Init( Json::Value& Descriptor );
};

class GridElem : public Widget
{
    typedef Widget BaseClass;
public:
    GridElem( int32_t Id );
    virtual void Init( Json::Value& Descriptor );
};

#endif // INCLUDED_GRID_H

