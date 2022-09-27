#include "fspch.h"
#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_glfw.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"


#include "Fluid.h"
#include "Core/Camera.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Node/Scene.h"

namespace FluidSimulation {

	// Initializes variables and class properties.
    Fluid::Fluid(glm::vec3 gridDimension)
    {
		float screenWidth = Application::Get().GetWindow().GetWidth();
		float screenHeight = Application::Get().GetWindow().GetHeight();

		screenSize = glm::vec2(screenWidth, screenHeight);

		this->gridDimension = gridDimension;


		advectionProgram = Shader("Simulation/basicFluid.vert", "Simulation/advection.frag", "Simulation/layeredRender.geom");
		macCormackAdvectionProgram = Shader("Simulation/basicFluid.vert", "Simulation/maccormack_advect.frag", "Simulation/layeredRender.geom");
		divergenceProgram = Shader("Simulation/basicFluid.vert", "Simulation/divergence.frag", "Simulation/layeredRender.geom");
		curlProgram =  Shader("Simulation/basicFluid.vert", "Simulation/curl.frag", "Simulation/layeredRender.geom");
		gradientSubtractionProgram = Shader("Simulation/basicFluid.vert", "Simulation/gradientSubtraction.frag", "Simulation/layeredRender.geom");
		buoyancyProgram = Shader("Simulation/basicFluid.vert", "Simulation/buoyancy.frag", "Simulation/layeredRender.geom");
		clearProgram =  Shader("Simulation/basicFluid.vert", "Simulation/clearTexture.frag", "Simulation/layeredRender.geom");
		pressureProgram = Shader("Simulation/basicFluid.vert", "Simulation/jacobi.frag", "Simulation/layeredRender.geom");
		diffuseProgram = Shader("Simulation/basicFluid.vert", "Simulation/jacobi.frag", "Simulation/layeredRender.geom");
		vorticityProgram = Shader("Simulation/basicFluid.vert", "Simulation/vorticity.frag", "Simulation/layeredRender.geom");
		boundaryProgram = Shader("Simulation/basicFluid.vert", "Simulation/obstacleBounds.frag", "Simulation/layeredRender.geom");
		splatProgram = Shader("Simulation/basicFluid.vert", "Simulation/splat.frag", "Simulation/layeredRender.geom");

		drawTexture = Shader("Simulation/drawTexture.vert", "Simulation/drawTexture.frag");

		rayDataProgram = Shader("Simulation/rayData.vert", "Simulation/rayData.frag");
		rayMarchProgram = Shader("Simulation/rayMarching.vert", "Simulation/rayMarching.frag");

    }

	void Fluid::Initialize()
	{
		phi_hat = PingPongFramebuffer(gridDimension, 4);
		phi_hat_1 = PingPongFramebuffer(gridDimension, 4);
		velocity = PingPongFramebuffer(gridDimension, 3);
		divergence = Framebuffer(gridDimension, 1);
		curl =  Framebuffer(gridDimension, 3);
		dye = PingPongFramebuffer(gridDimension, 4);
		temperature = PingPongFramebuffer(gridDimension, 1);
		pressure = PingPongFramebuffer(gridDimension, 1);
		obstacleBounds = Framebuffer(gridDimension, 1);
		rayData =  Framebuffer(screenSize, 4);

		depthBuffer = Framebuffer(screenSize, 4);
		depthBuffer.AttachDepthBufferTexture();

		drawBuffer = &dye.readFBO;

		setUpQuad();
		setUpBox();
		setUpBoundaries();
		CreateJitterTexture();
	}

