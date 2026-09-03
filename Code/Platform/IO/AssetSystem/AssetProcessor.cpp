#include "Platform/IO/AssetSystem/AssetProcessor.h"
#include "Platform/IO/AssetSystem/Assets.h"
#include "Platform/IO/FileSystem/File.h"

#include <cstdint>
#include <memory>
#include <print>
#include <sstream>
#include <filesystem>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

enum class ShaderType {
    None = -1, Vertex = 0, Fragment = 1
};


std::string GetAssetName(const std::string& Path) {
    return std::filesystem::path(Path).stem().string();
}

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

void AssetProcessor::Process(const std::vector<std::string>& Files) {
    for (const std::string& FilePath : Files) {
        if (FilePath.ends_with(".shader")) {
            ProcessShaders(FilePath);
        }

        else if (FilePath.ends_with(".png") || FilePath.ends_with(".jpeg") || FilePath.ends_with(".jpg")) {
            ProcessImages(FilePath);
        }

        else if (FilePath.ends_with(".glb") || FilePath.ends_with(".gltf")) {
            ProcessMeshes(FilePath);
        }
    }
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
    Assets::Shaders.Add(GetAssetName(File), std::move(shader));
}

void AssetProcessor::ProcessImages(const std::string& File) {
    stbi_set_flip_vertically_on_load(true);
    int W, H, Nrc;
    uint8_t* Data = stbi_load(File.c_str(), &W, &H, &Nrc, 4);

    std::unique_ptr<Texture> texture = Texture::Create();
    texture->SendData(Data, W, H);
    stbi_image_free(Data);
    Assets::Textures.Add(GetAssetName(File), std::move(texture));
}

void AssetProcessor::ProcessMeshes(const std::string& File) {
    cgltf_options options = {};
    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, File.c_str(), &data);

    if (result != cgltf_result_success) {
        std::println("Error when parsing file {}", File);
    }

    result = cgltf_load_buffers(&options, data, File.c_str());

    if (result != cgltf_result_success) {
        cgltf_free(data);
        std::println("Error loading buffers from file {}", File);
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (cgltf_size m = 0; m < data->meshes_count; ++m) {
        const cgltf_mesh& mesh = data->meshes[m];

        for (cgltf_size p = 0; p < mesh.primitives_count; ++p) {
            const cgltf_primitive& primitive = mesh.primitives[p];
            uint32_t vertexStart = static_cast<uint32_t>(vertices.size());
            cgltf_size numVertices = 0;

            for (cgltf_size a = 0; a < primitive.attributes_count; ++a) {

                if (primitive.attributes[a].type == cgltf_attribute_type_position) {
                    numVertices = primitive.attributes[a].data->count;
                    break;
                }
            }

            if (numVertices == 0) continue;

            size_t baseIndex = vertices.size();
            vertices.resize(baseIndex + numVertices);

            for (size_t i = baseIndex; i < vertices.size(); ++i) {
                vertices[i].Position = glm::vec3(0.0f);
                vertices[i].Normal   = glm::vec3(0.0f, 1.0f, 0.0f);
                vertices[i].TexCoord = glm::vec2(0.0f);
                vertices[i].Color    = glm::vec4(1.0f);
            }

            for (cgltf_size a = 0; a < primitive.attributes_count; ++a) {
                const cgltf_attribute& attr = primitive.attributes[a];
                const cgltf_accessor* accessor = attr.data;

                if (attr.type == cgltf_attribute_type_position) {

                    for (cgltf_size i = 0; i < numVertices; ++i) {
                        cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].Position.x, 3);
                    }

                } 

                else if (attr.type == cgltf_attribute_type_normal) {
                    for (cgltf_size i = 0; i < numVertices; ++i) {
                        cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].Normal.x, 3);
                    }
                } 

                else if (attr.type == cgltf_attribute_type_texcoord) {
                    for (cgltf_size i = 0; i < numVertices; ++i) {
                        cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].TexCoord.x, 2);
                        vertices[baseIndex + i].TexCoord.y = 1.0f - vertices[baseIndex + i].TexCoord.y;
                    }
                } 
                
                else if (attr.type == cgltf_attribute_type_color) {
                    cgltf_size numComponents = cgltf_num_components(accessor->type);
                    for (cgltf_size i = 0; i < numVertices; ++i) {
                        if (numComponents == 3) {
                            vertices[baseIndex + i].Color.a = 1.0f;
                            cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].Color.r, 3);
                        } else if (numComponents == 4) {
                            cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].Color.r, 4);
                        }
                    }
                }
            }
            
            if (primitive.indices) {
                const cgltf_accessor* indexAccessor = primitive.indices;
                cgltf_size indexCount = indexAccessor->count;
                indices.reserve(indices.size() + indexCount);
                for (cgltf_size i = 0; i < indexCount; ++i) {
                    uint32_t index = static_cast<uint32_t>(cgltf_accessor_read_index(indexAccessor, i));
                    indices.push_back(vertexStart + index);
                }
            }
        }
    }
    if (!vertices.empty()) {
        std::unique_ptr<VertexBuffer> vbo = VertexBuffer::Create();
        vbo->SendData(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Vertex)));
        std::unique_ptr<IndexBuffer> ibo = IndexBuffer::Create();
        if (!indices.empty()) {
            ibo->SendData(indices.data(), static_cast<uint32_t>(indices.size()));
        }
        std::unique_ptr<Mesh> mesh = Mesh::Create();
        mesh->SendData(std::move(vbo), std::move(ibo));
        Assets::Meshes.Add(GetAssetName(File), std::move(mesh));
    }
    cgltf_free(data);
    std::println("{}: Parsed", File);
}