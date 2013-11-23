#include "i_audio.h"

VorbisFileCache::Elem VorbisFileCache::LoadImpl(const boost::filesystem::path& Source)
{
	VorbisFileCache::Elem Vf(VorbisFile::Create(Filesys::Get().Open(Source)).release());
	if(Vf.get()&&!Vf->IsFinishedLoading())
	{
		boost::mutex::scoped_lock Lock(mLoadingElemsMtx);
		mNewLoadables.push_back(Vf);
	}
	return Vf;
}

void VorbisFileCache::LoaderThread()
{
	while(!mClosing)
	{
		{
			boost::mutex::scoped_lock Lock(mLoadingElemsMtx);
			std::copy(mNewLoadables.begin(),mNewLoadables.end(),std::back_insert_iterator<LoadingElems_t>(mLoadingElems));
			mNewLoadables.clear();
		}
		for(LoadingElems_t::iterator i=mLoadingElems.begin();i!=mLoadingElems.end();)
		{
			Elem& Vf(*i);
			boost::mutex::scoped_lock Lock(Vf->GetMutex());
			Vf->ReadBlock();
			if(Vf->IsFinishedLoading())
				i=mLoadingElems.erase(i);
			else
				++i;
		}
		if(mLoadingElems.empty())
			boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
	}
}

VorbisFileCache::VorbisFileCache()
:mClosing(false)
{
	mLoaderThread=boost::thread(boost::bind(&VorbisFileCache::LoaderThread,this));
	mPhaseChangeId=EventServer<PhaseChangedEvent>::Get().Subscribe(boost::bind(&VorbisFileCache::OnPhaseChangedEvent,this,_1));
}

void VorbisFileCache::OnPhaseChangedEvent( PhaseChangedEvent const& Evt )
{
	if(Evt.CurrentPhase==ProgramPhase::CloseSignal)
	{
		mClosing=true;
		mLoaderThread.join();
	}
}
