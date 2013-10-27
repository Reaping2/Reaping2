#include "platform/i_platform.h"

namespace{
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

	void TestMain()
	{
		TestCompression();
		BuildPackage();
		Package Pkg(AutoFile(new OsFile("data.pkg")));
		AutoFile F=Pkg.Open("data/Ping-da-ding-ding-ding.ogg");
		std::string Buffer;
		if(F.get())
			F->ReadAll(Buffer);
	}

	struct Tester_t{
		Tester_t()
		{
			TestMain();
		}
	};

	Tester_t Tester;
}