#include "Platform/IO/AssetSystem/AssetProcessor.h"
#include "Platform/IO/AssetSystem/Assets.h"
#include "Platform/IO/FileSystem/File.h"
#include "Renderer/Device/DeviceBuffers.h"
#include "Renderer/Device/DeviceMesh.h"
#include "Renderer/Graphics/Material.h"
#include "Renderer/Graphics/Model.h"

#include <cstdint>
#include <memory>
#include <print>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <utility>
#include <vector>
#include <stb_image.h>

std::unique_ptr<Mesh> CreateMesh(const std::vector<Vertex>& Vertices, const std::vector<uint32_t>& Indices) {
    std::unique_ptr<Mesh> mesh = Mesh::Create();

    std::unique_ptr<VertexBuffer> vbuffer = VertexBuffer::Create();
    std::unique_ptr<IndexBuffer> ibuffer = IndexBuffer::Create();

    vbuffer->SendData(Vertices.data(), Vertices.size() * sizeof(Vertex));
    ibuffer->SendData(Indices.data(), Indices.size());

    mesh->SendData(std::move(vbuffer), std::move(ibuffer));

    return mesh;
}

void ProcessEmbeddedTexture(const aiTexture* SourceTexture, const std::string& TextureID) {
    if (!SourceTexture)
        return;

    int W, H, Nrc;

    uint8_t* Data = stbi_load_from_memory( reinterpret_cast<const uint8_t*>(SourceTexture->pcData), static_cast<int>(SourceTexture->mWidth), &W, &H, &Nrc, 4);

    if (!Data) {
        std::println("Failed to load embedded texture '{}'", TextureID);
        return;
    }

    std::unique_ptr<Texture> texture = Texture::Create();

    texture->SendData(Data, W, H);

    stbi_image_free(Data);

    Assets::Textures.Add(TextureID, std::move(texture));
}

std::string ProcessTexture(const aiScene* Scene, const aiString& TexturePath, const std::string& ModelID) {
    if (TexturePath.C_Str()[0] != '*') {
        return File::GetFileName(TexturePath.C_Str());
    }

    const aiTexture* EmbeddedTexture = Scene->GetEmbeddedTexture(TexturePath.C_Str());

    if (!EmbeddedTexture) {
        return "";
    }

    std::string TextureID;

    if (EmbeddedTexture->mFilename.length > 0 && EmbeddedTexture->mFilename.C_Str()[0] != '*') {
        TextureID = File::GetFileName(EmbeddedTexture->mFilename.C_Str());
    }
    else {
        TextureID = ModelID + "_Embedded_" + std::string(TexturePath.C_Str() + 1);
    }

    if (!Assets::Textures.Has(TextureID)) {
        ProcessEmbeddedTexture(EmbeddedTexture, TextureID);
    }

    return TextureID;
}

