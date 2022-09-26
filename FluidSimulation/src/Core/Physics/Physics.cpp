#include "fspch.h"
#include "Physics.h"
#include "Core/Node/Node.h"
#include "Utils/conv.h"


namespace FluidSimulation {

    // Physics initialization method
    Physics::Physics(Time& time) : time(time)
    {
        this->collisionConfiguration = new btDefaultCollisionConfiguration();
        this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
        this->overlappingPairCache = new btDbvtBroadphase();

        this->solver = new btSequentialImpulseConstraintSolver();

        this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->overlappingPairCache, this->solver, this->collisionConfiguration);

        this->dynamicsWorld->setGravity(btVector3(0.0f, -9.82f, 0.0f));
    }

    // Physics deconstructor method
    void Physics::Clear()
    {
        for (int i = this->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            this->dynamicsWorld->removeCollisionObject(obj);
        }

        for (int j = 0; j < this->collisionShapes.size(); j++)
        {
            btCollisionShape* shape = this->collisionShapes[j];
            this->collisionShapes[j] = 0;
            delete shape;
        }

        delete this->dynamicsWorld;
        delete this->solver;
        delete this->overlappingPairCache;
        delete this->dispatcher;
        delete this->collisionConfiguration;
        this->collisionShapes.clear();
    }

    // Update function is called once every frame
    void Physics::Update()
    {
        dynamicsWorld->stepSimulation(this->time.getDeltaTime() > 1.f / 60.f ? this->time.getDeltaTime() : 1.f / 60.f, 10);
        float matrix[16];



        for (unsigned int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];

            btRigidBody* body = btRigidBody::upcast(obj);
            btTransform trans;

            if (body && body->getMotionState())
            {
                body->getMotionState()->getWorldTransform(trans);
                Node* node = (Node*)body->getUserPointer();

                if (node != NULL)
                {
                    node->transform->SetPosition(conv::btVec3ToGlm3(trans.getOrigin()));
                    node->transform->SetRotation(conv::btQuaternionToFquat(trans.getRotation()));
                }
            }
            else
                trans = obj->getWorldTransform();

        }


    }
}