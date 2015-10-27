#ifndef INCLUDED_ENGINE_SHOW_TEXT_SYSTEM_H
#define INCLUDED_ENGINE_SHOW_TEXT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "show_text_event.h"

namespace engine {

struct TimedText
{
    double mSeconds;
    std::string mText;
    TimedText(double seconds, std::string text)
        :mSeconds(seconds),mText(text){}
};

class ShowTextSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(ShowTextSystem)
    ShowTextSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ModelValue mShowTextModel;
    std::string const& GetText();
    std::string mText;
    double mTTL;
    typedef std::list<TimedText> Timed_Texts_t;
    Timed_Texts_t mTimedTexts;
    AutoReg mOnShowText;
    void OnShowText(engine::ShowTextEvent const& Evt);
};

} // namespace engine

#endif//INCLUDED_ENGINE_SHOW_TEXT_SYSTEM_H


//command:  "classgenerator.exe" -g "system" -c "show_text_system" -e "engine-showText"
