#ifndef INCLUDED_CORE_SCENE_H
#define INCLUDED_CORE_SCENE_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "core/i_renderable_component.h"
#include "core/actor.h"
#include "collision_class.h"
#include "program_state.h"
class ActorHolder
{
public:

    struct ActorDefaultOrderer
    { 
        typedef int32_t result_type;
        result_type operator()(const Opt<Actor>& Obj)const;
    };
    struct IsRenderable
    { 
        typedef bool result_type;
        bool operator()(const Opt<Actor>& Obj)const;
    };
    struct GetLayer
    { 
        typedef int32_t result_type;
        result_type  operator()(const Opt<Actor>& Obj)const;
    };
    struct GetZOrder
    { 
        typedef int32_t result_type;
        result_type operator()(const Opt<Actor>& Obj)const;
    };
    struct GetCollisionClass
    { 
        typedef int32_t result_type;
        result_type operator()(const Opt<Actor>& Obj)const;
    };

    typedef multi_index_container<
        Opt<Actor>,
        indexed_by<
            ordered_unique<
                ActorHolder::ActorDefaultOrderer
            >,
            ordered_non_unique<
                composite_key<
                    Opt<Actor>,
                    ActorHolder::IsRenderable,
                    ActorHolder::GetLayer,
                    ActorHolder::GetZOrder
                >
            >,
            ordered_non_unique<
                composite_key<
                    Opt<Actor>,
                    ActorHolder::GetCollisionClass
                >
            >
        >
    > ActorList_t;
    typedef ActorList_t::nth_index<1>::type  ActorListRenderableComponent_t;
    ActorList_t mAllActors;

};

typedef ActorHolder::ActorList_t ActorList_t;

template<int N>
class ActorListFilter
{
public:
    typedef ActorList_t::const_iterator const_iterator;
protected:
    ActorList_t::const_iterator mI;
    ActorList_t::const_iterator mE;
    size_t mSize;
public:
    ActorListFilter(ActorList_t const& actorlist)
    {
        mI=actorlist.begin();
        mE=actorlist.end();
        mSize=actorlist.size();
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
    size_t size()
    {
        return mSize;
    }
};

template<>
class ActorListFilter<1>
{
public:
    typedef ActorList_t::nth_index<1>::type::const_iterator const_iterator;
protected:
    const_iterator mI;
    const_iterator mE;
    size_t mSize;
public:
    ActorListFilter(ActorList_t const& actorlist)
    {
        boost::tie(mI,mE)=actorlist.get<1>().equal_range(boost::make_tuple(true));
        mSize=std::distance(mI,mE);
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
    size_t size()
    {
        return mSize;
    }
};

template<>
class ActorListFilter<2>
{
public:
    typedef ActorList_t::nth_index<2>::type::const_iterator const_iterator;
protected:
    const_iterator mI;
    const_iterator mE;
    size_t mSize;
public:
    ActorListFilter(ActorList_t const& actorlist, CollisionClass::Type collisionClass)
    {
        boost::tie(mI,mE)=actorlist.get<2>().equal_range(boost::make_tuple(int32_t(collisionClass)));
        mSize=std::distance(mI,mE);
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
    size_t size()
    {
        return mSize;
    }

};

class Scene : public platform::Singleton<Scene>
{
    ActorHolder mActorHolder;
    typedef std::list< Opt<Actor> > NewActorList_t;
    NewActorList_t mNewActors;
    glm::vec4 mDimensions;
    int32_t mTypeId;
    friend class platform::Singleton<Scene>;
    Scene();
    bool mPaused;
    ModelValue mSceneModel;
    ModelValue mLoadModel;
    ModelValue mPauseModel;
    ModelValue mResumeModel;
    boost::ptr_vector<ModelValue> mPlayerModels;
    ModelValue mPlayerModel;
    int32_t mMaxHP;
    core::ProgramState& mProgramState;
public:
    enum ActorIndex
    {
        All=0,
        RenderableActors,
        CollisionClassActors
    };
    ~Scene();
    void SetType( std::string const& Type );
    int32_t GetTypeId()const;
    void Update( double DeltaTime );
    void AddActor( Actor* Object );
    void RemoveActor( int32_t guid );
    void RemoveActor( ActorList_t::iterator it );
    glm::vec4 const& GetDimensions();
    Opt<Actor> GetActor(int32_t guid);
    void SetPlayerModels(Opt<Actor> actor);
    template<typename MODIFIER>
    void ModifyActor(Actor* Obj, MODIFIER const& Modifier)
    {
        ActorList_t::iterator it = mActorHolder.mAllActors.find(Obj->GetGUID());
        mActorHolder.mAllActors.modify(it,Modifier);
    }
    ActorList_t const& GetActors() const
    {
        return mActorHolder.mAllActors;
    }
    ActorList_t& GetActors()
    {
        return mActorHolder.mAllActors;
    }
    //the template version works well with '=' i just dont know is it really needed, maybe this creating a wrapper is better

//     template<int N>
//     ActorListWrapper<N> GetActors() const
//     {
//         return ActorListWrapper<N>(mActorHolder.mAllActors);
//     }
    void Load( std::string const& Level );

    void AddTestCreep(double X, double Y);

    void Pause()
    {
        mPaused = true;
    }
    void Resume()
    {
        mPaused = false;
    }
    bool IsPaused()const
    {
        return mPaused;
    }
};

#endif//INCLUDED_CORE_SCENE_H
