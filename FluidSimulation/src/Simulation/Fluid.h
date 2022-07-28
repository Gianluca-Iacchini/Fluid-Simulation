#pragma once

#include "FluidStep.h"
#include "Core/Node/Node.h"
#include "Core/Render/Framebuffer.h"

namespace FluidSimulation {

    typedef Framebuffer::PingPongFramebuffer PingPongFramebuffer;

    class Fluid
    {
    public:
        Fluid(glm::vec3 gridScale = glm::vec3(16));
        void Update(float timeStep);
        int Read(Framebuffer* target, int textureTarget = -1);
        void Write(Framebuffer* target);
        void Initialize();
        void DrawDepthBuffer();
    private:

        void setUpBox();
        void setUpBoundaries();
        void setUpQuad();



        void Render();
        void CreateJitterTexture();
        void Advect(PingPongFramebuffer& quantity, float timeStep, float dissipation);
        void MacCormackAdvect(PingPongFramebuffer& quantity, float timeStep, float dissipation);
        void JacobiPressure(unsigned int iterations, float pressure);
        void Impulse(PingPongFramebuffer& base, glm::vec3 force, glm::vec3 pos, float radius);
        void Vorticity(float timeStep, float vorticity);
        void GradientSubtraction();
        void Buoyancy(PingPongFramebuffer& velocity, PingPongFramebuffer& dye, float timeStep, float buoyancy, float fluidWeight);


        void ResetState();
        void SetAlphaBlending();
        void SetSubtractiveBlending();



        glm::vec2 screenSize = glm::vec2(1280, 720);

        struct fluid_param
        {
            float DISSIPATION = 0.99995f;
            float VORTICITY = 0.3f;
            float PRESSURE = 0.0f;
        };

        glm::vec3 gridDimension;
        fluid_param param;

        Shader* advectionProgram = nullptr;
        Shader* macCormackAdvectionProgram = nullptr;
        Shader* divergenceProgram = nullptr;
        Shader* curlProgram = nullptr;
        Shader* gradientSubtractionProgram = nullptr;
        Shader* buoyancyProgram = nullptr;
        Shader* clearProgram = nullptr;
        Shader* setColorProgram = nullptr;
        Shader* pressureProgram = nullptr;
        Shader* diffuseProgram = nullptr;
        Shader* vorticityProgram = nullptr;
        Shader* boundaryProgram = nullptr;
        Shader* splatProgram = nullptr;

        Shader* drawTexture = nullptr;

        Shader* rayDataProgram = nullptr;
        Shader* rayMarchProgram = nullptr;


        PingPongFramebuffer velocity;
        PingPongFramebuffer phi_hat;
        PingPongFramebuffer phi_hat_1;
        Framebuffer* divergence;
        Framebuffer* curl;
        PingPongFramebuffer dye;
        PingPongFramebuffer temperature;
        PingPongFramebuffer pressure;
        PingPongFramebuffer trackPart;
        Framebuffer* obstacleBounds;
        Framebuffer* rayData;

        Framebuffer* depthBuffer;



        bool isImpulse = true;
        bool isImpulseDye = true;

        unsigned int boxVAO;
        unsigned int boxVBO;
        unsigned int boxEBO;

        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int quadEBO;

        unsigned int textureNum = 0;

        unsigned int jitterTexture = 0;
    };

}