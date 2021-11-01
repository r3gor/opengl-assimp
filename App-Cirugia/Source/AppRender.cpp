#include "AppOpenGL.h"
#include "Panel.hpp"
#include "Model_old.h"

#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include "FlyingCamera.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Sphere.h"

#include "Model.h"

#define LOG(x) std::cout<<"LOG:::"<<#x<<std::endl;

Shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
Shader vertexShaderLoadModel, fragmentShaderLoadModel;
ShaderProgram mainProgramLoadModel;
Sphere sphereShape1{20, 60, 60, true};
Sphere sphereShape2{15, 60, 60, true};
Sphere sphereShape3{10, 60, 60, true};
Sphere sphereShape4{5, 60, 60, true};

ModelOld vaso("models/vessel.obj", "vessel.prop", false);
ModelOld vaso2("models/vessel.obj", "vessel2.prop", false);
//ModelOld spongebob("models/spongebob.obj", "spongebob.prop", false);
ModelOld hand("models/hand.obj", "hand.prop", false);
//ModelOld thor("models/thor.obj", "thor.prop", false);
ModelOld ball("models/head.obj", "head.prop", false);
ModelOld knife("models/knife.obj", "knife.prop", false);

Model backpack("../Resources/Models/backpack/backpack.obj", "backpack.prop", false);
Model nanosuit("../Resources/Models/Nanosuit/nanosuit.obj", "nanosuit.prop");
Model shenlong("../Resources/Models/shenron-dragon-ball/source/Shenron/Shenron.FBX", "shenlong.prop", false);
Model wolf("../Resources/Models/Wolf/Wolf.obj", "Wolf.prop", false);


bool renderWire = false;
float tuboSpeed = 80;
float normalSpeed;
bool turboCamera = false;

float speedCamera = 5.0f;
FlyingCamera camera(glm::vec3(0.0f, 8.0f, 20.0f), glm::vec3(0.0f, 8.0f, 19.0f), glm::vec3(0.0f, 1.0f, 0.0f), speedCamera);
float rotationAngleRad; // in radians

