#include "app.h"
#include "Scene/Entity/Components.h"
#include "Scene/Entity/Entity.h"

#include <string>
#include <vector>

// global Entity
static Entity Camera;
static Entity Cube;
static Entity Light;

App::App() {
    // Set the window title
    WindowTilte = "Testing";
    // Use WindowWidth or WindowHeight to change the size, or, use Context.DefaultWindow.Title . . .
}

App::~App() {

}

void App::OnStart() {
    // Scan a folder and return a vector of the assets path (e.g: image.png, model.glb)
    std::vector<std::string> assets_files = ScanFiles("Sandbox/Assets");
    AssetProcessor(assets_files); // Process every file found and return an asset with the file name (e.g image.png -> image)

    Assets::Textures.Get("Default")->Mipmaps(true); // Find an image asset with the name Default and enable mipmaps

    // Create Entities 
    Camera = Context.DefaultScene->CreateEntity();
    Cube = Context.DefaultScene->CreateEntity();
    Light = Context.DefaultScene->CreateEntity();

    // Add components and set values for the entity
    Context.DefaultScene->AddComponent<TransformComponent>(Camera).Position = {0.0f, 0.0f, 5.0f}; // The default position and rotation is 0
    Context.DefaultScene->AddComponent<CameraComponent>(Camera).Current = true; // True by default, use this for multiple cameras

    Context.DefaultScene->AddComponent<TransformComponent>(Cube).Position = {-1.0f, 0.0f, 0.0f};
    Context.DefaultScene->AddComponent<MeshComponent>(Cube).MeshID = "Cube"; // Pick a mesh by their name/id

    Context.DefaultScene->AddComponent<TransformComponent>(Light).Position = {0.0f, 5.0f, 2.0f};
    Context.DefaultScene->AddComponent<LightComponent>(Light).LightColor = {1.0f, 0.0f, 1.0f};
}

void App::OnUpdate() {
    // Rotate the Cube entity every frame and multiply by delta
    Context.DefaultScene->GetComponent<TransformComponent>(Cube)->Rotation.y += 50 * Context.Delta->DeltaTime;
    Context.DefaultScene->GetComponent<TransformComponent>(Cube)->Rotation.z += 50 * Context.Delta->DeltaTime;
}

void App::OnStop() {
    // Clear every asset (Mandatory)
    Assets::Meshes.Clear();
    Assets::Shaders.Clear();
    Assets::Textures.Clear();
}
