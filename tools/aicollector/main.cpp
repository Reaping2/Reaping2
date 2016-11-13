#include <json/json.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <algorithm>
#include <regex>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

// append autoids to the given vector
// - recursively parses a json "tree" and stores all values of string type objects
// - strings with leading '%' are stored without '%' as well
void parseJSON(Json::Value& root, std::set<std::string>& autoids)
{
    for ( auto it = root.begin(), e = root.end(); it != e; ++it)
    {
        if ( (*it).isString() )
        {
            std::string id((*it).asString());
            autoids.insert(id);
            // insert it without %% as well
            id.erase(std::remove(id.begin(), id.end(),'%'), id.end());
            autoids.insert(id);
        }
        parseJSON(*it, autoids);
    }
}

// append autoids to the given vector
// - collects string literals from AutoId calls
// - collects string literals from macros that use AutoId
// possible issues:
// - multiplie AutoId expressions in 1 line are not parsed
void parseCpp(std::string& content, std::set<std::string>& autoids)
{
    static std::array<std::string,7> macros = {
        "DEFINE_BUFF_BASE",
        "DEFINE_COMPONENT_BASE",
        "DEFINE_ROOM_PROPERTY",
        "DEFINE_MAP_ELEMENT_BASE",
        "DEFINE_SUB_SYSTEM_BASE",
        "DEFINE_SYSTEM_BASE",
        "DEFINE_MESSAGE_BASE" };
    // the matching group (1) contains the actual string
    static std::regex autoid_regex("AutoId\\(\\s*\"(\\S+)\"\\s*\\)", std::regex_constants::ECMAScript | std::regex_constants::icase);
    auto aid_begin = std::sregex_iterator(content.begin(), content.end(), autoid_regex);
    auto aid_end = std::sregex_iterator();
    for ( auto it = aid_begin; it != aid_end; ++it )
    {
        std::smatch match = *it;
        //std::cout << "full: " << match.str() << ", group: " << match.str(1) << std::endl;
        autoids.insert(match.str(1));
    }
    // todo macros:
    // define_buff_base
    // define_component_base
    // define_room_property_base
    // define_map_element_base
    // define_sub_system_base
    // define_system_base
    // define_message_base
    for ( auto m : macros )
    {
        //std::cout << "matching for macro " << m << "..." << std::endl;
        std::regex macro_regex( m+"\\(\\s*(\\S+)\\s*\\)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        auto m_begin = std::sregex_iterator(content.begin(), content.end(), macro_regex);
        auto m_end = std::sregex_iterator();
        for ( auto it = m_begin; it != m_end; ++it )
        {
            std::smatch match = *it;
            //std::cout << "full: " << match.str() << ", group: " << match.str(1) << std::endl;
            autoids.insert(match.str(1));
        }
    }
}

int main( int argc, char** argv)
{
    std::string path(""), output("");
    std::vector<std::string> ext;
    bool enableCpp(false), enableJSON(false);
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help","prints help")
        ("path",po::value<std::string>(&path),"path to files")
        ("ext", po::value< std::vector<std::string> >(&ext)->multitoken(),"extension of files, use comma separated list for multiple extensions")
        ("enable-cpp", "enable CPP parsing")
        ("enable-json", "enable JSON parsing")
        ("output-file", po::value<std::string>(&output),"name of the output file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc,argv,desc),vm);
    po::notify(vm);
    if (vm.count("help"))
    {
        std::cout << "This program collects autoids from files in a directory recursively." << std::endl
                  << "Files can be interpreted as either source files or content files, so enable-cpp and enable-json are exclusive." << std::endl
                  << desc << std::endl;
        return 1;
    }
    if (vm.count("path") == 0)
    {
        std::cout <<"no path given" << std::endl;
        return 1;
    }
    if (vm.count("output-file") == 0)
    {
        std::cout << "no output file given" << std::endl;
        return 1;
    }
    enableCpp = vm.count("enable-cpp") > 0;
    enableJSON = vm.count("enable-json") > 0;
    if ( enableCpp && enableJSON )
    {
        std::cout << "enable-cpp and enable-json are exclusive, but you enabled both" << std::endl;
        return 1;
    }
    if ( !enableCpp && !enableJSON )
    {
        std::cout << "you must enable either cpp or json parsing" << std::endl;
        return 1;
    }
    if (vm.count("ext") == 0)
    {
        std::cout << "no extension given" << std::endl;
        if (enableCpp)
        {
            std::cout << "defaulting to .cpp,.h" << std::endl;
            ext.push_back(".cpp");
            ext.push_back(".h");
        }
        else
        {
            std::cout << "defaulting to .json" << std::endl;
            ext.push_back(".json");
        }
    }
    //std::cout << "supported extensions:" << std::endl;
    // auto prepend '.' : cpp -> .cpp
    for ( std::string& s : ext )
    {
        if ( s[0] != '.' )
        {
            s = '.'+s;
        }
        //std::cout << s << std::endl;
    }
    /*
    std::string e = ext.front();
    e = std::accumulate(std::next(ext.begin()), ext.end(),e, [](std::string& s, std::string& ss){ return s+","+ss;});
    std::cout << "called with" << std::endl
              << "path:" << path << std::endl
              << "ext:" << e << std::endl
              << "enable-cpp" << enableCpp << std::endl
              << "enable-json" << enableJSON << std::endl
              << "output " << output << std::endl;*/
    if (!fs::exists(path) || !fs::is_directory(path))
    {
        std::cout << path << "does not exist or not a directory" << std::endl;
        return 1;
    }
    std::set<std::string> autoids;
    for ( fs::recursive_directory_iterator it(path), eit; it != eit; ++it )
    {
        if ( fs::is_regular_file(*it) )
        {
            //std::cout << "checking file " << it->path().filename() << " extesion :" << it->path().extension() << std::endl;
            if ( std::find(ext.begin(), ext.end(), it->path().extension()) == ext.end() )
            {
                //std::cout << "not supported extension " << it->path().extension() << std::endl;
                continue;
            }
            if (enableJSON)
            {
                fs::ifstream in(it->path());
                Json::Value root;
                in >> root;
                parseJSON(root, autoids);
            }
            else
            {
                //std::cout << "checking " << it->path() << std::endl;
                fs::ifstream in(it->path());
                std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
                //std::cout << "content " << content << std::endl;
                parseCpp(content, autoids);
            }
        }
    }
    fs::ofstream out(output);
    for (auto autoid:autoids)
    {
        out << autoid << std::endl;
    }
    return 0;
}
