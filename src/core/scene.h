#ifndef INCLUDED_CORE_SCENE_H
#define INCLUDED_CORE_SCENE_H

class Scene : public Singleton<Scene>
{
    ActorList_t mAllActors;
    ActorList_t mNewActors;
    CollisionStore& mCollisionStore;
    glm::vec4 mDimensions;
    int32_t mTypeId;
    friend class Singleton<Scene>;
    Scene();
    bool mPaused;
    ModelValue mSceneModel;
    ModelValue mLoadModel;
    ModelValue mPauseModel;
    ModelValue mResumeModel;
    Grid mCollisionGrid;
public:
    ~Scene();
    void SetType( std::string const& Type );
    int32_t GetTypeId()const;
    void Update( double DeltaTime );
    void AddActor( Actor* Object );
    glm::vec4 const& GetDimensions();
    ActorList_t const& GetActors()const
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
