#include "Platform/IO/FileSystem/File.h"

#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>

std::string File::Read(const std::string& Path) {
    std::filesystem::path FilePath = Path;
    std::ifstream file(FilePath);
    std::stringstream buffer;

    if (!std::filesystem::exists(FilePath)) {
        std::println("Error: {} does not exist", Path);
        return "";
    }

    if (!std::filesystem::is_regular_file(FilePath)) {
        std::println("Error: {} is a directory", Path);
        return "";
    }

    if (file.is_open()) {
        std::println("File {} found", Path);
        buffer << file.rdbuf();
        return buffer.str();
    }

    return "";
}

std::vector<std::string> File::ScanFolder(const std::string& Path) {
    std::vector<std::string> FilesInPath;
    std::filesystem::path FolderPath = Path;

    if (!std::filesystem::exists(Path)) {
        std::println("Error: Folder {} does not exist", Path);
    }

    if (std::filesystem::is_directory(FolderPath)) {
        for (const std::filesystem::directory_entry& entry: std::filesystem::recursive_directory_iterator(FolderPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                std::println("File found: {}", entry.path().string());
                FilesInPath.push_back(entry.path().string());
            }
        }
    }

    return FilesInPath;
}