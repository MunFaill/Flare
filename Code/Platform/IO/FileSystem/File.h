#pragma once

#include <vector>
#include <string>

std::string ReadFile(const std::string& FilePath);
std::vector<std::string> ScanFiles(const std::string& FolderPath);