	// Initialize a basic quad. Vertices contains position, texture coordinates and grid coordinates.
    void Fluid::setUpQuad()
    {
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		    //positions    //texCoords  //cellCoords
            -1.0f, -1.0f,  0.0f, 0.0f,  0.f, 0.f,
             1.0f, -1.0f,  1.0f, 0.0f,  gridDimension.x, 0.f,
            -1.0f,  1.0f,  0.0f, 1.0f,  0.0f, gridDimension.y,
             1.0f,  1.0f,  1.0f, 1.0f,  gridDimension.x, gridDimension.y,
		};
        unsigned int indices[] = { 0,1,2,1,3,2};

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadEBO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);


        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));

        glBindVertexArray(0);
    }



	// Setup of a 3D cube
	void Fluid::setUpBox()
    {
 

		float vertices[] =
		{
		    // Position            

		    // Front Face
		    0.f, 0.f, 0.f,   
		    0.f, 0.f, 1.0f,  
		    0.f, 1.f, 0.f,   
		    0.f, 1.f, 1.f,   

		    // Back Face
		    1.f, 0.f, 0.f,   
		    1.f, 0.f, 1.f,   
		    1.f, 1.f, 0.f,   
		    1.f, 1.f, 1.f,
		};

		unsigned int indices[] =
		{
		    // Front Face
		    0, 4, 1, 1, 4, 5,

		    // Back Face
		    0, 1, 2, 2, 1, 3,

		    // Right Face
		    4, 6, 5, 6, 7, 5,

		    // Left face
		    2, 3, 6, 3, 7, 6,

		    // Top Face
		    1, 5, 3, 3, 5, 7,

		    // Bottom Face
		    0, 2, 4, 2, 6, 4
		};

		glGenVertexArrays(1, &boxVAO);
		glGenBuffers(1, &boxVBO);
		glGenBuffers(1, &boxEBO);

		glBindVertexArray(boxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
    }


	// Initializes boundary texture.
    void Fluid::setUpBoundaries()
    {
        glBindVertexArray(quadVAO);
        boundaryProgram.Use();
        boundaryProgram.SetVec3("gridDimension", gridDimension);
        Write(obstacleBounds);
        glBindVertexArray(0);
    }

	// Function to be called in the main loop; all scene geometry will be written to this framebuffer and displayed later. The depth texture will be used for geometry occlusion while the color texture will be used to blend the scene.
	void Fluid::DrawDepthBuffer()
	{
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer.GetFramebufferHandle());

        glViewport(0, 0, screenSize.x, screenSize.y);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	// Initializes a 256x256 jitter texture which will be used to reduce banding in the final draw call.
	void Fluid::CreateJitterTexture()
	{
		glGenTextures(1, &jitterTexture);
		glBindTexture(GL_TEXTURE_2D, jitterTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		unsigned char* data = new unsigned char[256 * 256 * sizeof(unsigned char)];

		for (unsigned int i = 0; i < 256 * 256; i++)
		{
			data[i] = (unsigned char)(rand() / float(RAND_MAX) * 256);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, data);


		glBindTexture(GL_TEXTURE_2D, 0);
		delete[] data;
	}



	// Wrapper function for activating and binding 3D texture.
    int Fluid::Read(Framebuffer& target, int textureTarget)
    {

        GLuint textureHandle = target.GetColorTextureHandle();

        glActiveTexture(GL_TEXTURE0 + textureTarget);
        glBindTexture(GL_TEXTURE_3D, textureHandle);

        return textureTarget;
    }

	// Wrapper function for activating and writing to target framebuffer
    void Fluid::Write(Framebuffer& target)
    {

        GLuint fboHandle = target.GetFramebufferHandle();
        glm::vec3 fboTextureSize = target.GetFramebufferSize();

        glViewport(0, 0, fboTextureSize.x, fboTextureSize.y);

        glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, fboTextureSize.z);
        ResetState();


    }


	// Wrapper function to reset texture and framebuffers states.
	void Fluid::ResetState()
    {
		for (unsigned int i = 0; i < 5; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_3D, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_BLEND);
    }

	// Advects a fluid quantity by the fluid velocity
    void Fluid::Advect(PingPongFramebuffer& quantity, float timeStep, float dissipation)
    {
		advectionProgram.Use();
		advectionProgram.SetInt("obstacle", Read(obstacleBounds, 0));
		advectionProgram.SetInt("velocity", Read(velocity.readFBO, 1));
		advectionProgram.SetInt("quantity", Read(quantity.readFBO, 2));
		advectionProgram.SetVec3("gridDimension", gridDimension);
		advectionProgram.SetFloat("timeStep", timeStep);
		advectionProgram.SetFloat("dissipation", dissipation);
		Write(quantity.writeFBO);
		quantity.Swap();
    }

	// Advection using Mac-Cormack scheme.
	void Fluid::MacCormackAdvect(PingPongFramebuffer& quantity, float timeStep, float dissipation)
	{
		advectionProgram.Use();
		advectionProgram.SetInt("obstacle", Read(obstacleBounds, 0));
		advectionProgram.SetInt("velocity", Read(velocity.readFBO, 1));
		advectionProgram.SetInt("quantity", Read(quantity.readFBO, 2));
        advectionProgram.SetVec3("gridDimension", gridDimension);
		advectionProgram.SetFloat("timeStep", timeStep);
		advectionProgram.SetFloat("dissipation", dissipation);
		Write(phi_hat_1.writeFBO);
		phi_hat_1.Swap();

		advectionProgram.Use();
		advectionProgram.SetInt("obstacle", Read(obstacleBounds, 0));
		advectionProgram.SetInt("velocity", Read(velocity.readFBO, 1));
		advectionProgram.SetInt("quantity", Read(phi_hat_1.readFBO, 2));
        advectionProgram.SetVec3("gridDimension", gridDimension);
		advectionProgram.SetFloat("timeStep", -timeStep);
		advectionProgram.SetFloat("dissipation", dissipation);
		Write(phi_hat.writeFBO);
		phi_hat.Swap();

		macCormackAdvectionProgram.Use();
		macCormackAdvectionProgram.SetVec3("gridDimension", gridDimension);
		macCormackAdvectionProgram.SetFloat("timeStep", timeStep);
		macCormackAdvectionProgram.SetFloat("dissipation", dissipation);
		macCormackAdvectionProgram.SetInt("obstacle", Read(obstacleBounds, 0));
		macCormackAdvectionProgram.SetInt("velocity", Read(velocity.readFBO, 1));
		macCormackAdvectionProgram.SetInt("phi_n", Read(quantity.readFBO, 2));
		macCormackAdvectionProgram.SetInt("phi_hat", Read(phi_hat.readFBO, 3));
		macCormackAdvectionProgram.SetInt("phi_hat_1", Read(phi_hat_1.readFBO, 4));
		Write(quantity.writeFBO);
		quantity.Swap();
	}

	// Computes fluid pressure, more iterations will grant a better result but will be more costly
	void Fluid::JacobiPressure(unsigned int iterations, float pressureParam)
    {
        divergenceProgram.Use();
        divergenceProgram.SetVec3("gridDimension", gridDimension);
        divergenceProgram.SetInt("obstacle", Read(obstacleBounds, 0));
        divergenceProgram.SetInt("velocity", Read(velocity.readFBO, 1));
        Write(divergence);


        clearProgram.Use();
        clearProgram.SetVec3("gridDimension", gridDimension);
        clearProgram.SetInt("base", Read(pressure.readFBO, 0));
        clearProgram.SetFloat("value", pressureParam);
        Write(pressure.writeFBO);
		pressure.Swap();


		for (unsigned int i = 0; i < iterations; i++)
		{
			pressureProgram.Use();
			pressureProgram.SetVec3("gridDimension", gridDimension);
			pressureProgram.SetInt("obstacle", Read(obstacleBounds, 0));
			pressureProgram.SetInt("divergence", Read(divergence, 1));
			pressureProgram.SetInt("pressure", Read(pressure.readFBO, 2));
			Write(pressure.writeFBO);
			pressure.Swap();
		}

    }

	// Creates a gaussian splat in the base texture at the specified position with the specified radius and the specified color.
    void Fluid::Impulse(PingPongFramebuffer& base, glm::vec3 force, glm::vec3 pos, float radius)
    {
        splatProgram.Use();
        splatProgram.SetVec3("gridDimension", gridDimension);
        splatProgram.SetInt("obstacle", Read(obstacleBounds, 0));
        splatProgram.SetInt("base", Read(base.readFBO, 1));
        splatProgram.SetVec3("color", force);
        splatProgram.SetVec3("texSize", gridDimension);
        splatProgram.SetVec3("position", pos);
        splatProgram.SetFloat("radius", radius);
        Write(base.writeFBO);
		base.Swap();
    }

	// Computes fluid vorticity
    void Fluid::Vorticity(float timeStep, float vorticity)
    {
		curlProgram.Use();
		curlProgram.SetVec3("gridDimension", gridDimension);
		curlProgram.SetInt("w", Read(velocity.readFBO, 0));
		Write(curl);


		vorticityProgram.Use();
		vorticityProgram.SetVec3("gridDimension", gridDimension);
		vorticityProgram.SetInt("obstacle", Read(obstacleBounds, 0));
        vorticityProgram.SetInt("curl", Read(curl, 1));
		vorticityProgram.SetInt("w", Read(velocity.readFBO, 2));
		vorticityProgram.SetFloat("timeStep", timeStep);
		vorticityProgram.SetFloat("vorticity", vorticity);
		Write(velocity.writeFBO);
		velocity.Swap();

    }

	// Gradient subtraction to compute final fluid velocity for this iteration.
    void Fluid::GradientSubtraction()
    {
        gradientSubtractionProgram.Use();
        gradientSubtractionProgram.SetVec3("gridDimension", gridDimension);
        gradientSubtractionProgram.SetInt("obstacle", Read(obstacleBounds, 0));
        gradientSubtractionProgram.SetInt("pressure", Read(pressure.readFBO, 1));
        gradientSubtractionProgram.SetInt("velocity", Read(velocity.readFBO, 2));
        Write(velocity.writeFBO);
		velocity.Swap();
    }


	// Computes fluid buoyancy forces.
	void Fluid::Buoyancy(PingPongFramebuffer& velocity, PingPongFramebuffer& dye, float timeStep, float buoyancy, float fluidWeight)
	{
        buoyancyProgram.Use();
        buoyancyProgram.SetInt("obstacle", Read(obstacleBounds, 0));
        buoyancyProgram.SetInt("velocity", Read(velocity.readFBO, 1));
        buoyancyProgram.SetInt("temperature", Read(velocity.readFBO, 2));
        buoyancyProgram.SetInt("dye", Read(dye.readFBO, 3));
        buoyancyProgram.SetFloat("timeStep", timeStep);
        buoyancyProgram.SetFloat("ambientTemp", param.AMBIENT_TEMPERATURE);
        buoyancyProgram.SetFloat("buoyancy", buoyancy);
        buoyancyProgram.SetFloat("weight", fluidWeight);
        Write(velocity.writeFBO);
		velocity.Swap();
	}


	// Advance the fluid simulation by a single frame
	void Fluid::Update(float timeStep)
    {
        glBindVertexArray(quadVAO);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        //////////////////
        ///// ADVECT /////
        //////////////////

        // We advect velocity first;
		if (useMacCormackVelocity)
			MacCormackAdvect(velocity, timeStep, 0.9995f);
		else
			Advect(velocity, timeStep, 0.9995f);

        // Then we advect other quantities.
		if (useMacCormackDye)
			MacCormackAdvect(dye, timeStep, param.DISSIPATION);
		else
			Advect(dye, timeStep, param.DISSIPATION);

        Advect(temperature, timeStep, 1);



        //////////////////////////////////////
        ////////////// EXTERNAL FORCES ///////
        ////////////////////////////////////



        if (isImpulse)
        {
            Impulse(velocity, param.IMPULSE_FORCE, param.IMPULSE_POSITION * gridDimension, param.IMPULSE_RADIUS);
        }

		if (isImpulseDye)
		{
			Impulse(dye, param.DYE_COLOR * param.DYE_DENSITY, param.FLUID_POSITION * gridDimension, param.DYE_RADIUS);
            Impulse(temperature, glm::vec3(param.TEMPERATURE), param.FLUID_POSITION * gridDimension, param.DYE_RADIUS);

		}

		if (isBuoyancy)
		{
			Buoyancy(velocity, dye, timeStep, param.BUOYANCY, param.FLUID_WEIGHT);
		}


        ////////////////////////////////////
        /////////////// VORTICITY //////////
        ////////////////////////////////////


        Vorticity(timeStep, param.VORTICITY);



        ///////////////////////////////////////////////////
        /////////////////// PRESSURE //////////////////////
        ///////////////////////////////////////////////////

        JacobiPressure(param.JACOBI_ITERATIONS, param.PRESSURE);

        //////////////////////////////////////////////////////////////
        //////////////////////////// Gradient Subtraction ////////////
        //////////////////////////////////////////////////////////////

        GradientSubtraction();
        
		//IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//


        Render();

		ImGuiControlWindowUpdate();
    }

	// Render the fluid in the scene
	void Fluid::Render()
	{

		// First we render the scene that was drawn to Framebuffer::depthBuffer color texture.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(quadVAO);
		glViewport(0, 0, screenSize.x, screenSize.y);

		glClear(GL_COLOR_BUFFER_BIT);

		drawTexture.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthBuffer.GetColorTextureHandle());
		drawTexture.SetInt("colorTexture", 0);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ResetState();


		// Gets the maximum side of the fluid grid.
		float maxSide = glm::max(gridDimension.x, gridDimension.y);
		maxSide = glm::max(maxSide, gridDimension.z);

		// Adjusts the grid by maxSide, in this way we can have simulations inside grids that aren't perfect cubes.
		glm::mat4 scaleM = glm::mat4(1.f);
		scaleM = glm::scale(scaleM, gridDimension / maxSide);

		// Adjusts the grid by -.5f since box vertices are in range [0-1] instead of [-0.5, 0.5]
		glm::mat4 translateM = glm::mat4(1.f);
		translateM = glm::translate(translateM, glm::vec3(-0.5f));

		glm::mat4 gridMatrix = scaleM * translateM;




		// Compute model matrix
		glm::vec3 scale = glm::vec3(1.0f);
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 modelMatrix = glm::mat4(1.0f);

		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, 0.f, rotation);
		modelMatrix = glm::scale(modelMatrix, scale);


		// Retrieve view and projection matrix;
		Camera* camera = Camera::mainCamera;
		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), screenSize.x / screenSize.y, 0.1f, 100.0f);

		glm::mat4 modelView = viewMatrix * modelMatrix;

		modelView = modelView * gridMatrix;

		glm::mat4 modelViewProjection = glm::mat4(1.0f);
		modelViewProjection = projectionMatrix * modelView;

		glm::mat4 inverseModelViewProjection = glm::inverse(modelViewProjection);
		glm::mat4 invModelView = glm::inverse(modelView);

		// Converts eye position to 3D texture coordinates
		glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.f);
		glm::vec4 eyeGridSpace = invModelView * origin;

		// World scale is used to convert the length of a ray from view space to grid space
		glm::vec3 worldAxis = glm::vec3(modelView[0][0], modelView[0][1], modelView[0][2]);
		float worldScale = glm::length(worldAxis);


		 
		// We render a 3D box by drawing the back faces first and the front faces after that.
		// The fragment shader for both draw calls returns the texture coordinates of the box, subtractive blending is enabled in order to satisfy the following equation:
		// outPut.rgb = source.rgb;
		// outPut.a = destination.a - source.a
		// Where outPut is the final result, destination is the output of the back faces draw call and source is the output of the front faces draw call.
		glBindVertexArray(boxVAO);


		rayDataProgram.Use();
		rayDataProgram.SetMat4("modelViewProj", modelViewProjection);
		rayDataProgram.SetVec3("gridDimension", gridDimension);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthBuffer.GetDepthTextureHandle());
		rayDataProgram.SetInt("depthTexture", 0);
		rayDataProgram.SetVec2("screenSize", screenSize);
		rayDataProgram.SetFloat("zNear", 0.1f);
		rayDataProgram.SetFloat("zFar", 100.f);


		glViewport(0, 0, screenSize.x, screenSize.y);




		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		/// 
		///  Back faces
		/// 

		rayDataProgram.SetBool("isBackFace", true);



		glBindFramebuffer(GL_FRAMEBUFFER, rayData.GetFramebufferHandle());

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);


		glCullFace(GL_FRONT);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



		/// Front faces

		rayDataProgram.SetBool("isBackFace", false);

		glCullFace(GL_BACK);



		SetSubtractiveBlending();


		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);




		///////////////////////////
		// Ray marching
		///////////////////////////
		ResetState();
		SetAlphaBlending();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(quadVAO);

		rayMarchProgram.Use();
		rayMarchProgram.SetVec3("eyeOnGrid", eyeGridSpace);
		rayMarchProgram.SetVec3("gridDim", gridDimension);
		rayMarchProgram.SetVec3("recGridDim", glm::vec3(1.0f / gridDimension));
		rayMarchProgram.SetFloat("maxGridDim", maxSide);
		rayMarchProgram.SetFloat("gridScaleFactor", worldScale);
		rayMarchProgram.SetVec2("RTSize", screenSize);
		rayMarchProgram.SetMat4("invModelViewProj", inverseModelViewProjection);
		rayMarchProgram.SetFloat("zNear", 0.1f);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, rayData.GetColorTextureHandle());
		rayMarchProgram.SetInt("rayDataTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, jitterTexture);
		rayMarchProgram.SetInt("jitterTexture", 1);
		
		if (drawBuffer != NULL)
			rayMarchProgram.SetInt("dyeTexture", Read(*drawBuffer, 2));



		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		ResetState();




		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		glBindVertexArray(0);

	}

	void Fluid::SetAlphaBlending()
	{
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	}

	void Fluid::SetSubtractiveBlending()
	{
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ONE);
	}

	void Fluid::ImGuiControlWindowUpdate()
	{
		
		glm::vec2 windowSize = screenSize;

		int totalItems = imguiIsBuoyancy ? 18 : 18;
		windowSize.x = screenSize.x < 1920 ? windowSize.x / 2.6f : windowSize.x / 3.5f;

		float itemsHeight = totalItems * (ImGui::CalcTextSize("Reset").y + ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2) + totalItems * 3.f;
		float itemsWidth = ImGui::CalcTextSize("Impulse pos Reset").x + (ImGui::GetStyle().FramePadding.x * 4) + windowSize.x * 0.65f;
												
		ImGui::SetNextWindowSize(ImVec2(itemsWidth, itemsHeight));
		ImGui::SetNextWindowPos(ImVec2(screenSize.x - itemsWidth, 0.f));



		
		ImGui::Begin("Fluid simulation control values", 0, ImGuiWindowFlags_NoResize);



		ImGui::Text("[TAB] To toggle mouse Cursor");

		ImGuiCalcFPS();

		ImVec2 imguiSize = ImGui::GetWindowSize();


		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);		
		ImGui::SliderFloat("Radius", &param.DYE_RADIUS, 5.f, 25.f);
		ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
		if (ImGui::Button("Reset##1"))
		{
			param.DYE_RADIUS = 16.0f;
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
		float *fluidPos = glm::value_ptr(param.FLUID_POSITION);
		ImGui::SliderFloat3("Dye position", fluidPos, 0.0f, 1.0f);
		param.FLUID_POSITION = glm::make_vec3(fluidPos);
		ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
		if (ImGui::Button("Reset##2"))
		{
			param.FLUID_POSITION = glm::vec3(0.5f, 0.2f, 0.5f);
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
		float* fluidColor = glm::value_ptr(param.DYE_COLOR);
		ImGui::ColorEdit3("Dye Color", fluidColor);
		param.DYE_COLOR = glm::make_vec3(fluidColor);
		ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
		if (ImGui::Button("Reset##3"))
		{
			param.DYE_COLOR = glm::vec3(1.0f);
		}


		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
	    ImGui::SliderFloat("Dissipation", &imguiDissipation, 0, 1.f);
		param.DISSIPATION = glm::mix(1.f, 0.9f, imguiDissipation);
		ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
		if (ImGui::Button("Reset##4"))
		{
			imguiDissipation = 0.0f;
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
		ImGui::SliderFloat("Vorticity", &param.VORTICITY, 0.f, 2.5f);
		ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
		if (ImGui::Button("Reset##5"))
		{
			param.VORTICITY = 0.35f;
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
		ImGui::Checkbox("MacCormack Dye", &useMacCormackDye);

		ImGui::SameLine(ImGui::GetStyle().GrabMinSize + ImGui::GetItemRectSize().x + ImGui::GetStyle().ItemSpacing.x);
		ImGui::Checkbox("MacCormack Velocity", &useMacCormackVelocity);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.f);
		char* buttonString = isBuoyancy ? "Switch to impulse" : "Switch to buoyancy";
		ImGui::SetCursorPosX((imguiSize.x - ImGui::CalcTextSize(buttonString).x) / 4.f);
		if (ImGui::Button(buttonString))
		{
			imguiIsBuoyancy = !imguiIsBuoyancy;
		}


		ImVec2 imguiSize2 = ImGui::GetWindowSize();

		this->ImGuiBuoyancyImpulseSwitcher();

		const char* items[]{ "Dye", "Velocity", "Pressure", "Divergence" };
		int selectedItem = 0;

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.f);
		if (ImGui::BeginListBox("Render target", ImVec2(imguiSize.x * 0.65f, (ImGui::CalcTextSize("Pressure").y + ImGui::GetStyle().FramePadding.y * 2) * 4)))
		{
			if (ImGui::Selectable("Dye", drawBuffer == &dye.readFBO))
			{
				drawBuffer = &dye.readFBO;
			}
			else if (ImGui::Selectable("Velocity", drawBuffer == &velocity.readFBO))
			{
				drawBuffer = &velocity.readFBO;
			}
			else if (ImGui::Selectable("Pressure", drawBuffer == &pressure.readFBO))
			{
				drawBuffer = &pressure.readFBO;
			}
			else if (ImGui::Selectable("Divergence", drawBuffer == &divergence))
			{
				drawBuffer = &divergence;
			}
			ImGui::EndListBox();
		}





		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
		ImGui::SetCursorPosX((imguiSize.x - ImGui::CalcTextSize("Reset all to default").x) / 4.f);
		if (ImGui::Button("Reset all to default"))
		{
			param.DYE_RADIUS = 16.0f;
			param.FLUID_POSITION = glm::vec3(0.5f, 0.2f, 0.5f);
			param.DYE_COLOR = glm::vec3(1.0f);
			imguiDissipation = 0.01f;
			param.VORTICITY = 0.35f;
			param.BUOYANCY = 1.0f;
			param.TEMPERATURE = 12.f;
			param.FLUID_WEIGHT = 0.125f;
			param.IMPULSE_POSITION = glm::vec3(0.5f, 0.2f, 0.5f);
			param.IMPULSE_RADIUS = 12.f;
			param.IMPULSE_FORCE = glm::vec3(0.f, 100.f, 0.0f);
			drawBuffer = &dye.readFBO;
			useMacCormackDye = false;
			useMacCormackVelocity = false;
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Fluid::ImGuiBuoyancyImpulseSwitcher()
	{
		ImVec2 imguiSize = ImGui::GetWindowSize();

		if (imguiIsBuoyancy)
		{
			isBuoyancy = true;
			isImpulse = false;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
			ImGui::SliderFloat("Buoyancy", &param.BUOYANCY, -3.f, 3.f);
			ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
			if (ImGui::Button("Reset##6"))
			{
				param.BUOYANCY = 1.0f;
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
			float fluidWeight = -param.FLUID_WEIGHT;
			ImGui::SliderFloat("Fluid Weight", &fluidWeight, -1.5f, 1.5f);
			param.FLUID_WEIGHT = -fluidWeight;
			ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
			if (ImGui::Button("Reset##7"))
			{
				param.FLUID_WEIGHT = 0.125f;
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
			ImGui::SliderFloat("Temperature", &param.TEMPERATURE, -373, 1000.f);
			ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
			if (ImGui::Button("Reset##8"))
			{
				param.TEMPERATURE = 12.f;
			}
		}
		else
		{
			isBuoyancy = false;
			isImpulse = true;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
			float* impulseForce = glm::value_ptr(param.IMPULSE_FORCE);
			ImGui::SliderFloat3("Force", impulseForce, -200.f, 200.0f);
			param.IMPULSE_FORCE = glm::make_vec3(impulseForce);
			ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
			if (ImGui::Button("Reset##6"))
			{
				param.IMPULSE_FORCE = glm::vec3(0.f, 100.f, 0.0f);
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
			float* impulsePos = glm::value_ptr(param.IMPULSE_POSITION);
			ImGui::SliderFloat3("Impulse pos", impulsePos, 0.0f, 1.0f);
			param.IMPULSE_POSITION = glm::make_vec3(impulsePos);
			ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
			if (ImGui::Button("Reset##7"))
			{
				param.IMPULSE_POSITION = glm::vec3(0.5f, 0.2f, 0.5f);
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
			ImGui::SliderFloat("Impulse radius", &param.IMPULSE_RADIUS, 5.f, 25.f);
			ImGui::SameLine(imguiSize.x - (ImGui::CalcTextSize("Reset").x + ImGui::GetStyle().FramePadding.x * 2 + ImGui::GetFontSize()));
			if (ImGui::Button("Reset##8"))
			{
				param.IMPULSE_RADIUS = 12.0f;
			}

		}
	}

	void Fluid::ImGuiCalcFPS()
	{

		float fps = ImGui::GetIO().Framerate;

		std::string fpsString = "FPS: " + std::to_string(fps);

		ImGui::Text("INFO:");


		const GLubyte* renderer = glGetString(GL_RENDERER);
		
		char* gpuName = "?";
		if (renderer)
			gpuName = (char*)renderer;

		ImGui::Text("GPU: %s", gpuName);

		ImGui::Text("FPS: %i", (int)fps);

		ImGui::SameLine(ImGui::CalcTextSize("FPS: 00	").x + ImGui::GetStyle().ItemSpacing.x);

		ImGui::Text("Latency: %.2fms", 1000.f / fps);
		
		ImGui::Text("Resolution: %ix%i", (int)screenSize.x, (int)screenSize.y);

		ImGui::SameLine(ImGui::CalcTextSize("Resolution: 1000x%1000").x + ImGui::GetStyle().ItemSpacing.x);

		ImGui::Text("Jacobi Iterations: %i", param.JACOBI_ITERATIONS);

	}

}