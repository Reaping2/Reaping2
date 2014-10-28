#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class Actor;
class ActionRepo : public Factory<Action>, public Singleton<ActionRepo>
{
    friend class Singleton<ActionRepo>;
    ActionRepo();
};


#endif//INCLUDED_ACTION_HOLDER_H