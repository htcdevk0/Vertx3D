#ifndef VX_PHYSICS_H
#define VX_PHYSICS_H

/*

Made by: htcdevk0
Access: https://vertx3d.sbs to more information

Version:
Pre-Alpha

Version Type:
For user : No Comments

Compiler:
MSVC - Visual Studio 2022

*/

#include "../vertx3d.h"

#ifdef __cplusplus
extern "C"
{
#endif

    enum VX_ColliderType
    {
        VX_COLLIDER_NONE = 0,
        VX_COLLIDER_BOX = 1,
        VX_COLLIDER_SPHERE = 2,
        VX_COLLIDER_PLANE = 3,
        VX_COLLIDER_CAPSULE = 4
    };

    typedef struct vxRigidBody vxRigidBody;
    typedef struct vxCollider vxCollider;
    typedef struct vxPhysicsWorld vxPhysicsWorld;

    struct vxRigidBody
    {
        float mass;
        float restitution;
        float friction;
        Vector3 position;
        Vector3 velocity;
        Vector3 acceleration;
        Vector3 force;
        Vector3 angularVelocity;
        Vector3 torque;
        int useGravity;
        int isStatic;
        int isSleeping;
        vxMesh *mesh;
        void *userData;
        vxCollider *collider;
        vxRigidBody *next;
    };

    struct vxCollider
    {
        int type;
        Vector3 center;
        Vector3 size;
        float radius;
        float height;
        vxRigidBody *body;
        vxCollider *next;
    };

    struct vxPhysicsWorld
    {
        Vector3 gravity;
        float fixedTimeStep;
        int maxIterations;
        vxRigidBody *bodies;
        vxCollider *colliders;
        int bodyCount;
        int colliderCount;
    };

    VX_API vxPhysicsWorld *vxCreatePhysicsWorld();
    VX_API void vxDestroyPhysicsWorld(vxPhysicsWorld *world);
    VX_API void vxPhysicsSetGravity(vxPhysicsWorld *world, float x, float y, float z);
    VX_API void vxPhysicsStep(vxPhysicsWorld *world, float deltaTime);

    VX_API vxRigidBody *vxCreateRigidBody(vxMesh *mesh, float mass);
    VX_API void vxDestroyRigidBody(vxRigidBody *body);
    VX_API void vxAddRigidBody(vxPhysicsWorld *world, vxRigidBody *body);
    VX_API void vxRemoveRigidBody(vxPhysicsWorld *world, vxRigidBody *body);

    VX_API void vxBodySetPosition(vxRigidBody *body, float x, float y, float z);
    VX_API void vxBodyGetPosition(vxRigidBody *body, float *x, float *y, float *z);
    VX_API void vxBodySetVelocity(vxRigidBody *body, float x, float y, float z);
    VX_API void vxBodyAddForce(vxRigidBody *body, float x, float y, float z);
    VX_API void vxBodyAddImpulse(vxRigidBody *body, float x, float y, float z);
    VX_API void vxBodySetGravity(vxRigidBody *body, int useGravity);
    VX_API void vxBodySetRestitution(vxRigidBody *body, float restitution);
    VX_API void vxBodySetFriction(vxRigidBody *body, float friction);

    VX_API vxCollider *vxCreateBoxCollider(vxRigidBody *body, float width, float height, float depth);
    VX_API vxCollider *vxCreateSphereCollider(vxRigidBody *body, float radius);
    VX_API vxCollider *vxCreatePlaneCollider(vxRigidBody *body, float nx, float ny, float nz);
    VX_API vxCollider *vxCreateCapsuleCollider(vxRigidBody *body, float radius, float height);
    VX_API void vxDestroyCollider(vxCollider *collider);
    VX_API void vxAddCollider(vxPhysicsWorld *world, vxCollider *collider);

    VX_API int vxCheckCollision(vxCollider *a, vxCollider *b);
    VX_API int vxRaycast(vxPhysicsWorld *world, Vector3 origin, Vector3 direction, float maxDistance, vxCollider **hitCollider);

#define VX_GRAVITY_EARTH VEC3(0.0f, -9.81f, 0.0f)
#define VX_GRAVITY_MOON VEC3(0.0f, -1.62f, 0.0f)
#define VX_GRAVITY_NONE VEC3(0.0f, 0.0f, 0.0f)

#ifdef __cplusplus
}
#endif

#endif