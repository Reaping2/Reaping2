#pragma once
#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

#include "widget.h"

namespace Json {
class Value;
}

class ImageWidget : public Widget
{
    typedef Widget BaseClass;
public:
    ImageWidget( int32_t Id );
    virtual void Init( Json::Value& Descriptor );
};

#endif // INCLUDED_IMAGE_H

