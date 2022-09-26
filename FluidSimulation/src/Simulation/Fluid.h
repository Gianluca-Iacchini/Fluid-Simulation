#pragma once
#include "Core/Render/Shader.h"
#include "Core/Node/Node.h"
#include "Core/Render/Framebuffer.h"

// Fluid simulation core class
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

        void ImGuiControlWindowUpdate();
        void ImGuiBuoyancyImpulseSwitcher();


        glm::vec2 screenSize = glm::vec2(1280, 720);

        struct fluid_param
        {
            float DISSIPATION = 1.f;
            float VORTICITY = 0.35f;
            float BUOYANCY = 1.0f;
            float FLUID_WEIGHT = 0.125f;
            float PRESSURE = 0.0f;
            float DYE_RADIUS = 16.f;
            float TEMPERATURE = 12.f;
            float AMBIENT_TEMPERATURE = 0.f;
            float IMPULSE_RADIUS = 12.f;
            glm::vec3 IMPULSE_FORCE = glm::vec3(0, 100.f, 0);
            glm::vec3 IMPULSE_POSITION = glm::vec3(0.5f, 0.2f, 0.5f);
            glm::vec3 DYE_COLOR = glm::vec3(1);
            glm::vec3 DYE_DENSITY = glm::vec3(12);
            glm::vec3 FLUID_POSITION = glm::vec3(0.5f, 0.2f, 0.5f);
        };

        float imguiDissipation = 0.01f;
        bool imguiIsBuoyancy = true;
        bool useMacCormackDye = false;
        bool useMacCormackVelocity = false;

        glm::vec3 gridDimension;
        fluid_param param;

        Shader* advectionProgram = nullptr;
        Shader* macCormackAdvectionProgram = nullptr;
        Shader* divergenceProgram = nullptr;
        Shader* curlProgram = nullptr;
        Shader* gradientSubtractionProgram = nullptr;
        Shader* buoyancyProgram = nullptr;
        Shader* clearProgram = nullptr;
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

        Framebuffer** drawBuffer;


        bool isImpulse = false;
        bool isImpulseDye = true;
        bool isBuoyancy = true;

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