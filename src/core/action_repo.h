#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class ActionRepo : public Singleton<ActionRepo>
{
public:
	typedef boost::function<Action*()> ActionFunctor;
private:
	friend class Singleton<ActionRepo>;
	ActionRepo();
	~ActionRepo(){}
protected:
//	typedef typename Repository<Element_T> RepoBase;
	typedef std::map<int32_t, ActionFunctor> ElementMap_t;
	ElementMap_t mElements;
	ActionFunctor mDefaultElement;
public:
	Action * operator()(int32_t Id) const;
	Action * operator()(int32_t Id);	// lazy load
	void Register(int32_t Id, ActionFunctor functor);
	template<typename Element_T>
	static Action * Create();
};


#endif//INCLUDED_ACTION_HOLDER_H