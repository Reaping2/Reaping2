#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class Actor;
class ActionRepo : public Singleton<ActionRepo>
{
public:
	typedef boost::function<Action*(int32_t Id, Actor& actor)> ActionFunctor;
private:
	friend class Singleton<ActionRepo>;
	ActionRepo();
	~ActionRepo(){}
	template<typename Element_T>
	static Action * Create(int32_t Id, Actor& actor);
protected:
//	typedef typename Repository<Element_T> RepoBase;
	typedef std::map<int32_t, ActionFunctor> ElementMap_t;
	ElementMap_t mElements;
	ActionFunctor mDefaultElement;
public:
	Action * operator()(int32_t Id, Actor& actor) const;
	Action * operator()(int32_t Id, Actor& actor);	// lazy load
	void Register(int32_t Id, ActionFunctor functor);
	Action * GetDefaultAction(Actor& actor);
};


#endif//INCLUDED_ACTION_HOLDER_H