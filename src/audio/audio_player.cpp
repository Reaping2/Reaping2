#include "i_audio.h"

int AudioPlayer::PlayCallback(const void *, void *OutputBuffer,
								unsigned long FramesPerBuffer,
								const PaStreamCallbackTimeInfo*,
								PaStreamCallbackFlags,
								void *UserData )
{
	return Get().PlayCallbackImpl(OutputBuffer);
}

void AudioPlayer::Play(boost::filesystem::path const& Path)
{
	std::auto_ptr<VorbisFile> F=VorbisFile::Create(Filesys::Get().Open(Path));
	if(!F.get())
		return;
	boost::mutex::scoped_lock g(mReadMtx);
	mNewFiles.push_back(F.release());
}

void AudioPlayer::PlayThread()
{
	do{
		PaError err=Pa_Initialize();
		if(err!=paNoError)break;

		mOutputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (mOutputParameters.device == paNoDevice)break;

		mOutputParameters.channelCount=mNumChannels; /* stereo output */
		mOutputParameters.sampleFormat=paFloat32;
		mOutputParameters.suggestedLatency=Pa_GetDeviceInfo(mOutputParameters.device)->defaultLowOutputLatency;
		mOutputParameters.hostApiSpecificStreamInfo = NULL;

		err=Pa_OpenStream(
			&mStream,
			NULL,/* no input */
			&mOutputParameters,
			mPreferredSampleRate,
			mFramesPerBuffer,
			paClipOff, /* we won't output out of range samples so don't bother clipping them */
			AudioPlayer::PlayCallback,
			NULL);
		if(err!=paNoError)break;
		if(!mStream)break;
		err=Pa_StartStream(mStream);
		if(err!=paNoError)break;

		while((err=Pa_IsStreamActive(mStream))==1)Pa_Sleep(100);
		if(err<0)break;

		err=Pa_CloseStream(mStream);
	}while(false);

	Pa_Terminate();
}

bool AudioPlayer::IsSampleRateSupported( int32_t Rate ) const
{
	return std::find(mSupportedSampleRates.begin(),mSupportedSampleRates.end(),Rate)!=mSupportedSampleRates.end();
}

AudioPlayer::AudioPlayer()
: mPreferredSampleRate(44100)
, mFramesPerBuffer(512)
, mClosing(false)
, mNumChannels(2)
, mBuffer(mNumChannels)
{
	mSupportedSampleRates.push_back(mPreferredSampleRate);	// cd qual only, sry :(
	mPhaseChangeId=EventServer<PhaseChangedEvent>::Get().Subscribe(boost::bind(&AudioPlayer::OnPhaseChangedEvent,this,_1));
	mReadThread=boost::thread(boost::bind(&AudioPlayer::ReadThread,this));
	mPlaybackThread=boost::thread(boost::bind(&AudioPlayer::PlayThread,this));
}

int AudioPlayer::PlayCallbackImpl( void *OutputBuffer )
{
	size_t Ctr=0;
	float *WritePointer = (float*)OutputBuffer;
	do{
		boost::mutex::scoped_lock g(mPlayMtx);
		AudioBuffer::Buffer_t const& Left=mBuffer.GetData(0);
		AudioBuffer::Buffer_t const& Right=mBuffer.GetData(1);
		size_t const Avail=std::min<size_t>(mFramesPerBuffer,Left.size());
		if(!Avail)break;
		float const* LeftPtr=&*Left.begin();
		float const* RightPtr=&*Right.begin();
		for(;Ctr<Avail;++Ctr)
		{
			*WritePointer++=*LeftPtr++;
			*WritePointer++=*RightPtr++;
		}
		mBuffer.Read(Avail);
	}while(false);
	for(;Ctr<mFramesPerBuffer;++Ctr)
	{
		*WritePointer++=0;
		*WritePointer++=0;
	}
	return GetStatus();
}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::OnPhaseChangedEvent( PhaseChangedEvent const& Evt )
{
	if(Evt.CurrentPhase==ProgramPhase::CloseSignal)
	{
		{
			boost::mutex::scoped_lock g(mCloseMtx);
			mClosing=true;
		}
		mPlaybackThread.join();
		mReadThread.join();
	}
}

int AudioPlayer::GetStatus()
{
	boost::mutex::scoped_lock g(mCloseMtx);
	return mClosing?paComplete:paContinue;
}

void AudioPlayer::ReadThread()
{
	while(GetStatus()==paContinue)
	{
		bool Dirty=false;
		{	// merge new files
			boost::mutex::scoped_lock g(mReadMtx);
			Dirty=!mNewFiles.empty()||Dirty;
			mActiveFiles.transfer(mActiveFiles.end(),mNewFiles);
		}
		size_t MinCommonSize=std::numeric_limits<size_t>::max();
		for(AudioFiles_t::iterator i=mActiveFiles.begin(),e=mActiveFiles.end();i!=e;++i)
		{	// preload active files
			VorbisFile& f=*i;
			Dirty=f.FillBufferIfNeeded()||Dirty;
			MinCommonSize=std::min<size_t>(MinCommonSize,f.GetBuffer().GetSize());
		}
		if(!Dirty)
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
			continue;
		}
		{
			boost::mutex::scoped_lock g(mPlayMtx);
			for(AudioFiles_t::iterator i=mActiveFiles.begin(),e=mActiveFiles.end();i!=e;++i)
			{
				VorbisFile& f=*i;
				mBuffer.Write(f.GetBuffer(),MinCommonSize);
			}
		}
		for(AudioFiles_t::iterator i=mActiveFiles.begin();i!=mActiveFiles.end();)
		{
			VorbisFile& f=*i;
			if(f.IsFinished())
				i=mActiveFiles.erase(i);
			else
				++i;
		}
	}
}
