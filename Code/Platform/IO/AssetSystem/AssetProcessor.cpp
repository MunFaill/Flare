#include "Platform/IO/AssetSystem/Asset.h"
#include "Platform/IO/FileSystem/File.h"
#include "Renderer/Device/DeviceBuffers.h"

#include <Renderer/Device/Device.h>
#include <cstdint>
#include <memory>
#include <print>
#include <sstream>
#include <string>
#include <filesystem>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

// Helpers
enum class ShaderType {
    None = -1, Vertex = 0, Fragment = 1
};

std::string GetAssetName(const std::string& Path) {
    return std::filesystem::path(Path).stem().string();
}

void AssetProcessor(const std::vector<std::string>& Files) {
    for (const std::string& FilePath : Files) {
        if (FilePath.ends_with(".shader")) {
            std::string Source = ReadFile(FilePath);

            std::string VertexSource;
            std::string FragmentSource;

            std::stringstream ss(Source);
            std::string Line;
            ShaderType CurrentType = ShaderType::None;

            while (std::getline(ss, Line)) {
                if (Line.find("#[VERTEX]") != std::string::npos) {
                    CurrentType = ShaderType::Vertex;
                } else if (Line.find("#[FRAGMENT]") != std::string::npos) {
                    CurrentType = ShaderType::Fragment;
                } else if (CurrentType != ShaderType::None) {
                    if (CurrentType == ShaderType::Vertex) {
                        VertexSource += Line + "\n";
                    } else if (CurrentType == ShaderType::Fragment) {
                        FragmentSource += Line + "\n";
                    }
                }
            }
            
            std::unique_ptr<Shader> shader = Shader::Create();
            shader->SendData(VertexSource.c_str(), FragmentSource.c_str());
            Assets::Shaders.Add(GetAssetName(FilePath), std::move(shader));
        }

        else if (FilePath.ends_with(".glb") || FilePath.ends_with(".gltf")) {
            cgltf_options options = {};
            cgltf_data* data = nullptr;

            cgltf_result result = cgltf_parse_file(&options, FilePath.c_str(), &data);
            if (result != cgltf_result_success) {
                continue;
            }

            result = cgltf_load_buffers(&options, data, FilePath.c_str());
            if (result != cgltf_result_success) {
                cgltf_free(data);
                continue;
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
                        } else if (attr.type == cgltf_attribute_type_normal) {
                            for (cgltf_size i = 0; i < numVertices; ++i) {
                                cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].Normal.x, 3);
                            }
                        } else if (attr.type == cgltf_attribute_type_texcoord) {
                            for (cgltf_size i = 0; i < numVertices; ++i) {
                                cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].TexCoord.x, 2);
                            }
                        } else if (attr.type == cgltf_attribute_type_color) {
                            for (cgltf_size i = 0; i < numVertices; ++i) {
                                cgltf_accessor_read_float(accessor, i, &vertices[baseIndex + i].Color.r, 4);
                            }
                        }
                    }

                    if (primitive.indices) {
                        const cgltf_accessor* indexAccessor = primitive.indices;
                        cgltf_size indexCount = indexAccessor->count;

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

                Assets::Meshes.Add(GetAssetName(FilePath), std::move(mesh));
            }

            cgltf_free(data);
        }
        std::println("{}: Parsed", FilePath);
    }

}