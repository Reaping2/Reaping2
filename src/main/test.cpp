#include "platform/i_platform.h"
#include "ui/i_ui.h"
#include "audio/i_audio.h"

namespace {
struct Tester_t
{

    typedef std::vector<boost::filesystem::path> PathVector;
    void BuildFilesList( boost::filesystem::path Dir, PathVector& Elems )
    {
        namespace fs = boost::filesystem;
        if( fs::exists( Dir ) && fs::is_directory( Dir ) )
        {
            for( boost::filesystem::directory_iterator dir_iter( Dir ), end_iter; dir_iter != end_iter; ++dir_iter )
            {
                if( fs::is_regular_file( dir_iter->status() ) )
                {
                    Elems.push_back( *dir_iter );
                }
                else if( fs::is_directory( dir_iter->status() ) )
                {
                    BuildFilesList( *dir_iter, Elems );
                }
            }
        }
    }

    void BuildPackage()
    {
        AutoFile f( new OsFile( "data_new.pkg", std::ios_base::out | std::ios_base::trunc ) );
        PackageWriter writer( f );
        writer.Add( "c:/setuplogfile.log", "setup.log" );
        PathVector Paths;
        BuildFilesList( "data", Paths );
        for( PathVector::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
        {
            writer.Add( *i, *i );
        }
        writer.Save();
    }

    void TestCompression()
    {
        std::string InText = "The quick brown fox jumped over the lazy dog.";
        std::string OutText;
        Compression::Get().Deflate( OutText, InText );
        std::string CheckBack;
        Compression::Get().Inflate( CheckBack, OutText );
    }

    void TestLibPng()
    {
        OsFile Png( "face.png" );
        PngTexture Texture( Png );
        if( Texture.IsValid() )
        {

        }
    }

    void TestJson()
    {
        std::unique_ptr<File> JsonFile = std::move( mPackage->Open( "textures.json" ) );
        if( !JsonFile.get() )
        {
            return;
        }
        JsonReader Reader( *JsonFile );
        if( Reader.IsValid() )
        {

        }
    }

    void TestUI()
    {
    }

    void RegTestEmpty()
    {
        // pass
    }

    void RegTestUnreg()
    {
        mReg[0].Unregister();
        mReg[4].Unregister();
        mReg[2].Unregister();
        mReg[3].Unregister();
        mReg[1].Unregister();
    }

    static const size_t mNumRegs = 5;
    Registration mReg[mNumRegs];
    void TestRegistrations()
    {
        for( size_t i = 0; i < mNumRegs; ++i )
        {
            //TODO: fix it
            //             mReg[i] = TimerServerSystem::Get().AddTimer( i == 2 ? boost::bind( &Tester_t::RegTestUnreg, this ) :
            //                                                    boost::bind( &Tester_t::RegTestEmpty, this ), 1. );
        }
    }

    void TestVorbisFile()
    {
        VorbisFileCache::Get().Load( "sounds/Ping-da-ding-ding-ding.ogg" );
        VorbisFileCache::Get().Load( "sounds/Zap_Beat.ogg" );
    }

    void TestTgaPerf()
    {
        /*          AutoFile t1=mPackage->Open("test/desert.tga");
                    AutoFile t2=mPackage->Open("test/poks.tga");
                    AutoFile p1=mPackage->Open("test/desert.png");
                    AutoFile p2=mPackage->Open("test/poks.png");
                    if(!t1.get()||!t2.get()||!p1.get()||!p2.get())return;
                    size_t const Times=100;
                    Timer_t Perf;
                    for(size_t i=0;i<Times;++i)
                    {
                        TgaTexture tt1(*t1);
                        TgaTexture tt2(*t2);
                        t1->SetPosition(0);
                        t2->SetPosition(0);
                    }
                    Perf.Log("TGA done");
                    for(size_t i=0;i<Times;++i)
                    {
                        PngTexture pt1(*p1);
                        PngTexture pt2(*p2);
                        p1->SetPosition(0);
                        p2->SetPosition(0);
                    }
                    Perf.Log("PNG done");
                    // TGA is approximately 10 times faster*/
    }

    void TestMain()
    {
        //          TestUI();
        //          TestCompression();
        //          BuildPackage();
        //          AutoFile F=mPackage->Open("data/Ping-da-ding-ding-ding.ogg");
        //          std::string Buffer;
        //          if(F.get())
        //              F->ReadAll(Buffer);
        //          TestLibPng();
        //          TestJson();
        //          TestRegistrations();
        //          TestVorbisFile();
        //          TestTgaPerf();
    }

    std::auto_ptr<Package> mPackage;
    Tester_t()
        : mPackage( new Package( AutoFile( new OsFile( "data.pkg" ) ) ) )
    {
        TestMain();
        mPackage.reset();
    }
};

Tester_t Tester;
}
