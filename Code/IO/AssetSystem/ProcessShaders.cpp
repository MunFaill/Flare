#include "IO/AssetSystem/AssetProcessor.h"
#include "IO/AssetSystem/Assets.h"
#include "IO/FileSystem/File.h"

#include <sstream>
#include <filesystem>
#include <print>

enum class ShaderType {
    None = -1, Vertex = 0, Fragment = 1
};


std::string ResolveShaderIncludes(const std::string& filePath) {
    std::string source = File::Read(filePath);
    std::stringstream ss(source);
    std::stringstream result;
    std::string line;

    std::string directory = std::filesystem::path(filePath).parent_path().string();

    while (std::getline(ss, line)) {
        if (line.rfind("#include", 0) == 0) {
            std::size_t firstQuote = line.find('"');
            std::size_t lastQuote = line.rfind('"');

            if (firstQuote != std::string::npos && lastQuote != std::string::npos && firstQuote < lastQuote) {
                std::string includeFileName = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                std::string fullIncludePath = directory + "/" + includeFileName;

                result << ResolveShaderIncludes(fullIncludePath) << "\n";
            }
        } else {
            result << line << "\n";
        }
    }

    return result.str();
}

void AssetProcessor::ProcessShaders(const std::string& File) {
    std::string Source = ResolveShaderIncludes(File);

    std::string VertexSource;
    std::string FragmentSource;

    std::stringstream ss(Source);
    std::string Line;
    ShaderType CurrentType = ShaderType::None;

    while (std::getline(ss, Line)) {
        if (Line.find("#[VERTEX]") != std::string::npos) {
            CurrentType = ShaderType::Vertex;
        }

        else if (Line.find("#[FRAGMENT]") != std::string::npos) {
            CurrentType = ShaderType::Fragment;
        }

        else if (CurrentType != ShaderType::None) {
            if (CurrentType == ShaderType::Vertex) {
                VertexSource += Line + "\n";
            }

            else if (CurrentType == ShaderType::Fragment) {
                FragmentSource += Line + "\n";
            }
        }

        else {
            std::println("Could not indentify shader portion: {}", Line);
        }
    }

    std::unique_ptr<Shader> shader = Shader::Create();
    shader->SendData(VertexSource.c_str(), FragmentSource.c_str());
    Assets::Shaders.Add(File::GetFileName(File), std::move(shader));
}