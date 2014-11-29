#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

#include "platform/factory.h"
#include "platform/singleton.h"
class Action;

class ActionRepo : public platform::Factory<Action>, public platform::Singleton<ActionRepo>
{
    friend class platform::Singleton<ActionRepo>;
    ActionRepo();
};


#endif//INCLUDED_ACTION_HOLDER_H