void AssetProcessor::ProcessMeshes(const std::string& FilePath) {
    Assimp::Importer Importer;
    const aiScene* Scene = Importer.ReadFile(FilePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);

    if (!Scene || !Scene->mRootNode) {
        std::println("Assimp failed to load '{}': {}", FilePath, Importer.GetErrorString());
        return;
    }

    std::string ModelID = File::GetFileName(FilePath);
    std::unique_ptr<Model> ModelAsset = std::make_unique<Model>();

    std::println("Loaded model '{}': {} meshes, {} materials", FilePath, Scene->mNumMeshes, Scene->mNumMaterials);

    for (unsigned int i = 0; i < Scene->mNumMaterials; ++i) {
        aiMaterial* SourceMaterial = Scene->mMaterials[i];

        std::string MaterialName = "Material_" + std::to_string(i);

        aiString AssimpMaterialName;

        if (SourceMaterial->Get(AI_MATKEY_NAME, AssimpMaterialName) == AI_SUCCESS && AssimpMaterialName.length > 0) {
            MaterialName = AssimpMaterialName.C_Str();
        }

        std::string MaterialID = ModelID + "_" + MaterialName;

        std::unique_ptr<Material> MaterialAsset = std::make_unique<Material>();

        aiColor4D DiffuseColor;

        if (aiGetMaterialColor(SourceMaterial, AI_MATKEY_COLOR_DIFFUSE, &DiffuseColor) == AI_SUCCESS) {
            MaterialAsset->Albedo = {DiffuseColor.r, DiffuseColor.g, DiffuseColor.b, DiffuseColor.a};
        }

        aiString TexturePath;

        if (SourceMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &TexturePath) == AI_SUCCESS) {
            std::println("Material '{}' uses diffuse texture '{}'", MaterialID, TexturePath.C_Str());

            std::string TextureID = ProcessTexture(Scene, TexturePath, ModelID);

            if (!TextureID.empty()) {
                MaterialAsset->DiffuseTextureID = TextureID;
            }
        }

        if (SourceMaterial->GetTexture(aiTextureType_SPECULAR, 0, &TexturePath) == AI_SUCCESS) {
            std::println("Material '{}' uses specular texture '{}'", MaterialID, TexturePath.C_Str());

            std::string TextureID = ProcessTexture(Scene, TexturePath, ModelID);

            if (!TextureID.empty()) {
                MaterialAsset->SpecularTextureID = TextureID;
            }
        }

        Assets::Materials.Add(MaterialID, std::move(MaterialAsset));
    }

    for (unsigned int i = 0; i < Scene->mNumMeshes; i++) {
        aiMesh* SourceMesh = Scene->mMeshes[i];

        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

        Vertices.reserve(SourceMesh->mNumVertices);

        for (unsigned int v = 0; v < SourceMesh->mNumVertices; ++v) {
            Vertex vertex;

            vertex.Position = {
                SourceMesh->mVertices[v].x,
                SourceMesh->mVertices[v].y,
                SourceMesh->mVertices[v].z
            };

            if (SourceMesh->HasNormals()) {
                vertex.Normal = {
                    SourceMesh->mNormals[v].x,
                    SourceMesh->mNormals[v].y,
                    SourceMesh->mNormals[v].z
                };
            } else {
                vertex.Normal = {0.0f, 1.0f, 0.0f};
            }

            if (SourceMesh->HasTextureCoords(0)) {
                vertex.TexCoord = {
                    SourceMesh->mTextureCoords[0][v].x,
                    SourceMesh->mTextureCoords[0][v].y
                };
            } else {
                vertex.TexCoord = {0.0f, 0.0f};
            }

            vertex.Color = {1.0f, 1.0f, 1.0f, 1.0f};

            Vertices.push_back(vertex);
        }

        for (unsigned int f = 0; f < SourceMesh->mNumFaces; ++f) {
            const aiFace& Face = SourceMesh->mFaces[f];

            for (unsigned int index = 0; index < Face.mNumIndices; ++index) {
                Indices.push_back(Face.mIndices[index]);
            }
        }

        std::unique_ptr<Mesh> MeshAsset = CreateMesh(Vertices, Indices);

        std::string MeshName = SourceMesh->mName.C_Str();

        if (MeshName.empty()) {
            MeshName = std::to_string(i);
        }

        std::string MeshID = ModelID + "_" + MeshName;

        Assets::Meshes.Add(MeshID, std::move(MeshAsset));

        MeshPart ModelMeshPart;
        ModelMeshPart.MeshID = MeshID;

        if (SourceMesh->mMaterialIndex < Scene->mNumMaterials) {
            aiMaterial* SourceMaterial = Scene->mMaterials[SourceMesh->mMaterialIndex];

            aiString AssimpMaterialName;

            std::string MaterialName = "Material_" + std::to_string(SourceMesh->mMaterialIndex);

            if (SourceMaterial->Get(AI_MATKEY_NAME, AssimpMaterialName) == AI_SUCCESS && AssimpMaterialName.length > 0) {
                MaterialName = AssimpMaterialName.C_Str();
            }

            ModelMeshPart.MaterialID = ModelID + "_" + MaterialName;
        } else {
            ModelMeshPart.MaterialID = "Default";
        }

        ModelAsset->MeshParts.push_back(std::move(ModelMeshPart));
    }

    Assets::Models.Add(ModelID, std::move(ModelAsset));

    std::println("Processed model '{}'", ModelID);
}