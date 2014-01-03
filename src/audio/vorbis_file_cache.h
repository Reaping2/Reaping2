#ifndef INCLUDED_AUDIO_VORBIS_FILE_CACHE_H
#define INCLUDED_AUDIO_VORBIS_FILE_CACHE_H

class VorbisFileCache : public Cache<VorbisFile>, public Singleton<VorbisFileCache>
{
	AutoReg mPhaseChangeId;
	boost::mutex mLoadingElemsMtx;
	boost::thread mLoaderThread;
	typedef std::vector<Elem> LoadingElems_t;
	LoadingElems_t mLoadingElems;
	LoadingElems_t mNewLoadables;
	boost::atomic<bool> mClosing;
	void LoaderThread();
	Elem LoadImpl(const boost::filesystem::path& Source);
	void OnPhaseChangedEvent(PhaseChangedEvent const& Evt);
	friend class Singleton<VorbisFileCache>;
	VorbisFileCache();
};

#endif//INCLUDED_AUDIO_VORBIS_FILE_CACHE_H
