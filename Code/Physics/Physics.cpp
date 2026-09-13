#include "Physics/Physics.h"
#include "ECS/Components.h"

#include <box3d/box3d.h>
#include <print>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

static b3WorldDef WorldDef;
static b3WorldId WorldID;

struct PhysicsBodyData {
    flecs::entity_t entityId;
    b3BodyId bodyId;
};

static std::vector<PhysicsBodyData> InternalBodies;

void Physics::Initialize(const flecs::world& World) {
    WorldDef = b3DefaultWorldDef();
    WorldDef.gravity = (b3Vec3){0.0f, -9.8f, 0.0f};

    WorldID = b3CreateWorld(&WorldDef);

    InternalBodies.clear();

    World.each([&](flecs::entity e, TransformComponent& Transform, BoxCollisionComponent& Collision) {
        b3BodyDef BoxCollision = b3DefaultBodyDef();

        switch (Collision.BodyType) {
            case StaticBody:
                BoxCollision.type = b3_staticBody;
                break;

            case DynamicBody:
                BoxCollision.type = b3_dynamicBody;
                break;

            case KinematicBody:
                BoxCollision.type = b3_kinematicBody;
                break;
        }

        // Position
        BoxCollision.position = {Transform.Position.x, Transform.Position.y, Transform.Position.z};

        // Rotation
        glm::quat rotation = glm::quat(glm::radians(Transform.Rotation));

        BoxCollision.rotation = {rotation.x, rotation.y, rotation.z, rotation.w};

        b3BodyId BoxCollisionID = b3CreateBody(WorldID, &BoxCollision);

        // Half extents
        b3BoxHull BoxCollisionShape = b3MakeBoxHull(Transform.Scale.x / 2, Transform.Scale.y / 2, Transform.Scale.z / 2);

        b3ShapeDef BoxCollisionShapeDef = b3DefaultShapeDef();

        BoxCollisionShapeDef.density = Collision.Density;

        BoxCollisionShapeDef.baseMaterial.friction = Collision.Friction;

        b3CreateHullShape(BoxCollisionID, &BoxCollisionShapeDef, &BoxCollisionShape.base);

        InternalBodies.push_back({e.id(), BoxCollisionID});
    });

    std::println(
        "Physics initialized with {} bodies",
        InternalBodies.size()
    );
}

void Physics::Update(const flecs::world& World, float DeltaTime) {
    b3World_Step(WorldID, DeltaTime, 4);

    for (const auto& data : InternalBodies) {
        flecs::entity e = World.entity(data.entityId);

        if (!e.is_valid() || !e.has<TransformComponent>())
            continue;

        TransformComponent& transform = e.get_mut<TransformComponent>();

        // Position
        b3Vec3 pos = b3Body_GetPosition(data.bodyId);

        transform.Position = {pos.x, pos.y, pos.z};

        // Rotation
        b3Quat rotation = b3Body_GetRotation(data.bodyId);

        glm::quat quat(rotation.s,rotation.v.x,rotation.v.y,rotation.v.z);

        transform.Rotation = glm::degrees(glm::eulerAngles(quat));
    }
}

void Physics::Shutdown() {
    InternalBodies.clear();
    b3DestroyWorld(WorldID);
}