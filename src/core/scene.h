#ifndef INCLUDED_CORE_SCENE_H
#define INCLUDED_CORE_SCENE_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "core/i_core.h"
#include "core/i_renderable_component.h"

int32_t RenderableOrderer(const Opt<Actor>& Obj);
int32_t ActorDefaultOrderer(const Opt<Actor>& Obj);
typedef multi_index_container<
    Opt<Actor>,
    indexed_by<
        ordered_unique<
            global_fun<const Opt<Actor>&, int32_t, ActorDefaultOrderer>
        >,
        ordered_non_unique<
            global_fun<const Opt<Actor>&, int32_t, RenderableOrderer>
        >
    >
> ActorList_t;
typedef ActorList_t::nth_index<1>::type  ActorListRenderableComponent_t;

class Scene : public platform::Singleton<Scene>
{
    ActorList_t mAllActors;
    typedef std::list< Opt<Actor> > NewActorList_t;
    NewActorList_t mNewActors;
    CollisionStore& mCollisionStore;
    glm::vec4 mDimensions;
    int32_t mTypeId;
    friend class platform::Singleton<Scene>;
    Scene();
    bool mPaused;
    ModelValue mSceneModel;
    ModelValue mLoadModel;
    ModelValue mPauseModel;
    ModelValue mResumeModel;
    Grid mCollisionGrid;
public:
    enum ActorIndex
    {
        All,
        RenderableComponents
    };
    ~Scene();
    void SetType( std::string const& Type );
    int32_t GetTypeId()const;
    void Update( double DeltaTime );
    void AddActor( Actor* Object );
    glm::vec4 const& GetDimensions();
    template<typename MODIFIER>
    void ModifyActor(Actor* Obj, MODIFIER& Modifier)
    {
        ActorList_t::iterator it = mAllActors.find(Obj->GetGUID());
        mAllActors.modify(it,Modifier);
    }
    ActorList_t const& GetActors() const
    {
        return mAllActors;
    }
    void Load( std::string const& Level );
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
