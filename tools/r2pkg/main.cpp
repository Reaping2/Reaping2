#include "platform/i_platform.h"
#include <boost/program_options.hpp>

namespace fs=boost::filesystem;

typedef std::vector<fs::path> PathVector;
void BuildFilesList(fs::path Dir,PathVector& Elems)
{
	if(fs::exists(Dir)&&fs::is_directory(Dir))
	{
		for(fs::directory_iterator dir_iter(Dir),end_iter;dir_iter!=end_iter;++dir_iter)
		{
			if(fs::is_regular_file(dir_iter->status()))
				Elems.push_back(*dir_iter);
			else if(fs::is_directory(dir_iter->status()))
				BuildFilesList(*dir_iter,Elems);
		}
	}
}

void BuildPackage(const std::string& FolderName,std::string FileName)
{
	if(FileName.empty())
	{
		fs::path Path(FolderName);
		FileName=Path.filename().string()+".pkg";
	}
	AutoFile f(new OsFile(FileName,std::ios_base::out|std::ios_base::trunc));
	PackageWriter writer(f);
	PathVector Paths;
	BuildFilesList(FolderName,Paths);
	for(PathVector::const_iterator i=Paths.begin(),e=Paths.end();i!=e;++i)
		writer.Add(*i,*i);
	writer.Save();
}

void ExtractPackage(const std::string& PackageName,std::string FolderName)
{
	if(FolderName.empty())
	{
		fs::path Path(PackageName);
		FolderName=Path.filename().string();
		if(FolderName.size()>4&&FolderName[FolderName.size()-4])
			FolderName=FolderName.substr(0,FolderName.size()-4);
	}
	Package Pkg(AutoFile(new OsFile(PackageName)));
	typedef std::vector<fs::path> paths_t;
	paths_t IncludedFiles;
	Pkg.GetFileNames(IncludedFiles);
	for(paths_t::const_iterator i=IncludedFiles.begin(),e=IncludedFiles.end();i!=e;++i)
	{
		fs::path p=(*i);
		AutoFile f=Pkg.Open(p);
		p=FolderName/p;
		fs::create_directories(p.parent_path());
		OsFile o(p,std::ios_base::out|std::ios_base::trunc);
		std::string Buffer;
		f->ReadAll(Buffer);
		o.Write(Buffer);
	}
}

int main(int argc, char* argv[])
{
	namespace po=boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	std::string to_compress_dir;
	std::string to_uncompress_file;
	desc.add_options()
		(" help", "produce help message")
		(" -c", po::value<std::string>(&to_compress_dir), "folder to compress")
		(" -x", po::value<std::string>(&to_uncompress_file), "file to extract")
		(" -o", po::value<std::string>(), "destination")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	if (vm.count("-c")) {
		std::cout << "Compressing " << vm["-c"].as<std::string>() << ".\n";
		BuildPackage(vm["-c"].as<std::string>(),vm.count("-o")?vm["-o"].as<std::string>():std::string());
	} else if (vm.count("-x")) {
		std::cout << "Extracting from " << vm["-x"].as<std::string>() <<".\n";
		ExtractPackage(vm["-x"].as<std::string>(),vm.count("-o")?vm["-o"].as<std::string>():std::string());
	} else {
		std::cout << desc << "\n";
	}

	return 0;
}