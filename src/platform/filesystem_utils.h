#ifndef INCLUDED_PLATFORM_FILESYSTEM_UTILS_H
#define INCLUDED_PLATFORM_FILESYSTEM_UTILS_H

#include "jsonreader.h"
#include <boost/filesystem/path.hpp>

namespace fs_utils {

typedef std::function<void( Json::Value const& desc )> DescFunc_t;
void for_each( std::string const& path, std::string const& extension, DescFunc_t&& func);

typedef std::function<void( Json::Value const& desc, boost::filesystem::path const& path )> DescPathFunc_t;
void for_each( std::string const& path, std::string const& extension, DescPathFunc_t&& func );

} // namespace fs_utils

#endif//INCLUDED_PLATFORM_FILESYSTEM_UTILS_H
