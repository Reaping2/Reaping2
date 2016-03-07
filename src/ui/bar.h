#ifndef INCLUDED_UI_BAR_H
#define INCLUDED_UI_BAR_H

class Bar : public Widget
{
    typedef Widget BaseClass;
public:
    Bar( int32_t Id );
    void Init( Json::Value& Descriptor );
};

#endif//INCLUDED_UI_BAR_H
