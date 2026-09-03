#pragma once

#include <vector>
#include <string>

struct File {
    static std::string Read(const std::string& Path);
    static std::vector<std::string> ScanFolder(const std::string& Path);
};