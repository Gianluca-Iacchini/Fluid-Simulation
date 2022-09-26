#pragma once

#include "Core/Time.h"
#include <btBulletDynamicsCommon.h>

/* Simple wrapper class for Bullet3 */
namespace FluidSimulation {

    enum Shape { BOX, SPHERE };



    class Physics
    {
    public:
        btDiscreteDynamicsWorld* dynamicsWorld;

        btAlignedObjectArray<btCollisionShape*> collisionShapes;

        btDefaultCollisionConfiguration* collisionConfiguration;

        btCollisionDispatcher* dispatcher;

        btBroadphaseInterface* overlappingPairCache;

        btSequentialImpulseConstraintSolver* solver;

        Physics(Time& time);
        void Clear();
        void Update();

    private:
        Time& time;

    };
}