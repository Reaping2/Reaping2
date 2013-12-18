#include "i_core.h"

void Scene::AddActor( Actor* Object )
{
	mNewActors.push_back(*Object);
	// mikor az obj torlodik, magatol kikerul a listabol
}

void Scene::Update( double DeltaTime )
{

	if(IsPaused())return;
	mCollisionGrid.Clear();
	for(ActorList_t::iterator it=mAllActors.begin(),e=mAllActors.end();it!=e;++it)
	{
		Actor& Obj=*it;
		Obj.DoControlling(DeltaTime);
		mCollisionGrid.AddActor(&Obj,DeltaTime);
	}
	PossibleCollisions_t const& PossibleCollisions=mCollisionGrid.GetPossibleCollisions();
	for(PossibleCollisions_t::const_iterator i=PossibleCollisions.begin(),e=PossibleCollisions.end();i!=e;++i)
	{
		Actor& A=*(i->A1);
		Actor& B=*(i->A2);
		CollisionModel const& CollModel=mCollisionStore.GetCollisionModel(A.GetCC(),B.GetCC());
		if(!CollModel.AreActorsColliding(A,B,DeltaTime))continue;
		A.Collide(B);
		B.Collide(A);
	}
	for(ActorList_t::iterator it=mAllActors.begin(),e=mAllActors.end();it!=e;++it)
		it->Update(DeltaTime);
	for(ActorList_t::iterator it=mAllActors.begin(),e=mAllActors.end(),n;(n=it,it!=e?(++n,true):false);it=n)
		it->UpdateLifetime();
	if(!mNewActors.empty())
		mAllActors.splice(mAllActors.end(),mNewActors);
}

Scene::Scene()
: mDimensions(-2,-2,2,2)
, mCollisionStore(CollisionStore::Get())
, mTypeId(0)
, mPaused(true)
, mSceneModel("scene",&RootModel::Get())
, mLoadModel(StringFunc(this,&Scene::Load),"load",&mSceneModel)
, mPauseModel(VoidFunc(this,&Scene::Pause),"pause",&mSceneModel)
, mResumeModel(VoidFunc(this,&Scene::Resume),"resume",&mSceneModel)
{
	mCollisionGrid.Build(mDimensions,0.4f);
}

glm::vec4 const& Scene::GetDimensions()
{
	return mDimensions;
}

Scene::~Scene()
{
	mAllActors.splice(mAllActors.end(),mNewActors);
	while(!mAllActors.empty())
		delete &mAllActors.front();
}

void Scene::SetType( std::string const& Type )
{
	mTypeId=AutoId(Type);
}

int32_t Scene::GetTypeId() const
{
	return mTypeId;
}

void Scene::Load( std::string const& Level )
{
	mPaused=false;
	mAllActors.splice(mAllActors.end(),mNewActors);
	while(!mAllActors.empty())
		delete &mAllActors.front();

	SetType("grass");
	struct point{double x; double y;};
	const size_t NumPoints=5;
	const point points[NumPoints]={
		{-1,-1},{-1,-0.8},{-1,-0.6},{-0.8,-0.6},{-0.6,-0.6},
	};

	for(size_t i=0;i<NumPoints;++i)
	{
		Wall* wall=new Wall("wall");
		wall->SetX(points[i].x);
		wall->SetY(points[i].y);
		AddActor(wall);
	}

	Player* Pl=new Player();
	Pl->SetController(std::auto_ptr<Controller>(new PlayerController));
	AddActor(Pl);

#ifdef DEBUG
	static const size_t BenchmarkCreeps=200;
#else
	static const size_t BenchmarkCreeps=5000;
#endif
	for(size_t i=0;i<BenchmarkCreeps;++i)
	{
		Creep* Obj=new Creep(rand()%2?"pok1":"pok2",
			mDimensions.x+(rand()%(int)(1000*(mDimensions.z-mDimensions.x)))/1000.,
			mDimensions.y+(rand()%(int)(1000*(mDimensions.w-mDimensions.y)))/1000.,
			rand()%2?Pl:(Actor*)NULL);
		AddActor(Obj);
	}
}

