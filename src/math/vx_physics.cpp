#include "../include/vx_math/vx_physics.h"
#include <cmath>
#include <vector>
#include <algorithm>

/*

Made by: htcdevk0
Access: https://vertx3d.sbs to more information

Version:
Pre-Alpha

Version Type:
For user : No Comments

Compiler:
Mingw64 - Msys2

*/

struct PhysicsWorldInternal
{
    vxPhysicsWorld *world;
    float accumulator;
};

static float vec3_length(Vector3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static Vector3 vec3_normalize(Vector3 v)
{
    float len = vec3_length(v);
    if (len > 0.0001f)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}

VX_API vxPhysicsWorld *vxCreatePhysicsWorld()
{
    vxPhysicsWorld *world = (vxPhysicsWorld *)malloc(sizeof(vxPhysicsWorld));
    if (!world)
        return nullptr;

    world->gravity = VEC3(0.0f, -9.81f, 0.0f);
    world->fixedTimeStep = 1.0f / 60.0f;
    world->maxIterations = 10;
    world->bodies = nullptr;
    world->colliders = nullptr;
    world->bodyCount = 0;
    world->colliderCount = 0;

    return world;
}

VX_API void vxDestroyPhysicsWorld(vxPhysicsWorld *world)
{
    if (!world)
        return;

    vxRigidBody *body = world->bodies;
    while (body)
    {
        vxRigidBody *next = body->next;
        free(body);
        body = next;
    }

    vxCollider *collider = world->colliders;
    while (collider)
    {
        vxCollider *next = collider->next;
        free(collider);
        collider = next;
    }

    free(world);
}

VX_API void vxPhysicsSetGravity(vxPhysicsWorld *world, float x, float y, float z)
{
    if (world)
    {
        world->gravity.x = x;
        world->gravity.y = y;
        world->gravity.z = z;
    }
}

VX_API void vxPhysicsStep(vxPhysicsWorld *world, float deltaTime)
{
    if (!world)
        return;

    vxRigidBody *ground = nullptr;
    vxRigidBody *body = world->bodies;

    while (body && !ground)
    {
        if (body->isStatic || body->mass == 0.0f)
        {
            ground = body;
        }
        body = body->next;
    }

    float groundY = ground ? ground->position.y : -1.0f;

    body = world->bodies;
    while (body)
    {
        if (body->isStatic || body->mass == 0.0f)
        {
            body = body->next;
            continue;
        }

        if (body->useGravity)
        {
            body->velocity.y += world->gravity.y * deltaTime;
        }

        body->position.x += body->velocity.x * deltaTime;
        body->position.y += body->velocity.y * deltaTime;
        body->position.z += body->velocity.z * deltaTime;

        float cubeBottom = body->position.y - 0.25f;
        float groundTop = groundY + 0.25f;

        if (cubeBottom < groundTop)
        {

            body->position.y = groundTop + 0.25f;

            if (body->velocity.y < 0)
            {
                body->velocity.y = -body->velocity.y * body->restitution;

                if (fabs(body->velocity.y) < 0.1f)
                {
                    body->velocity.y = 0;
                }
            }
        }

        body->velocity.x *= 0.99f;
        body->velocity.z *= 0.99f;

        body = body->next;
    }
}

VX_API vxRigidBody *vxCreateRigidBody(vxMesh *mesh, float mass)
{
    vxRigidBody *body = (vxRigidBody *)malloc(sizeof(vxRigidBody));
    if (!body)
        return nullptr;

    body->mass = mass;
    body->restitution = 0.5f;
    body->friction = 0.3f;
    body->position = VEC3_ZERO;
    body->velocity = VEC3_ZERO;
    body->acceleration = VEC3_ZERO;
    body->force = VEC3_ZERO;
    body->angularVelocity = VEC3_ZERO;
    body->torque = VEC3_ZERO;
    body->useGravity = 1;
    body->isStatic = (mass <= 0.0f);
    body->isSleeping = 0;
    body->mesh = mesh;
    body->userData = nullptr;
    body->collider = nullptr;
    body->next = nullptr;

    return body;
}

VX_API void vxDestroyRigidBody(vxRigidBody *body)
{
    if (body)
    {
        free(body);
    }
}

VX_API void vxAddRigidBody(vxPhysicsWorld *world, vxRigidBody *body)
{
    if (!world || !body)
        return;

    body->next = world->bodies;
    world->bodies = body;
    world->bodyCount++;
}

VX_API void vxRemoveRigidBody(vxPhysicsWorld *world, vxRigidBody *body)
{
    if (!world || !body)
        return;

    vxRigidBody *prev = nullptr;
    vxRigidBody *curr = world->bodies;

    while (curr)
    {
        if (curr == body)
        {
            if (prev)
            {
                prev->next = curr->next;
            }
            else
            {
                world->bodies = curr->next;
            }
            world->bodyCount--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

VX_API void vxBodySetPosition(vxRigidBody *body, float x, float y, float z)
{
    if (body)
    {
        body->position.x = x;
        body->position.y = y;
        body->position.z = z;
    }
}

VX_API void vxBodyGetPosition(vxRigidBody *body, float *x, float *y, float *z)
{
    if (body && x && y && z)
    {
        *x = body->position.x;
        *y = body->position.y;
        *z = body->position.z;
    }
}

VX_API void vxBodySetVelocity(vxRigidBody *body, float x, float y, float z)
{
    if (body)
    {
        body->velocity.x = x;
        body->velocity.y = y;
        body->velocity.z = z;
    }
}

VX_API void vxBodyAddForce(vxRigidBody *body, float x, float y, float z)
{
    if (body)
    {
        body->force.x += x;
        body->force.y += y;
        body->force.z += z;
    }
}

VX_API void vxBodyAddImpulse(vxRigidBody *body, float x, float y, float z)
{
    if (body && body->mass > 0)
    {

        body->velocity.x += x / body->mass;
        body->velocity.y += y / body->mass;
        body->velocity.z += z / body->mass;
    }
}

VX_API void vxBodySetGravity(vxRigidBody *body, int useGravity)
{
    if (body)
    {
        body->useGravity = useGravity ? 1 : 0;
    }
}

VX_API void vxBodySetRestitution(vxRigidBody *body, float restitution)
{
    if (body)
    {
        body->restitution = restitution;
    }
}

VX_API void vxBodySetFriction(vxRigidBody *body, float friction)
{
    if (body)
    {
        body->friction = friction;
    }
}

VX_API vxCollider *vxCreateBoxCollider(vxRigidBody *body, float width, float height, float depth)
{
    vxCollider *collider = (vxCollider *)malloc(sizeof(vxCollider));
    if (!collider)
        return nullptr;

    collider->type = VX_COLLIDER_BOX;
    collider->center = VEC3_ZERO;
    collider->size.x = width;
    collider->size.y = height;
    collider->size.z = depth;
    collider->radius = 0.0f;
    collider->height = 0.0f;
    collider->body = body;
    collider->next = nullptr;

    if (body)
    {
        body->collider = collider;
    }

    return collider;
}

VX_API vxCollider *vxCreateSphereCollider(vxRigidBody *body, float radius)
{
    vxCollider *collider = (vxCollider *)malloc(sizeof(vxCollider));
    if (!collider)
        return nullptr;

    collider->type = VX_COLLIDER_SPHERE;
    collider->center = VEC3_ZERO;
    collider->size = VEC3_ZERO;
    collider->radius = radius;
    collider->height = 0.0f;
    collider->body = body;
    collider->next = nullptr;

    if (body)
    {
        body->collider = collider;
    }

    return collider;
}

VX_API void vxAddCollider(vxPhysicsWorld *world, vxCollider *collider)
{
    if (!world || !collider)
        return;

    collider->next = world->colliders;
    world->colliders = collider;
    world->colliderCount++;
}

VX_API int vxCheckCollision(vxCollider *a, vxCollider *b)
{
    if (!a || !b || !a->body || !b->body)
        return 0;

    Vector3 posA = a->body->position;
    Vector3 posB = b->body->position;

    posA.x += a->center.x;
    posA.y += a->center.y;
    posA.z += a->center.z;

    posB.x += b->center.x;
    posB.y += b->center.y;
    posB.z += b->center.z;

    if (a->type == VX_COLLIDER_BOX && b->type == VX_COLLIDER_BOX)
    {
        float halfAx = a->size.x / 2;
        float halfAy = a->size.y / 2;
        float halfAz = a->size.z / 2;

        float halfBx = b->size.x / 2;
        float halfBy = b->size.y / 2;
        float halfBz = b->size.z / 2;

        return (fabs(posA.x - posB.x) < (halfAx + halfBx)) &&
               (fabs(posA.y - posB.y) < (halfAy + halfBy)) &&
               (fabs(posA.z - posB.z) < (halfAz + halfBz));
    }

    if ((a->type == VX_COLLIDER_BOX && b->type == VX_COLLIDER_SPHERE) ||
        (a->type == VX_COLLIDER_SPHERE && b->type == VX_COLLIDER_BOX))
    {

        vxCollider *box = (a->type == VX_COLLIDER_BOX) ? a : b;
        vxCollider *sphere = (a->type == VX_COLLIDER_SPHERE) ? a : b;
        Vector3 boxPos = (box == a) ? posA : posB;
        Vector3 spherePos = (sphere == a) ? posA : posB;

        float closestX = fmax(boxPos.x - box->size.x / 2, fmin(spherePos.x, boxPos.x + box->size.x / 2));
        float closestY = fmax(boxPos.y - box->size.y / 2, fmin(spherePos.y, boxPos.y + box->size.y / 2));
        float closestZ = fmax(boxPos.z - box->size.z / 2, fmin(spherePos.z, boxPos.z + box->size.z / 2));

        float dx = spherePos.x - closestX;
        float dy = spherePos.y - closestY;
        float dz = spherePos.z - closestZ;

        return (dx * dx + dy * dy + dz * dz) <= (sphere->radius * sphere->radius);
    }

    return 0;
}