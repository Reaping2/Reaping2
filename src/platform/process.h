#pragma once
#ifndef INCLUDED_PROCESS_H
#define INCLUDED_PROCESS_H

#include <vector>
#include <string>

namespace platform {
namespace process {
bool Start( std::string const& command, std::vector<std::string> const& arguments );
bool Start( std::wstring const& command, std::vector<std::wstring> const& arguments );
}
}

#endif // INCLUDED_PROCESS_H

