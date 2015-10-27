#include "platform/i_platform.h"
#include "show_text_system.h"

namespace engine {

ShowTextSystem::ShowTextSystem()
    : mScene( Scene::Get() )
    , mShowTextModel( GetText(), "show_text", &RootModel::Get() )
    , mText("Welcome to HELL bitches!")
    , mTTL(4)
{
}


void ShowTextSystem::Init()
{
    mOnShowText=EventServer<engine::ShowTextEvent>::Get().Subscribe( boost::bind( &ShowTextSystem::OnShowText, this, _1 ) );
}


void ShowTextSystem::Update(double DeltaTime)
{
    mTTL-=DeltaTime;
    if (mTTL<0)
    {
        mTTL=0;
    }
    if (mTTL<=0&&!mText.empty())
    {
        mText.clear();
        mTTL=0;
    }
    if (mTTL<=0&&mTimedTexts.size()>0)
    {
        TimedText& text=mTimedTexts.front();
        mTTL=text.mSeconds;
        mText=text.mText;
        mTimedTexts.pop_front();
    }
}

void ShowTextSystem::OnShowText(engine::ShowTextEvent const& Evt)
{
    mTimedTexts.push_back(TimedText(Evt.mSeconds,Evt.mText));
}

std::string const& ShowTextSystem::GetText()
{
    return mText;
}


} // namespace engine

