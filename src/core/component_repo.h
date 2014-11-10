#ifndef INCLUDED_COMPONENT_REPO_H
#define INCLUDED_COMPONENT_REPO_H

class Component;
class ComponentRepo : public Factory<Component>, public Singleton<ComponentRepo>
{
    friend class Singleton<ComponentRepo>;
    ComponentRepo();
};


#endif//INCLUDED_COMPONENT_REPO_H