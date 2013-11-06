#include "platform/i_platform.h"
#include "ui/i_ui.h"

namespace{
	struct Tester_t{

		typedef std::vector<boost::filesystem::path> PathVector;
		void BuildFilesList(boost::filesystem::path Dir,PathVector& Elems)
		{
			namespace fs=boost::filesystem;
			if(fs::exists(Dir)&&fs::is_directory(Dir))
			{
				for(boost::filesystem::directory_iterator dir_iter(Dir),end_iter;dir_iter!=end_iter;++dir_iter)
				{
					if(fs::is_regular_file(dir_iter->status()))
						Elems.push_back(*dir_iter);
					else if(fs::is_directory(dir_iter->status()))
						BuildFilesList(*dir_iter,Elems);
				}
			}
		}

		void BuildPackage()
		{
			AutoFile f(new OsFile("data_new.pkg",std::ios_base::out|std::ios_base::trunc));
			PackageWriter writer(f);
			writer.Add("c:/setuplogfile.log","setup.log");
			PathVector Paths;
			BuildFilesList("data",Paths);
			for(PathVector::const_iterator i=Paths.begin(),e=Paths.end();i!=e;++i)
				writer.Add(*i,*i);
			writer.Save();
		}

		void TestCompression()
		{
			std::string InText="The quick brown fox jumped over the lazy dog.";
			std::string OutText;
			Compression::Get().Deflate(OutText,InText);
			std::string CheckBack;
			Compression::Get().Inflate(CheckBack,OutText);
		}

		void TestLibPng()
		{
			OsFile Png("face.png");
			PngTexture Texture(Png);
			if(Texture.IsValid())
			{

			}
		}

		void TestJson()
		{
			AutoFile JsonFile=mPackage->Open("textures.json");
			if(!JsonFile.get())return;
			JsonReader Reader(*JsonFile);
			if(Reader.IsValid())
			{

			}
		}

		void TestUI()
		{
			std::auto_ptr<Widget> Root(new Widget);
			for(int i=0;i<10;++i)
			{
				Widget* Itr=new Widget;
				Root->AddChild(Itr);
				for(int j=0;j<5;++j)
					Itr->AddChild(new Widget);
				if(i%2)
					delete Itr;
			}
			Root.reset();
			Root.reset(new Widget);
		}

		void TestMain()
		{
			TestUI();
			TestCompression();
			BuildPackage();
			AutoFile F=mPackage->Open("data/Ping-da-ding-ding-ding.ogg");
			std::string Buffer;
			if(F.get())
				F->ReadAll(Buffer);
			TestLibPng();
			TestJson();
		}

		std::auto_ptr<Package> mPackage;
		Tester_t()
			: mPackage(new Package(AutoFile(new OsFile("data.pkg"))))
		{
			TestMain();
			mPackage.reset();
		}
	};

	Tester_t Tester;
}