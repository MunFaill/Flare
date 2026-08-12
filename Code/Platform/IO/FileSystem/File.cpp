#include "Platform/IO/FileSystem/File.h"

#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

std::string ReadFile(const std::string& FilePath) {
    std::filesystem::path filePath = FilePath;
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (!std::filesystem::exists(filePath)) {
        std::println("Error: {} does not exist", FilePath);
        return "";
    }

    if (!std::filesystem::is_regular_file(filePath)) {
        std::println("File {} is a directory", FilePath);
        return "";
    }

    if (file.is_open()) {
        std::println("File {} found", FilePath);
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "";
}

std::vector<std::string> ScanFiles(const std::string& FolderPath) {
    std::vector<std::string> FilesInPath;
    std::filesystem::path folderPath = FolderPath;

    if (!std::filesystem::exists(folderPath)) {
        std::println("Error: Folder {} does not exist", FolderPath);
    }

    if (std::filesystem::is_directory(folderPath)) {
        for (const std::filesystem::directory_entry& entry: std::filesystem::recursive_directory_iterator(folderPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                std::println("File found: {}", entry.path().string());
                FilesInPath.push_back(entry.path().string());
            }
        }
    }

    return FilesInPath;
}
