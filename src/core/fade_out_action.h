#ifndef INCLUDED_CORE_FADE_OUT_ACTION_H
#define INCLUDED_CORE_FADE_OUT_ACTION_H

class FadeOutAction : public Action
{
	friend class Factory<Action>;
	FadeOutAction(int32_t Id);
	virtual void Deactivate();
};

#endif//INCLUDED_CORE_FADE_OUT_ACTION_H