void OpenGLWindow::initializeScene(){
	// imgui init
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    vertexShader.loadShaderFromFile("shaders/vertshader.glsl", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("shaders/fragshader.glsl", GL_FRAGMENT_SHADER);

	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded())
	{
		closeWindow(true);
		return;
	}

	mainProgram.createProgram();
	mainProgram.addShaderToProgram(vertexShader);
	mainProgram.addShaderToProgram(fragmentShader);

    if (!mainProgram.linkProgram())
	{
		closeWindow(true);
		return;
	}

	vertexShaderLoadModel.loadShaderFromFile("Source/1.model_loading.vs", GL_VERTEX_SHADER);
	fragmentShaderLoadModel.loadShaderFromFile("Source/1.model_loading.fs", GL_FRAGMENT_SHADER);
	mainProgramLoadModel.createProgram();
	mainProgramLoadModel.addShaderToProgram(vertexShaderLoadModel);
	mainProgramLoadModel.addShaderToProgram(fragmentShaderLoadModel);
	mainProgramLoadModel.linkProgram();

	//stbi_set_flip_vertically_on_load(true);

    sphereShape1.createVertexObjects();
    sphereShape1.initData();

	sphereShape2.createVertexObjects();
    sphereShape2.initData();

    sphereShape3.createVertexObjects();
    sphereShape3.initData();

    sphereShape4.createVertexObjects();
    sphereShape4.initData();

	vaso.init();
	vaso2.init();
	shenlong.Init();
	hand.init();
	backpack.Init();
	ball.init();
	knife.init();
	nanosuit.Init();
	wolf.Init();
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void OpenGLWindow::renderScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LINE_SMOOTH);	
    glLineWidth(10);

	mainProgram.useProgram();
	mainProgram["matrices.projectionMatrix"] = getProjectionMatrix();
	mainProgram["matrices.viewMatrix"] = camera.getViewMatrix();
	mainProgram["matrices.modelMatrix"] = glm::mat4(1.0);
	
	if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)   renderWire = true;
	if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) renderWire = false;
	
	//if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
	//	normalSpeed = Panel::speedCamera != tuboSpeed? Panel::speedCamera:normalSpeed;
	//	Panel::speedCamera = tuboSpeed;
	//}
	//if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
	//	Panel::speedCamera = normalSpeed;
	//}

	if (renderWire){
		sphereShape1.renderWire();
		sphereShape2.renderWire();
		sphereShape3.renderWire();
		sphereShape4.renderWire();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		sphereShape1.renderFill();
		sphereShape2.renderFill();
		sphereShape3.renderFill();
		sphereShape4.renderFill();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	mainProgram["matrices.modelMatrix"] = vaso.getModelMatrix();
	vaso.draw(0);

	mainProgram["matrices.modelMatrix"] = vaso2.getModelMatrix();
	vaso2.draw(0);

	//mainProgram["matrices.modelMatrix"] = spongebob.getModelMatrix();
	//spongebob.draw(0);

	mainProgram["matrices.modelMatrix"] = hand.getModelMatrix();
	hand.draw(0);

	/*mainProgram["matrices.modelMatrix"] = thor.getModelMatrix();
	thor.draw(0);*/

	mainProgram["matrices.modelMatrix"] = ball.getModelMatrix();
	ball.draw(0);

	/*
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(8.0, 4.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, 3.14f + 3.14f/4.0f, glm::vec3(1, 0, 0));
	*/
	mainProgramLoadModel.useProgram();

	mainProgramLoadModel["projection"] = getProjectionMatrix();
	mainProgramLoadModel["view"] = camera.getViewMatrix();

	mainProgramLoadModel["model"] = nanosuit.getModelMatrix();
	nanosuit.Draw(mainProgramLoadModel);

	mainProgramLoadModel["model"] = shenlong.getModelMatrix() /* * modelMatrix*/;
	shenlong.Draw(mainProgramLoadModel);

	mainProgramLoadModel["model"] = backpack.getModelMatrix();
	backpack.Draw(mainProgramLoadModel);

	mainProgramLoadModel["model"] = wolf.getModelMatrix();
	wolf.Draw(mainProgramLoadModel);

	mainProgram.useProgram();
	mainProgram["matrices.viewMatrix"] = glm::mat4(1.0f);
	glm::mat4 knife_trans = glm::rotate(glm::mat4(1.0f), 3.14f*3.0f/2.0f, glm::vec3(1,0,0));
	mainProgram["matrices.modelMatrix"] = knife.getModelMatrix() * knife_trans;
	knife.draw(0);

	camera.setMoveSpeed(Panel::speedCamera);

    Panel::renderMainWindow();
	vaso.editPanel();
	vaso2.editPanel();
	shenlong.editPanel();
	hand.editPanel();
	backpack.editPanel();
	ball.editPanel();
	knife.editPanel();
	nanosuit.editPanel();
	wolf.editPanel();
    Panel::endMainWindow();

	rotationAngleRad += glm::radians(sof(45.0f));

	std::string windowTitleWithFPS = "FPS: "
		+ std::to_string(getFPS()) +
		", VSync: " + (isVerticalSynchronizationEnabled() ? "On" : "Off") + " (Press F3 to toggle)";
	glfwSetWindowTitle(getWindow(), windowTitleWithFPS.c_str());
}

void OpenGLWindow::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();
}

void OpenGLWindow::handleInput()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

	if (keyPressedOnce(GLFW_KEY_F3)) {
		setVerticalSynchronization(!isVerticalSynchronizationEnabled());
	}

	if (keyPressedOnce(GLFW_KEY_Q)) {
		cameraOn = cameraOn? false:true;
		int posX, posY, width, height;
		glfwGetWindowPos(getWindow(), &posX, &posY);
		glfwGetWindowSize(getWindow(), &width, &height);
		glfwSetCursorPos(_window, posX + width / 2, posY + height / 2);
	}

	if (cameraOn){
		int posX, posY, width, height;
		glfwGetWindowPos(getWindow(), &posX, &posY);
		glfwGetWindowSize(getWindow(), &width, &height);
		camera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));

		camera.update([this](int keyCode) {return this->keyPressed(keyCode); },
			[this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
			[this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
			[this](float f) {return this->sof(f); });
	}
}

void OpenGLWindow::onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
