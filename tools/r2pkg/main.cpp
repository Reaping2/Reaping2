#include "platform/i_platform.h"
#include <boost/program_options.hpp>

namespace fs = boost::filesystem;

typedef std::vector<fs::path> PathVector;
void BuildFilesList( fs::path Dir, PathVector& Elems )
{
    if( !fs::exists( Dir ) || !fs::is_directory( Dir ) )
    {
        return;
    }
    for( fs::recursive_directory_iterator dir_iter( Dir ), end_iter; dir_iter != end_iter; ++dir_iter )
    {
        if( !fs::is_regular_file( dir_iter->status() ) )
        {
            continue;
        }
        const fs::path& p = *dir_iter;
        Elems.push_back( p );
    }
}

fs::path RelativePath( const fs::path& Base, const fs::path& Target )
{
    // canonical: paths must exists (this is quaranteed in this app)
    fs::path BaseCanon = fs::canonical( Base / "" );
    fs::path TargetCanon = fs::canonical( Target );
    fs::path Output;

    assert( TargetCanon.native().compare( 0, BaseCanon.native().size(), BaseCanon.native() ) == 0 );
    while( !TargetCanon.empty() && !fs::equivalent( TargetCanon, BaseCanon ) )
    {
        Output = TargetCanon.filename() / Output;
        TargetCanon = TargetCanon.parent_path();
    }

    return Output;
}

void BuildPackage( const std::string& FolderName, std::string FileName )
{
    if( FileName.empty() )
    {
        fs::path Path( FolderName );
        FileName = Path.filename().string() + ".pkg";
    }
    AutoFile f( new OsFile( FileName, std::ios_base::out | std::ios_base::trunc ) );
    PackageWriter writer( f );
    PathVector Paths;
    fs::path Dir( FolderName );
    BuildFilesList( Dir, Paths );
    fs::path BasePath = Dir.is_absolute() ? Dir : fs::current_path() / FolderName;
    for( PathVector::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        const fs::path& RelPath = *i;
        const fs::path PathInPack = RelativePath( BasePath, RelPath );
        LOG( "Adding %s as %s\n", RelPath.string().c_str(), PathInPack.string().c_str() );
        writer.Add( RelPath, PathInPack );
    }
    writer.Save();
    LOG( "All done." );
}

void ExtractPackage( const std::string& PackageName, std::string FolderName )
{
    if( FolderName.empty() )
    {
        fs::path Path( PackageName );
        FolderName = Path.stem().string();
    }
    Package Pkg( AutoFile( new OsFile( PackageName ) ) );
    typedef std::vector<fs::path> paths_t;
    paths_t IncludedFiles;
    Pkg.GetFileNames( IncludedFiles );
    for( paths_t::const_iterator i = IncludedFiles.begin(), e = IncludedFiles.end(); i != e; ++i )
    {
        fs::path p = ( *i );
        std::unique_ptr<File> f = std::move( Pkg.Open( p ) );
        p = FolderName / p;
        fs::create_directories( p.parent_path() );
        OsFile o( p, std::ios_base::out | std::ios_base::trunc );
        std::string Buffer;
        f->ReadAll( Buffer );
        o.Write( Buffer );
    }
}

int main( int argc, char* argv[] )
{
    namespace po = boost::program_options;
    // Declare the supported options.
    po::options_description desc( "Allowed options" );
    std::string to_compress_dir;
    std::string to_uncompress_file;
    desc.add_options()
    ( "help", "produce help message" )
    ( "-c", po::value<std::string>( &to_compress_dir ), "folder to compress" )
    ( "-x", po::value<std::string>( &to_uncompress_file ), "file to extract" )
    ( "-o", po::value<std::string>(), "destination" )
    ;

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    if ( vm.count( "help" ) )
    {
        std::cout << desc << "\n";
        return 1;
    }

    if ( vm.count( "-c" ) )
    {
        std::cout << "Compressing " << vm["-c"].as<std::string>() << ".\n";
        BuildPackage( vm["-c"].as<std::string>(), vm.count( "-o" ) ? vm["-o"].as<std::string>() : std::string() );
    }
    else if ( vm.count( "-x" ) )
    {
        std::cout << "Extracting from " << vm["-x"].as<std::string>() << ".\n";
        ExtractPackage( vm["-x"].as<std::string>(), vm.count( "-o" ) ? vm["-o"].as<std::string>() : std::string() );
    }
    else
    {
        std::cout << desc << "\n";
    }

    return 0;
}
